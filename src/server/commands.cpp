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
        if (DEBUG) cout << "login: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";
    
    // User does not exist
    if (!user_exists(uid)) {

        fs::create_directory(user_folder_path);
        fs::create_directory(user_folder_path + "hosted");
        fs::create_directory(user_folder_path + "bidded");
        createFile(user_folder_path + "/login.txt", "");
        createFile(user_folder_path + "/pass.txt", password);   

        write_udp_message("RLI REG\n");
        return;
    }

    if (!passwordsMatch(uid, password)) {
        write_udp_message("RLI NOK\n");
        return;
    }
    
    write_udp_message("RLI OK\n");
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
        if (DEBUG) cout << "logout: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string user_folder_path = "./src/server/data/users/" + uid + "/";

    // User does not exist
    if ( !user_exists(uid) ) { 
        if (DEBUG) cout << "logout: user %s does not exist\n", uid.c_str();
        write_udp_message("RLO UNR\n");
        return;
    }

    // Password does not match
    if (!passwordsMatch(user_folder_path + "pass.txt", password)) {
        if (DEBUG) cout << "logout: password does not match\n";
        write_udp_message("ERR\n");
        return;
    }
    
    // User is not logged in
    if ( !user_loggged_in(uid) ) {
        write_udp_message("RLO NOK\n");
        return;
    }

    // User is logged in
    if (unregister == true) fs::remove(user_folder_path + "/pass.txt");
    fs::remove(user_folder_path + "/login.txt");
    write_udp_message("RLO OK\n");
        
};

void unregister(string request) {};
void my_auctions(string request) {};
void my_bids(string request) {};
void list(string request) {};
void show_record(string request) {};


// ################ TCP COMMANDS ################
void openn(string request) {

    vector<string> fields = split(request);

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
        if (DEBUG) cout << "open: invalid arguments" << endl;
        write_tcp_message("ERR\n");
        return;
    }

    string uid = fields[1];
    string password = fields[2];
    string name = fields[3];
    string start_value = fields[4];
    string time_active = fields[5];
    string file_name = fields[6];
    string file_size = fields[7];
    string aid = generateAid();

    if (aid == "ERR") {
        if (DEBUG) cout << "open: auction could not be started" << endl;
        write_tcp_message("ROA NOK\n");
        return;
    }

    if (!user_loggged_in(uid)) {
        if (DEBUG) cout << "open: user is not logged in" << endl;
        write_tcp_message("ROA NLG\n");
        return;
    }

    // Create auction folder
    string user_folder_path = "src/server/data/auctions/" + aid + "/";
    fs::create_directory(user_folder_path);
    fs::create_directory(user_folder_path + "bids/");
    fs::create_directory(user_folder_path + "asset/");

    time_t start_fulltime = time(NULL);
    string content = uid + " " + name + " " + file_name + " " + start_value + " " + 
                        time_active  + " " + start_datetime(start_fulltime) + " " + 
                        to_string(start_fulltime);


    createFile(user_folder_path + "start.txt", content);

    saveImage(sockett, user_folder_path + "asset/" + file_name, stoi(file_size));

    string response = "ROA OK " + aid + "\n";
    write_tcp_message(response);
};

void closee(string request) {

};

void show_asset(string request) {

};

void bid(string request) {

};