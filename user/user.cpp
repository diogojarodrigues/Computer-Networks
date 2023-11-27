#include <iostream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <cstring>

// #define PORT "58097"                     //TODO: UNCOMENT WHEN SUBMITTING
// #define PORT "localhost"                 //TODO: UNCOMENT WHEN SUBMITTING

#define PORT "58011"                        //TODO: CHANGE THIS WHEN SUBMITTING
#define SERVER "tejo.tecnico.ulisboa.pt"    //TODO: CHANGE THIS WHEN SUBMITTING

using namespace std;


// ############################################################
//                      AUXILIARY FUNCTIONS
// ############################################################

vector<string> command;
string current_uid = "";
string current_password = "";

void get_input(vector<string>* command) {
    string line;
    command->clear();
    
    // Read the entire line
    getline(cin, line);

    istringstream iss(line);                // Creating a string stream from the line
    string word;

    // Extract words from the line using stringstream and store them in the vector
    while (iss >> word) {
        command->push_back(word);
    }
}

bool isNumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false; // If any character is not a digit, return false
        }
    }
    return true; // All characters are digits
}

bool isAlphanumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

string get_auctions_bids(string response){
    int k=1; // 1 and 3 in aid, 0 in status
    string final = response.substr(7, response.size()-7);
    string auctions="";
    for (char c : final) {
        if(k>=1 && k<=4){
            auctions += c;
            k++;
            continue;
        }
        else if(c=='1')
            auctions += "is active\n";
        else if(c=='0')
            auctions += "is not active\n";
        else if(c==' ')
            k=1;
        else if(c=='\n')
            continue;
        }
        
    return auctions;  
}

void print_auctions(string auctions){
    
    printf("%s\n", auctions.c_str());

    int k=0; // 1 and 3 in aid, 0 in status
    for (char c : auctions) {
        if(k>=0 && k<=3){
            if (k==0)
                cout << "AID: ";
            cout << c;
            k++;
            continue;
        }
        else if(c=='1')
            cout << "is active\n";
        else if(c=='0')
            cout << "is not active\n";
        else if(c==' ')
            k=0;
        }
}

// ############################################################
//                      PROTOCOL FUNCTIONS
// ############################################################

string send_udp_message(string message) {
    
    int fd,errcode; 
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[99999];

    fd=socket(AF_INET,SOCK_DGRAM,0);    //UDP socket
    if (fd==-1) exit(1);                /*error*/

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_DGRAM;       //UDP socket
    
    errcode=getaddrinfo(SERVER, PORT, &hints, &res);
    if(errcode!=0) exit(1);            /*error*/
    
    n=sendto(fd, message.c_str(), message.size(), 0, res->ai_addr, res->ai_addrlen);
    if(n==-1) exit(1);                 /*error*/

    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,99999,0, (struct sockaddr*) &addr, &addrlen);
    if(n==-1) exit(1);                 /*error*/

    freeaddrinfo(res);
    close(fd);

    return buffer;
}

string send_tcp_message(string message) {

    int fd,errcode;
    ssize_t n;
    // socklen_t addrlen;               //TODO: WHY DONT WE NEED THIS?
    struct addrinfo hints,*res;
    // struct sockaddr_in addr;         //TODO: WHY DONT WE NEED THIS?
    char buffer[128];

    fd=socket(AF_INET,SOCK_STREAM,0);   //TCP socket
    if (fd==-1) exit(1);                //error
    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_STREAM;      //TCP socket
    
    errcode=getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &hints, &res);     //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    if(errcode!=0) exit(1);             /*error*/
    

    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1) exit(1);                  /*error*/
    
    n=write(fd, message.c_str(), message.size());
    if(n==-1) exit(1);                  /*error*/
    
    n=read(fd,buffer,128);
    if(n==-1) exit(1);                  /*error*/
    
    freeaddrinfo(res);                  //TODO: CHANGE THIS TO ONLY HAPPEN ONE TIME
    close(fd);

    return buffer;
}


// ############################################################
//                      COMMAND FUNCTIONS
// ############################################################

void login() {

    // Check if the user is already logged in
    if (!current_uid.empty() || !current_password.empty()) {
        cout << "You are already logged in" << endl;
        return;
    }

    // Check if the command is valid
    if (command.size()!=3) {
        cout << "login: format not valid!" << endl;
        return;
    }

    string uid, password;
    uid = command[1];
    password = command[2];

    if (uid.length() != 6 || isNumeric(uid) == false) {
        cout << "login: uid must be numeric and have 6 digits" << endl;
        return;
    }

    if (password.length() != 8 || isAlphanumeric(password) == false) {
        cout << "login: password must be alphanumeric and have 8 digits" << endl;
        return;
    }

    // Send the message to the server
    string request = "LIN " + uid + " " + password + "\n";
    string response = send_udp_message(request);

    // Check the response
    if (response == "RLI REG\n") {
        cout << "new user registered" << endl;
    } else if (response == "RLI OK\n") {
        cout << "successful login" << endl;
    } else if (response == "RLI NOK\n") {
        cout << "incorrect login attempt" << endl;
        return;
    } else {
        cout << "login: error" << endl;
        return;
    }

    // Save the current uid and password
    current_uid = uid;
    current_password = password;
}

void logout() {

    // Check if the command is valid
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should logged in first" << endl;
        return;
    }

    // Send the message to the server
    string request = "LOU " + current_uid + " " + current_password + "\n";
    string response = send_udp_message(request);

    // Check the response
    if (response == "RLO OK\n") {
        cout << "successful logout" << endl;
    } else if (response == "RLO NOK\n") {
        cout << "user not logged in" << endl;
    } else if (response == "RLO UNR\n") {
        cout << "unknown user" << endl;
    } else {
        cout << "logout: error" << endl;
    }

    // Clear the current uid and password
    current_uid.clear();
    current_password.clear();
};

