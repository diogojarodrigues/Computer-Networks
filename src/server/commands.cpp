#include "./protocol.hpp"
#include "./utils.hpp"

#include "commands.hpp"

void login(string request) {

    if (
        request.length() != 20
        || request[3] != ' '
        || request[10] != ' '
        || request[19] != '\n'
        || !isUid(request.substr(4, 6))
        || !isPassword(request.substr(11, 8))
    ) {
        if (DEBUG) printf("login: wrong arguments\n");
        write_udp_message("ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";
    
    if (fs::exists(user_folder_path)) {                     //User exists

        if (passwordsMatch(user_folder_path + "pass.txt", password)) {
            write_udp_message("RLI OK\n");
        } else {
            write_udp_message("RLI NOK\n");
        }

    } else {                                                //User does not exist, create one

        fs::create_directory(user_folder_path);
        fs::create_directory(user_folder_path + "hosted");
        fs::create_directory(user_folder_path + "bidded");
        createFile(user_folder_path + "/login.txt", "");
        createFile(user_folder_path + "/pass.txt", password);        
        
        write_udp_message("RLI REG\n");
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
        if (DEBUG) printf("invalid udp command\n");
        write_udp_message("ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";

    if (fs::exists(user_folder_path)) {

        if (!passwordsMatch(user_folder_path + "pass.txt", password)) {
            printf("pasword does not match\n");
            write_udp_message("ERR\n");
            return;
        }
        
        if (fs::exists(user_folder_path + "/pass.txt")) {

            if (unregister == true) fs::remove(user_folder_path + "/pass.txt");
            fs::remove(user_folder_path + "/login.txt");
            write_udp_message("RLO OK\n");

        } else {
            write_udp_message("RLO NOK\n");
        }

    } else {
        printf("user %s does not exist\n", uid.c_str());
        write_udp_message("RLO UNR\n");
    }

};

void unregister(string request) {};
void my_auctions(string request) {};
void my_bids(string request) {};
void list(string request) {};
void show_record(string request) {};


// ################ TCP COMMANDS ################
void openn(string request) {

    printf("openn: %s\n", request.c_str());
    vector<string> fields = split(request);
    printf("fields: ");
    for (string token : fields) {
        printf("_%s_ ", token.c_str());
    }
    printf("\nFields size: %zu\n", fields.size());

    if (
        fields.size() != 8 ||
        !isUid(fields[1]) ||
        !isPassword(fields[2]) ||
        !isName(fields[3]) ||
        !isValue(fields[4]) ||
        !isDuration(fields[5]) ||
        !isFileName(fields[6]) ||
        !isFileSize(fields[7])
    ) {
        printf("open: invalid arguments\n");
        write_tcp_message("ERR\n");
        return;
    }

    string uid = fields[1];
    string password = fields[2];
    string name = fields[3];
    // int start_value = stoi(fields[4]);
    // int time_active = stoi(fields[5]);
    string file_name = fields[6];
    // int file_size = stoi(fields[7]);
    string aid = generateAid();

    // Create auction folder
    string user_folder_path = "src/server/data/auctions/" + aid + "/";
    fs::create_directory(user_folder_path);
    fs::create_directory(user_folder_path + "bids/");
    createFile(user_folder_path + "start.txt", "");
    createFile(user_folder_path + "end.txt", "");
    createFile(user_folder_path + file_name, "");

    // Casos em que devemos enviar o ROA NOK
    // Casos em que deviamos dar user not logged in

    // saveImage();

    write_tcp_message("ROA OK\n");
};

void closee(string request) {

};

void show_asset(string request) {

};

void bid(string request) {

};