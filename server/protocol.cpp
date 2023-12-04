#include "protocol.hpp"

/* ################### GLOBAL VARIABLES ################### */

int udp_socket, tcp_socket;
struct addrinfo *udp_res, *tcp_res;
struct sockaddr_in udp_addr, tcp_addr;
socklen_t udp_addrlen, tcp_addrlen;

/* ################### UDP ################### */

int initialize_udp_socket() {
    struct addrinfo hints;
    int aux;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    aux = getaddrinfo(NULL, PORT, &hints, &udp_res);
    if (aux != 0) return -1;

    aux = ::bind(udp_socket, udp_res->ai_addr, udp_res->ai_addrlen);
    if (aux == -1) return -1;

    return 0;
}

string read_udp_message() {
    int aux, buffer_size = 2048;
    char buffer[buffer_size];

    udp_addrlen = sizeof(udp_addr);
    aux = recvfrom(udp_socket, buffer, buffer_size, 0, (struct sockaddr*) &udp_addr, &udp_addrlen);
    if (aux == -1) return "";

    return buffer;
}

void close_udp_socket() {
    freeaddrinfo(udp_res);
    close(udp_socket);
}

/* ################### TCP ################### */

int initialize_tcp_socket() {
    struct addrinfo hints;
    int aux;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    aux = getaddrinfo(NULL, PORT, &hints, &tcp_res);
    if (aux != 0) return -1;

    tcp_socket = socket(tcp_res->ai_family, tcp_res->ai_socktype, tcp_res->ai_protocol);
    if (tcp_socket == -1) return -1;

    aux = ::bind(tcp_socket, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (aux == -1) return -1;

    aux = listen(tcp_socket, 1); // Allow only one pending connection
    if (aux == -1) return -1;

    return 0;
}

string read_tcp_message() {
    //TODO: A função está sus, acho que não se usa recvfrom para TCP
    int aux, buffer_size = 2048;
    char buffer[buffer_size];

    tcp_addrlen = sizeof(tcp_addr);
    aux = recvfrom(tcp_socket, buffer, buffer_size, 0, (struct sockaddr*) &tcp_addr, &tcp_addrlen); //TODO: Check if this is correct 
    if (aux == -1) return "";

    return buffer;
}

void close_tcp_socket() {
    freeaddrinfo(tcp_res);
    close(tcp_socket);
}