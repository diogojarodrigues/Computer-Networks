#include "./utils.hpp"

void login(string request) {

    if (
        request.length() != 20
        || request[3] != ' '
        || request[10] != ' '
        || request[19] != '\n'
        || !isUid(request.substr(4, 6))
        || !isPassword(request.substr(11, 8))
    ) {
        printf("invalid udp command\n");
        sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        return;
    }

    string uid = request.substr(4, 10);
    string password = request.substr(11, 19);

    //todo: implementar a lógica toda


    // if (!folderExists(uid, true)) {                     //User does not exist, create it
    //     printf("user %s does not exist, creating them...\n", uid.c_str());
    //     fs::create_directory("data/users/" + uid);
    //     fs::create_file("data/users/" + uid + "/login.txt");
    //     fs::create_file("data/users/" + uid + "/pass.txt");
    //     fs::create_folder("data/users/" + uid + "/hosted");
    //     fs::create_folder("data/users/" + uid + "/bidded");
    //     sendto(udp_socket, "RLI REG\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
    // }



    sendto(udp_socket, "RLI OK\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
}

void logout(string request) {};
void unregister(string request) {};
void my_auctions(string request) {};
void my_bids(string request) {};
void list(string request) {};
void show_record(string request) {};


// ################ TCP COMMANDS ################
void openn(string request) {};
void closee(string request) {};
void show_asset(string request) {};
void bid(string request) {};