void unregister() {

    // Check if the command is valid
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should logged in first" << endl;
        return;
    }

    // Send the message to the server
    string request = "UNR " + current_uid + " " + current_password + "\n";
    string response = send_udp_message(request);

    // Check the response
    if (response == "RUR OK\n") {
        cout << "successful unregister" << endl;
    } else if (response == "RUR NOK\n") {
        cout << "incorrect unregister attempt" << endl;
    } else if (response == "RUR UNR\n") {
        cout << "unknown user" << endl;
    } else {
        cout << "unregister: error" << endl;
    }

    // Clear the current uid and password
    current_uid.clear();
    current_password.clear();
};

void exitt() {

    // Check if the user is logged in
    if (!current_uid.empty() || !current_password.empty()) {
        cout << "You should logout first" << endl;
        return;
    }

    cout << "Exiting..." << endl;
    exit(0);
};

void open() {

    // Check if the command is valid
    if (command.size() != 5) {
        cout << "open: format not valid!" << endl;
        return;
    }
    
    string name, fname, start_value,timeactive;
    name = command[1];
    fname = command[2];
    start_value = command[3];
    timeactive = command[4];

    if (name.length() > 10 || !isAlphanumeric(name)) {
        cout << "open: password must be alphanumeric and have 8 digits" << endl;
        return;
    }

    if (start_value.length() > 6 || !isNumeric(start_value)) {
        cout << "open: start value must be numeric and " << endl;
        return;
    }

    if (timeactive.length() > 5 ||!isNumeric(timeactive)) {
        cout << "open: time active must be numeric" << endl;
        return;
    }

    string request = "OPA " + current_uid + " " + current_password + " " + name + " " + start_value + " " + timeactive + " " + fname + " " + fsize + " " + fdata + "\n";
    string response = send_tcp_message(request);

    printf("%s\n", response.c_str());
    string status = response.substr(0, 7);
    printf("%s\n", status.c_str());

    if (status == "ROA NOK") {
        cout << "auction could not be started" << endl;
    } else if (status == "ROA NLG") {
        cout << "user is not logged in" << endl;
    } else if (status == "ROA OK ") {
        string aid = response.substr(8, 3);
        cout << "AID =" + aid << endl;
    } else {
        cout << "open: error" << endl;
    }
};

void closee() {
    if (command.size()!=2) {
        cout << "close: format not valid!" << endl;
        return;
    }
    string aid;
    aid = command[1];
    if (aid.length()!= 3 || !isNumeric(aid)) {
        cout << "close: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "CLS " + current_uid + " " + current_password + " "+ aid +"\n";
    string response = send_tcp_message(request);

    if (response == "RCL OK\n") {
        cout << "auction was closed" << endl;
    } else if (response == "RCL NLG\n") {
        cout << "user is not logged" << endl;
    } else if (response == "RCL EAU\n") {
        cout << "auction does not exist" << endl;
    } else if (response == "RCL EOW\n") {
        cout << "auction not owned by the user" << endl;
    } else if (response == "RCL END\n") {
        cout << "auction has already finished" << endl;
    } else {
        cout << "close: error" << endl;
    }
    
}

void myauctions() {
    string request = "LMA " + current_uid +"\n";
    string response = send_udp_message(request);
    string status = response.substr(0, 7);
    if (status == "RMA NOK") {
        cout << "user UID has no ongoing auctions" << endl;
    } else if (status == "RMA NLG") {
        cout << "user is not logged" << endl;
    } else if (status == "RMA OK ") {
        string auctions = get_auctions_bids(response);
        cout << auctions << endl;
    } 
};

void mybids() {
    string request = "LMB " + current_uid +"\n";
    string response = send_udp_message(request);
    string status = response.substr(0, 7);

    if (status == "RMB NOK") {
        cout << "user UID has no ongoing bids" << endl;
    } else if (status == "RMB NLG") {
        cout << "user is not logged" << endl;
    } else if (status == "RMB OK ") {
        string auctions= get_auctions_bids(response);   
        cout << auctions << endl;
        return;
    } 
};

void list() {};

void show_asset() {};

void bid() {};

void show_record() {};

void show_current_user() {
    cout << "Current user -> UID: " << current_uid << " Pass: " << current_password << endl;
};

// ############################################################
//                          MAIN
// ############################################################

int main(int argc, char** argv) {

    while (true) {
        
        cout << ">> ";
        get_input(&command);

        if (command[0]=="login") {
            login();
        } else if (command[0]=="logout"){
            logout();
        } else if (command[0]=="unregister"){
            unregister();
        } else if (command[0]=="exit"){
            exitt();
        } else if (command[0]=="open"){
            open();
        } else if (command[0]=="close"){
            closee();
        } else if (command[0]=="myauctions" || command[0] == "ma"){
            myauctions();
        } else if (command[0]=="mybids" || command[0] == "mb"){
            mybids();
        } else if (command[0]=="list" || command[0] == "l"){
            list();
        } else if (command[0]=="show_asset" || command[0] == "sa"){
            show_asset();
        } else if (command[0]=="bid" || command[0] == "b"){
            bid();
        } else if (command[0]=="show_record" || command[0] == "sr"){
            show_record();
        } else if (command[0]=="show_user" || command[0] == "user"){
            show_current_user();
        } else {
            cout << "Invalid command" << endl;
        }

        cout << endl;
    } 

    return 0;
}