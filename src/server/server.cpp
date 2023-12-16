#include "./utils.hpp"
#include "./protocol.hpp"
#include "./commands.hpp"

void handle_udp_message() {

    string request = read_udp_message();


    if (
        request.length() > 21
        || request.length() < 3
        || request[request.size() - 1] != '\n'
    ) {
        if (DEBUG) cout << "handle_udp_message: wrong arguments\n";
        sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        return;
    }

    string opcode = request.substr(0, 3);

    if (opcode == "LIN") {
        login(request);
    } else if (opcode == "LOU") {
        logout(request);
    } else if (opcode == "UNR") {
        logout(request, true);
    } else if (opcode == "LMA") {
        my_auctions(request);
    } else if (opcode == "LMB") {
        my_bids(request);
    } else if (opcode == "LST") {
        list(request);
    } else if (opcode == "SRC") {
        show_record(request);
    } else {
        cout << "invalid udp command\n";
    }

    cout << endl;
}

void read_file() {

}

void handle_tcp_message() {
    
    int sockett = connect_to_client();
    if (sockett == -1) {
        if (DEBUG) cout << "handle_tcp_message: error connecting to client\n";
        exit(-1);
    }

    string request = read_tcp_message(sockett);
    if (
        request.length() < 3
    ) {
        if (DEBUG) cout << "handle_tcp_message: wrong arguments\n";
        write_tcp_message(sockett, "ERR\n");
        return;
    }

    string opcode = request.substr(0, 3);
    if (opcode == "OPA") {
        if (DEBUG) cout << "\n";
        openn(sockett, request);
    } else if (opcode == "CLS") {
        closee(sockett, request);
    } else if (opcode == "SAS") {
        show_asset(sockett, request);
    } else if (opcode == "BID") {
        bid(sockett, request);
    } else {
        cout << "invalid tcp command\n";
    }
  
}

int main(int argc, char** argv) {

    // Ignore SIGPIPE
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1) exit(1);

    // Delete fork chieldrens
    signal(SIGCHLD, SIG_IGN);

    // Create data directory if it doesn't exist
    string path = "src/server/data";
    if (!fs::exists(path)) fs::create_directory(path);
    if (!fs::exists(path + "/users")) fs::create_directory(path + "/users");
    if (!fs::exists(path + "/auctions")) fs::create_directory(path + "/auctions");

    if(argc==2){
        if(!strcmp(argv[1],"-v")){
            verbose = true;
        }
    }
    if(argc==3){
        if(!strcmp(argv[1],"-p")){
            port = (const char*) argv[2];
        }
    }if(argc==4){
        if(!strcmp(argv[1],"-v")){
            verbose = true;
        }
        if(!strcmp(argv[2],"-p")){
            port = (const char*)argv[3];
        }
        if(!strcmp(argv[1],"-p")){
            port = (const char*)argv[2];
        }
        if(!strcmp(argv[3],"-v")){
            verbose = true;
        }
    }

    if (initialize_udp_socket() == -1) {
        cerr << "main: error initializing udp socket\n";
        exit(-1);
    }

    if (initialize_tcp_socket() == -1) {
        cerr << "main: error initializing tcp socket\n";
        exit(-1);
    }

    //File descriptor set for select
    fd_set readfds;
    while (1) {

        FD_ZERO(&readfds);

        // Add UDP socket to set
        FD_SET(udp_socket, &readfds);

        // Add TCP socket to set
        FD_SET(tcp_socket, &readfds);

        // Use select to monitor sockets
        int max_sd = (udp_socket > tcp_socket) ? udp_socket : tcp_socket;
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            cerr << "main: select error\n";
            exit(-1);
        }

        //If UDP socket is ready
        if (FD_ISSET(udp_socket, &readfds)) {
            handle_udp_message();
        }

        // If TCP socket is ready for a new connection
        if (FD_ISSET(tcp_socket, &readfds)) {
            handle_tcp_message();
        }
    }

    close_udp_socket();
    close_tcp_socket();

    return 0;
}
