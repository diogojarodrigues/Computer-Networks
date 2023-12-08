#ifndef UTILS_SERVER_HPP
#define UTILS_SERVER_HPP

#include "./protocol.hpp"
#include "./commands.hpp"

#include <filesystem>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

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

namespace fs = std::filesystem;

bool isUid(const string str);
bool isPassword(const string str);
bool passwordsMatch(const string path, const string password);

#endif // UTILS_SERVER_HPP