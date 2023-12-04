#ifndef UTILS_SERVER_HPP
#define UTILS_SERVER_HPP

#include "./protocol.hpp"
#include "./commands.hpp"

#include <filesystem> // Include the header for filesystem operations
#include <string>
#include <cstdio>

#include <iostream>
#include <filesystem>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

bool isUid(const string str);
bool isPassword(const string str);
bool folderExists(const string folderPath, bool isUser);

#endif // UTILS_SERVER_HPP