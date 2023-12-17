#include "protocol.hpp"
#include "utils.hpp"

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

        write_udp_message("RLI REG\n");                             //TODO: (fazer teste) é suposto ser RLI REG para ambos os casos em que o user não está registado? caso em que nunca foi criado e caso em que foi criado
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
    if ( !user_registered(uid) ) { 
        if (DEBUG) cout << "logout: user %s does not exist\n", uid.c_str();
        write_udp_message(opcode + " UNR\n");
        return;
    }

    // Password does not match
    if (!passwordsMatch(uid, password)) {
        if (DEBUG) cout << "logout: password does not match\n";
        write_udp_message("ERR\n");                                     //TODO: (fazer teste) é suposto ser opcode + " NOK?
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

void my_auctions(string request) {
    vector<string> fields = split(request);
    if(fields.size()!=2 || !isUid(fields[1])){
        if (DEBUG) cout << "my_auctions: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }
    string uid = fields[1];
    if(!user_loggged_in(uid)){
        write_udp_message("RMA NLG\n");
        if (DEBUG) cout << "my_auctions: user not logged in\n";
        return;
    }
    string path = "./src/server/data/users/" + uid + "/hosted/";
    vector<string> auctions;
    string message ="";
    for (const auto & entry : fs::directory_iterator(path)){
        string filePath = entry.path().string();
        string aid = filePath.substr(filePath.length()-7,3);
        auctions.push_back(aid);
    }
    if (auctions.size()==0){
        write_udp_message("RMA NOK\n");
        return;
    }
    message= "RMA OK";
    sort(auctions.begin(), auctions.end());
    for(string aid : auctions){
        if(auction_closed(aid)){
            message+= " " + aid + " 0";
        }else{
            message+= " " + aid + " 1";
        }
    }
    message+="\n";
    write_udp_message(message);
};

void my_bids(string request) {
    vector<string> fields = split(request);
    if(fields.size()!=2 || !isUid(fields[1])){
        if (DEBUG) cout << "my_bids: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }

    string uid = fields[1];
    
    if(!user_loggged_in(uid)){
        write_udp_message("RMB NLG\n");
        if (DEBUG) cout << "my_bids: user not logged in\n";
        return;
    }

    string path = "./src/server/data/users/" + uid + "/bidded/";
    vector<string> auctions;
    string message ="";
    for (const auto & entry : fs::directory_iterator(path)){
        string filePath = entry.path().string();
        string aid = filePath.substr(filePath.length()-7,3);
        auctions.push_back(aid);
    }

    if (auctions.size()==0){
        write_udp_message("RMB NOK\n");
        return;
    }

    message= "RMB OK";
    sort(auctions.begin(), auctions.end());
    for(string aid : auctions){
        if(auction_closed(aid)){
            message+= " " + aid + " 0";
        }else{
            message+= " " + aid + " 1";
        }
    }
    message+="\n";
    write_udp_message(message);
};

void list(string request) {
    vector<string> fields = split(request);
    if(fields.size()!=1 ){
        if (DEBUG) cout << "list: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }
    string path = "./src/server/data/auctions/";
    vector<string> auctions;
    string message ="";
    for (const auto & entry : fs::directory_iterator(path)){
        string filePath = entry.path().string();
        string aid = filePath.substr(filePath.length()-3,3);
        auctions.push_back(aid);
    }
    if (auctions.size()==0){
        write_udp_message("RLS NOK\n");
        return;
    }
    message= "RLS OK";
    sort(auctions.begin(), auctions.end());
    for(string aid : auctions){
        if(auction_closed(aid)){
            message+= " " + aid + " 0";
        }else{
            message+= " " + aid + " 1";
        }
    }
    message+="\n";
    write_udp_message(message);
};

void show_record(string request) {

    if (
        request.length() != 8
        || request[3] != ' '
        || request[7] != '\n'
        || !isAid(request.substr(4, 3))
    ) {
        if (DEBUG) cout << "show_record: wrong arguments\n";
        write_udp_message("ERR\n");
        return;
    }

    string aid = request.substr(4, 3);
    
    if (!auction_exists(aid)) {
        if (DEBUG) cout << "show_record: auction does not exist\n";
        write_udp_message("RRC NOK\n");
        return;
    }

    string path = "src/server/data/auctions/" + aid;

    // Auction Data
    string content = readFile(path + "/start.txt");
    vector<string> fields = split(content);
    if (fields.size() != 8) {
        if (DEBUG) cout << "show_record: invalid file\n";
        write_udp_message("ERR\n");
        return;
    }
    string awnser = "RRC OK " + fields[0] + " " + fields[1] + " " + fields[2] + " " + fields[3] + " " + fields[5] + " " + fields[6] + " " + fields[4];

    // Bids data
    vector<string> bidders = getBidders(aid);
    for (const auto &bidder : bidders) {
        awnser += " B " + bidder;
    }
    
    // End data
    if (auction_closed(aid))
        awnser += " E " + readFile(path + "/end.txt");
    
    awnser += "\n";
    write_udp_message(awnser);
};


// ################ TCP COMMANDS ################
void openn(int sockett, string request) {

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
        write_tcp_message(sockett, "ERR\n");
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
        write_tcp_message(sockett, "ROA NOK\n");
        return;
    }

    if (!user_loggged_in(uid)) {
        if (DEBUG) cout << "open: user is not logged in" << endl;
        write_tcp_message(sockett, "ROA NLG\n");
        return;
    }

    // Create auction folder
    string auction_path = "src/server/data/auctions/" + aid + "/";
    fs::create_directory(auction_path);
    fs::create_directory(auction_path + "bids/");
    fs::create_directory(auction_path + "asset/");

    // Create start.txt file
    time_t start_fulltime = time(NULL);
    string start_file_content = uid + " " + name + " " + file_name + " " + start_value + " " + 
                        time_active  + " " + format_datetime(start_fulltime) + " " + 
                        to_string(start_fulltime);
    createFile(auction_path + "start.txt", start_file_content);

    // Create hosted.txt file
    string user_bid_path = "src/server/data/users/" + uid + "/hosted/" + aid + ".txt";
    string hosted_file_content="";
    createFile(user_bid_path, hosted_file_content);

    if (saveImage(sockett, auction_path + "asset/" + file_name, stoi(file_size)) != atoi(file_size.c_str())) {
        if (DEBUG) cout << "open: image could not be saved" << endl;
        fs::remove_all(auction_path);
        fs::remove(user_bid_path);
        write_tcp_message(sockett, "ROA NOK\n");
        return;
    }

    string response = "ROA OK " + aid + "\n";
    write_tcp_message(sockett, response);
};

void closee(int sockett, string request) {

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
        write_tcp_message(sockett, "ERR\n");
        return;
    }

    string uid = request.substr(4, 6);
    string password = request.substr(11, 8);
    string aid = request.substr(20, 3);

    // If the user does not exist
    if (!user_loggged_in(uid)) {
        if (DEBUG) cout << "close: user is not logged in\n";
        write_tcp_message(sockett, "RCL NLG\n");
        return;
    }

    //If the auction does not exist
    if (!auction_exists(aid)) {
        if (DEBUG) cout << "close: auction does not exist\n";
        write_tcp_message(sockett, "RCL EAU\n");
        return;
    }

    //If the auction is not owned by user id
    if (uid != getAuctionOwner(aid)) {
        if (DEBUG) cout << "close: auction is not owned by user id\n";
        write_tcp_message(sockett, "RCL EOW\n");
        return;
    }

    //If the auction is already closed
    if (auction_closed(aid)) {
        if (DEBUG) cout << "close: auction is already closed\n";
        write_tcp_message(sockett, "RCL END\n");
        return;
    }
    
    time_t end_time = time(NULL);
    string duration = getAuctionDuration(aid, end_time);

    string file = "src/server/data/auctions/" + aid + "/end.txt";
    string end_content = format_datetime(end_time) + " " + duration;

    createFile(file, end_content);
    write_tcp_message(sockett, "RCL OK\n");

};

void show_asset(int sockett, string request) {

    if (
        request.length() != 8
        || request[3] != ' '
        || request[7] != '\n'
        || !isAid(request.substr(4, 3))
    ) {
        if (DEBUG) cout << "show_asset: wrong arguments\n";
        write_tcp_message(sockett, "ERR\n");
        return;
    }

    string aid = request.substr(4, 3);

    if (!auction_exists(aid)) {
        if (DEBUG) cout << "show_asset: auction does not exist\n";
        write_tcp_message(sockett, "RSA NOK\n");
        return;
    }

    write_tcp_message(sockett, "RSA OK ");
    sendImage(sockett, aid);

};

void bid(int sockett, string request) {
    vector<string> fields = split(request);

    if (fields.size() != 5 || !isUid(fields[1]) || !isPassword(fields[2]) || !isAid(fields[3]) || !isValue(fields[4])){
        if (DEBUG) cout << "open: invalid arguments" << endl;
        write_tcp_message(sockett, "ERR\n");
        return;
    }

    string uid = fields[1];
    string password = fields[2];
    string aid = fields[3];
    string value = fields[4];

    int valueint=stoi(value);

    if(!auction_exists(aid)|| auction_closed(aid)){
        write_tcp_message(sockett, "RBD NOK\n");
        return;
    }
    if(!user_loggged_in(uid)|| !passwordsMatch(uid,password)){
        write_tcp_message(sockett, "RBD NLG\n");
        return;
    }
    if(getAuctionOwner(aid)==uid){
        write_tcp_message(sockett, "RBD ILG\n");
        return;
    }
    if(valueint<=getHighestBid(aid)){
        write_tcp_message(sockett, "RBD REF\n");
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

    time_t bid_time = time(NULL);
    string duration = getAuctionDuration(aid, bid_time);

    // Create bid.txt file
    string bid_path = "src/server/data/auctions/" + aid + "/bids/" + bid + ".txt";
    string content = uid + " " + value + " " + format_datetime(time(NULL)) + " " + duration;
    createFile(bid_path, content);

    // Create bidded.txt file
    string user_bid_path = "src/server/data/users/" + uid + "/bidded/" + aid + ".txt";
    content="";
    createFile(user_bid_path, content);

    write_tcp_message(sockett, "RBD ACC\n");
};