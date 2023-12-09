#include "utils.hpp"

// ###########################################################
// #                   AUXILIARY FUNCTIONS                   #
// ###########################################################

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

bool isValidFileName(const string str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' && c != '-') {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

// Function to get the last ID from the directory
int getLastAid(const string& directory) {

    printf("diretoria existe? %d\n", fs::exists(directory));
    
    int lastID = -1;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            string filename = entry.path().filename().string();
            size_t pos = filename.find('_');
            if (pos != string::npos) {
                int id = stoi(filename.substr(0, pos));
                lastID = max(lastID, id);
            }
        }
    }

    return lastID;
}


// ###########################################################
// #                 VALIDATION FUNCTIONS                    #
// ###########################################################
bool isUid(const string str) {
    if (str.length() != 6) {
        if (DEBUG) printf("uid length is not 6\n");
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) printf("uid is not numeric\n");
    return aux;
}

bool isPassword(const string str) {
    if (str.length() != 8) {
        if (DEBUG) printf("password length is not 8\n");
        return false;
    }

    bool aux = isAlphanumeric(str);
    if (!aux && DEBUG) printf("password is not alphanumeric\n");
    return aux;
}

bool isFileName(const string str) {
    if (str.length() > 24) {
        if (DEBUG) printf("filename length is greater than 24\n");
        return false;
    }

    bool aux = isValidFileName(str);
    if (!aux && DEBUG) printf("filename is not valid\n");
    return aux;
}

bool isFileSize(const string str) {
    if (str.length() > 8) {
        if (DEBUG) printf("file size length is greater than 8\n");
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) printf("file size is not numeric\n");
    return aux;
}

bool isName(const string str) {
    if (str.length() > 10) {
        if (DEBUG) printf("name length is greater than 10\n");
        return false;
    }

    bool aux = isAlphanumeric(str);
    if (!aux && DEBUG) printf("name is not alphanumeric\n");
    return aux;    
}

bool isValue(const string str) {
    if (str.length() > 6) {
        if (DEBUG) printf("value length is greater than 6\n");
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) printf("value is not numeric\n");
    return aux;
}

bool isDuration(const string str) {
    if (str.length() > 5) {
        if (DEBUG) printf("duration length is greater than 5\n");
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) printf("duration is not numeric\n");
    return aux;
}

// ###########################################################
// #                    UTILS FUNCTIONS                      #
// ###########################################################
bool passwordsMatch(const string path, const string password) {
    ifstream inputPassFile(path);
    if (!inputPassFile.is_open()) {
        // printf("error opening pass.txt\n");
        // sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);            //TODO: não devia estar aqui
        // exit(-1);
        return false;
    }

    //check if password is correct
    string correctPassword;
    inputPassFile >> correctPassword;
    inputPassFile.close();

    return password == correctPassword;
}

vector<string> split(const string str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Function to generate the next ID with three digits
string generateAid() {
    const string directory = "src/server/data/auctions/";

    int lastID = getLastAid(directory);
    int nextID = lastID + 1;
    
    ostringstream oss;
    oss << setw(3) << setfill('0') << nextID;
    return oss.str();
}

void createFile(const string path, const string content) {
    ofstream file(path);

    if (!file.is_open()) {
        printf("error creating file\n");
        exit(-1);
    }
    if (content != "") file << content;

    file.close();
}