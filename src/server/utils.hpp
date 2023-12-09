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
bool isPassword(const string str);
bool isFileName(const string str);
bool isFileSize(const string str);
bool isName(const string str);
bool isValue(const string str);
bool isDuration(const string str);

// Utils functions
bool passwordsMatch(const string path, const string password);
vector<string> split(const string str);
string generateAid();
void createFile(const string path, const string content);

#endif // UTILS_SERVER_HPP