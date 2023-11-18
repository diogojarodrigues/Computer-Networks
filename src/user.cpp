#include <iostream>
#include <string>
#include <unordered_set>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#define PORT "58011"
#define SERVER "tejo.tecnico.ulisboa.pt" 

using namespace std;

int send_udp_message(string command) {
    int fd,errcode; 
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_DGRAM,0);    //UDP socket
    if(fd==-1) exit(1);                 /*error*/

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_DGRAM;       //UDP socket
    
    errcode=getaddrinfo(SERVER, PORT, &hints, &res);
    if(errcode!=0) exit(1);            /*error*/
    
    n=sendto(fd, command.c_str(), command.size() + 1, 0, res->ai_addr, res->ai_addrlen);
    if(n==-1) exit(1);                 /*error*/

    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,128,0, (struct sockaddr*) &addr, &addrlen);
    if(n==-1) exit(1);                 /*error*/
    
    write(1,"received: ",6); 
    write(1,buffer,n);
    
    freeaddrinfo(res);
    close(fd);

    return 0;
}

int send_tcp_message(string command) {

    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_STREAM,0);   //TCP socket
    if (fd==-1) exit(1);                //error
    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;            //IPv4
    hints.ai_socktype=SOCK_STREAM;      //TCP socket
    
    errcode=getaddrinfo("tejo.tecnico.ulisboa.pt", PORT, &hints, &res);
    if(errcode!=0) exit(1);             /*error*/
    
    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1) exit(1);                  /*error*/
    
    n=write(fd, command.c_str(), command.size() + 1);
    if(n==-1) exit(1);                  /*error*/
    
    n=read(fd,buffer,128);
    if(n==-1) exit(1);                  /*error*/
    
    write(1,"received: ",6); 
    write(1,buffer,n);
    
    freeaddrinfo(res);
    close(fd);

    return 0;
}

int main(int argc, char** argv) {

    string command, op_code;


    while (true) {
        getline(cin, command);
        command[command.size()] = '\n';

        op_code = command.substr(0, 3);
            
        if (op_code == "EXI") {
            
            cout << "Exiting..." << endl;
            break;

        } else if (

            op_code == "OPA"
            || op_code == "CLS"
            || op_code == "SAS"
            || op_code == "BID"
        
        ) {
            
            cout << "Opening TCP connection..." << endl;
            send_tcp_message(command);
        
        } else if (

            op_code == "LIN"
            || op_code == "LOU"
            || op_code == "UNR"
            || op_code == "LMA"
            || op_code == "LMB"
            || op_code == "LST"
            || op_code == "SRC"
        
        ) {
            
            cout << "Opening UDP connection..." << endl;
            send_udp_message(command);
        
        } else if (

            op_code == "EXI"
        
        ) {
            
            cout << "Exiting..." << endl;
            break;
        
        } else {
            
            cout << "Invalid command" << endl;
        
        }


    } 

    return 0;
}
