#ifndef UTILS_HPP
#define UTILS_HPP

#include "./protocol.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

using namespace std;

// const enum {
//     RECEIVE_TCP_IMAGE,
//     SEND_TCP_IMAGE,
//     DEFAULT
// };

extern vector<string> command;
extern string current_uid;
extern string current_password;

bool isNumeric(const std::string& str);
bool isAlphanumeric(const std::string& str);
std::string get_auctions_bids(std::string response);
void print_auctions(std::string auctions);
void create_file_copy(std::ifstream* source_file, const std::string& destination_filename);

#endif // UTILS_HPP
