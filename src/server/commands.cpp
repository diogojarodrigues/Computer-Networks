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
    

    if (!user_registered(uid)) {                                    // User not registered

        if (!user_exists(uid)) {                                    // User have never existed on the system
            fs::create_directory(user_folder_path);
            fs::create_directory(user_folder_path + "hosted");
            fs::create_directory(user_folder_path + "bidded");
        }
        
        createFile(user_folder_path + "/login.txt", "");             
        createFile(user_folder_path + "/pass.txt", password); 

        write_udp_message("RLI REG\n");                             //TODO: é suposto ser RLI REG para ambos os casos em que o user não está registado?
        return;
    }

    if (!passwordsMatch(uid, password)) {                           // Password does not match
        write_udp_message("RLI NOK\n");
        return;
    }

    if (!user_loggged_in(uid)) {                                    // User is not logged in
        createFile(user_folder_path + "/login.txt", "");
    }
    
    write_udp_message("RLI OK\n");
}

void logout(string request, bool unregister) {
    
    string opcode = (unregister == true) ? "RUR" : "RLO";

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
        write_udp_message(opcode + " UNR\n");
        return;
    }

    // Password does not match
    if (!passwordsMatch(uid, password)) {
        if (DEBUG) cout << "logout: password does not match\n";
        write_udp_message("ERR\n");                                     //TODO: é suposto seropcode + " NOK?
        return;
    }
    
    // User is not logged in
    if ( !user_loggged_in(uid) ) {
        write_udp_message(opcode + " NOK\n");
        return;
    }

    // User is logged in
    if (unregister == true) fs::remove(user_folder_path + "/pass.txt");
    fs::remove(user_folder_path + "/login.txt");
    write_udp_message(opcode + " OK\n");
        
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


    string user_bid_path = "src/server/data/users/" + uid + "/hosted/" + aid + ".txt";
    content="";
    createFile(user_bid_path, content);


    saveImage(sockett, user_folder_path + "asset/" + file_name, stoi(file_size));

    string response = "ROA OK " + aid + "\n";
    write_tcp_message(response);
};

void closee(string request) {

    //0-2 3 4-9 10 11-18 19 20-22 23 
    if (
        request.length() != 24
        || request[3] != ' '
        || request[10] != ' '
        || request[19] != ' '
        || request[23] != '\n'
        || !isUid(request.substr(4, 6))
        || !isPassword(request.substr(11, 8))
        || !isAid(request.substr(20, 3))
    ) {
        if (DEBUG) cout << "close: wrong arguments\n";
        write_tcp_message("ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string aid = request.substr(20, 3);

    // If the user does not exist
    if (!user_loggged_in(uid)) {
        if (DEBUG) cout << "close: user is not logged in\n";
        write_tcp_message("RCL NLG\n");
        return;
    }

    //If the auction does not exist
    if (!auction_exists(aid)) {
        if (DEBUG) cout << "close: auction does not exist\n";
        write_tcp_message("RCL EAU\n");
        return;
    }

    //If the auction is not owned by user id
    if (uid != getAuctionOwner(aid)) {
        if (DEBUG) cout << "close: auction is not owned by user id\n";
        write_tcp_message("RCL EOW\n");
        return;
    }

    //If the auction is already closed
    if (auction_closed(aid)) {
        if (DEBUG) cout << "close: auction is already closed\n";
        write_tcp_message("RCL END\n");
        return;
    }
    

    //Close auction
    time_t start_fulltime = time(NULL);
    string file = "src/server/data/auctions/" + aid + "/end.txt";
    string content = start_datetime(start_fulltime) + " " + to_string(start_fulltime);

    createFile(file, content);
    write_tcp_message("RCL OK\n");

};

void show_asset(string request) {

    if (
        request.length() != 8
        || request[3] != ' '
        || request[7] != '\n'
        || !isAid(request.substr(4, 3))
    ) {
        if (DEBUG) cout << "show_asset: wrong arguments\n";
        write_tcp_message("ERR\n");
        return;
    }

    string aid = request.substr(4, 3);

    //TODO: talvez faltem mais verificações
    if (!auction_exists(aid)) {
        if (DEBUG) cout << "show_asset: auction does not exist\n";
        write_tcp_message("RSA NOK\n");
        return;
    }

    write_tcp_message("RSA OK ");
    sendImage(sockett, aid);

};

void bid(string request) {
    vector<string> fields = split(request);
    string uid = fields[1];
    string password = fields[2];
    string aid = fields[3];
    string value = fields[4];


    if (fields.size() != 5 || !isUid(uid) || !isPassword(password) || !isAid(aid) || !isValue(value.c_str())){
        if (DEBUG) cout << "open: invalid arguments" << endl;
        write_tcp_message("ERR\n");
        return;
    }

    int valueint=stoi(value);

    if(!auction_exists(aid)|| auction_closed(aid)){
        write_tcp_message("RBD NOK\n");
        return;
    }
    if(!user_loggged_in(uid)|| !passwordsMatch(uid,password)){
        write_tcp_message("RBD NLG\n");
        return;
    }
    if(getAuctionOwner(aid)==uid){
        write_tcp_message("RBD ILG\n");
        return;
    }
    if(valueint<=getHighestBid(aid)){
        write_tcp_message("RBD REF\n");
        return;
    }

    string bid;
    if(valueint<10){
        bid="00000"+value;
    }
    else if(valueint<100){
        bid="0000"+value;
    }
    else if(valueint<1000){
        bid="000"+value; 
    }
    else if(valueint<10000){
        bid="00"+value;
    }
    else if(valueint<100000){
        bid="0"+value;
    }
    else{
        bid=value;
    }
    string bid_path = "src/server/data/auctions/" + aid + "/bids/" + bid + ".txt";
    
    const string path = "src/server/data/auctions/" + aid + "/start.txt";
    ifstream inputStartFile(path);
    if (!inputStartFile.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }
    const int buffer_size = 512;
    char buffer[buffer_size] = "";
    inputStartFile.read(buffer, buffer_size);
    inputStartFile.close();
    //UID name assetfname startvalue timeactive start_ datetime startfulltime
    int k=0;
    string initial_fulltime;
    for(char c : buffer){
        if(c==' ')
            k++;
        else if(k==7)
            initial_fulltime+=c;
    }

    time_t start_fulltime = time(NULL);
    int time_active = stoi(initial_fulltime) - start_fulltime;
    string content = uid + " " + value + " " + start_datetime(time(NULL)) + " " + to_string(time_active);
    createFile(bid_path, content);
    string user_bid_path = "src/server/data/users/" + uid + "/bidded/" + aid + ".txt";
    content="";
    createFile(user_bid_path, content);
    write_tcp_message("RBD ACC\n");


};