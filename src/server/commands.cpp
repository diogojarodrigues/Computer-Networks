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

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";
    
    if (fs::exists(user_folder_path)) {                     //User exists
        printf("user %s exists\n", uid.c_str());

        if (passwordsMatch(user_folder_path + "pass.txt", password)) {
            sendto(udp_socket, "RLI OK\n", 7, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        } else {
            sendto(udp_socket, "RLI NOK\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        }

    } else {                                                //User does not exist, create one

        printf("user %s does not exist, creating them...\n", uid.c_str());

        fs::create_directory(user_folder_path);
        fs::create_directory(user_folder_path + "/hosted");
        fs::create_directory(user_folder_path + "/bidded");

        //todo: create function create file, folder, and read/write files
        std::ofstream outputLoginFile(user_folder_path + "/login.txt");
        if (!outputLoginFile.is_open()) {
            printf("error creating login.txt\n");
            sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
            return;
        }
        outputLoginFile.close();
        
        std::ofstream outputPassFile(user_folder_path + "/pass.txt");
        if (!outputPassFile.is_open()) {
            printf("error creating pass.txt\n");
            sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
            return;
        }

        outputPassFile << password;
        outputPassFile.close();

        sendto(udp_socket, "RLI REG\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
    }
}

void logout(string request, bool unregister) {

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

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";

    if (fs::exists(user_folder_path)) {

        if (!passwordsMatch(user_folder_path + "pass.txt", password)) {
            printf("pasword does not match\n");
            sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
            return;
        }
        
        if (fs::exists(user_folder_path + "/pass.txt")) {

            if (unregister == true) fs::remove(user_folder_path + "/pass.txt");
            fs::remove(user_folder_path + "/login.txt");
            sendto(udp_socket, "RLO OK\n", 7, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        
        } else {
            sendto(udp_socket, "RLO NOK\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        }

    } else {
        printf("user %s does not exist\n", uid.c_str());
        sendto(udp_socket, "RLO UNR\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
    }

};

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