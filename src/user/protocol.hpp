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

#define PORT "58097"
#define SERVER_IP "localhost"

using namespace std;

enum type {
    RECEIVE_TCP_IMAGE,
    SEND_TCP_IMAGE,
    DEFAULT
};

extern const char* port;
extern const char* server_ip;

string send_udp_request(string message);

void receive_tcp_image(int fd);

string send_tcp_request(string message, type type=DEFAULT, ifstream* file=nullptr);

#endif // PROTOCOL_HPP
