#include "utils.hpp"

//Funções Auxiliares
bool isAlpha(const string str) {
    for (char c : str) {
        if (!isalpha(static_cast<unsigned char>(c))) {
            return false; // If any character is not a letter, return false
        }
    }
    return true; // All characters are letters
}

bool isNumeric(const string str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false; // If any character is not a digit, return false
        }
    }
    return true; // All characters are digits
}

bool isAlphanumeric(const string str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c))) {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

//Funções Principais
bool isUid(const string str) {
    if (str.length() != 6) return false;
    return isNumeric(str);
}

bool isPassword(const string str) {
    if (str.length() != 8) return false;
    return isAlphanumeric(str);
}

bool folderExists(const string folderPath, bool isUser) {
    string path = "data/";
    if (isUser) {
        path += "users/" + folderPath;
    } else {
        path += "auctions" + folderPath;
    }

    // return fs::exists(folderPath) && fs::is_directory(folderPath);
    return true;
}
