#ifndef UTILS_SERVER_HPP
#define UTILS_SERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEBUG 1
#define BUFFER_SIZE 2048

using namespace std;
namespace fs = std::filesystem;

// Validations functions
bool isUid(const string str);
bool isAid(const string str);
bool isPassword(const string str);
bool isFileName(const string str);
bool isFileSize(const string str);
bool isName(const string str);
bool isValue(const string str);
bool isDuration(const string str);

// Utils functions
bool user_exists(const string uid);
bool user_loggged_in(const string uid);
bool user_registered(const string uid);
bool passwordsMatch(const string uid, const string password);
bool auction_exists(const string aid);
bool auction_closed(const string aid);
string getAuctionOwner(const string aid);

vector<string> split(const string str);
string start_datetime(time_t timestamp);
void createFile(const string path, const string content);

string generateAid();
void saveImage(int socket, const string file, int size);
void sendImage(int sockett, const string aid);


#endif // UTILS_SERVER_HPP