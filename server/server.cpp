#include "./utils.hpp"

void handle_udp_message() {

    string request = read_udp_message();

    if (
        request.length() > 21
        || request.length() < 3
        || request[request.size() - 1] != '\n'
    ) {
        printf("invalid udp command\n");
        sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        return;
    }

    string opcode = request.substr(0, 3);
    printf("received UDP request: %s\n", request.c_str());

    if (opcode == "LIN") {
        login(request);
    } else if (opcode == "LOU") {
        logout(request);
    } else if (opcode == "UNR") {
        unregister(request);
    } else if (opcode == "LMA") {
        my_auctions(request);
    } else if (opcode == "LMB") {
        my_bids(request);
    } else if (opcode == "LST") {
        list(request);
    } else if (opcode == "SRC") {
        show_record(request);
    } else {
        printf("invalid udp command\n");
    }

}

void handle_tcp_message() {
    //TODO: Ainda não testei o TCP
    printf("connection tcp received\n");

    char buffer[2048];
    int bytes_read;

    int new_socket = accept(tcp_socket, (struct sockaddr*)&tcp_addr, &tcp_addrlen);
    if (new_socket < 0) {
        printf("accept error\n");
        exit(-1);
    }

    // Handle data from the TCP connection
    while (1) {
        bytes_read = recv(new_socket, buffer, sizeof(buffer), 0);

        if (bytes_read <= 0) {
            // Connection closed or error
            close(new_socket);
            break;
        
        } else {
            // Handle received data
            printf("received TCP: %s\n", buffer);
            send(new_socket, "RLI OK\n", 8, 0);
        }
    }

}

int main(int argc, char** argv) {

    if (initialize_udp_socket() == -1) {
        printf("main: error initializing udp socket\n");
        exit(-1);
    }

    if (initialize_tcp_socket() == -1) {
        printf("main: error initializing tcp socket\n");
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
            printf("select error\n");
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
