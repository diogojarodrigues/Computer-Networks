#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#define PORT "58098"                            //TODO: UNCOMENT WHEN SUBMITTING
#define SERVER_IP "localhost"                      //TODO: UNCOMENT WHEN SUBMITTING

//#define PORT "58011"                         //TODO: CHANGE THIS WHEN SUBMITTING
//#define SERVER_IP "tejo.tecnico.ulisboa.pt"     //TODO: CHANGE THIS WHEN SUBMITTING

using namespace std;

enum type {
    RECEIVE_TCP_IMAGE,
    SEND_TCP_IMAGE,
    DEFAULT
};

extern const char* port;
extern const char* server_ip;

string send_udp_message(string message);

void receive_tcp_image(int fd);

string send_tcp_message(string message, type type=DEFAULT, ifstream* file=nullptr);

#endif // PROTOCOL_HPP
