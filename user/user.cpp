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

// #define PORT "58097"                     //TODO: UNCOMENT WHEN SUBMITTING
// #define PORT "localhost"                 //TODO: UNCOMENT WHEN SUBMITTING

#define PORT "58011"                        //TODO: CHANGE THIS WHEN SUBMITTING
#define SERVER "tejo.tecnico.ulisboa.pt"    //TODO: CHANGE THIS WHEN SUBMITTING

using namespace std;


// ############################################################
//                      AUXILIARY FUNCTIONS
// ############################################################

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


// ############################################################
//                      PROTOCOL FUNCTIONS
// ############################################################

string send_udp_message(string message) {
    
    int fd,errcode; 
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[128];

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
    n=recvfrom(fd,buffer,128,0, (struct sockaddr*) &addr, &addrlen);
    if(n==-1) exit(1);                 /*error*/

    freeaddrinfo(res);
    close(fd);

    return buffer;
}

int send_tcp_message(string message) {

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

    return 0;
}


// ############################################################
//                      COMMAND FUNCTIONS
// ############################################################

void login(vector<string> command) {

    // Check if the command is valid
    if (command.size()!=3) {
        cout << "login: format not valid!" << endl;
        return;
    }

    string uid, password;
    uid = command[1];
    password = command[2];

    if (uid.length() != 6 || isNumeric(uid) == false) {
        cout << "login: uid must be alphanumeric and have 6 digits" << endl;
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
    } else {
        cout << "login: error" << endl;
    }

}

void logout() {};
void unregister() {};
void exitt() {};
void open() {};
void closee() {};
void myauctions() {};
void mybids() {};
void list() {};
void show_asset() {};
void bid() {};
void show_record() {};


// ############################################################
//                          MAIN
// ############################################################

int main(int argc, char** argv) {

    vector<string> command;

    while (true) {
        
        get_input(&command);

        if (command[0]=="login") {
            login(command);
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
        } else {
            cout << "Invalid command" << endl;
        }

    } 

    return 0;
}