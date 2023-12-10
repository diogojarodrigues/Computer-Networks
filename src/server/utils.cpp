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
    for (int i = 0; i < 1000; i++) {
        ostringstream name;
        name << setfill('0') << setw(3) << i;
        string filename = name.str();

        if (!fs::exists(directory + "/" + filename)) {
            return i;
        }
    }
    return -1;
}


// ###########################################################
// #                 VALIDATION FUNCTIONS                    #
// ###########################################################
bool isUid(const string str) {
    if (str.length() != 6) {
        if (DEBUG) cout << "uid length is not 6\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "uid is not numeric\n";
    return aux;
}

bool isAid(const string str) {
    if (str.length() != 3) {
        if (DEBUG) cout << "aid length is not 3\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "aid is not numeric\n";
    return aux;
}

bool isPassword(const string str) {
    if (str.length() != 8) {
        if (DEBUG) cout << "password length is not 8\n";
        return false;
    }

    bool aux = isAlphanumeric(str);
    if (!aux && DEBUG) cout << "password is not alphanumeric\n";
    return aux;
}

bool isFileName(const string str) {
    if (str.length() > 24) {
        if (DEBUG) cout << "filename length is greater than 24\n";
        return false;
    }

    bool aux = isValidFileName(str);
    if (!aux && DEBUG) cout << "filename is not valid\n";
    return aux;
}

bool isFileSize(const string str) {
    if (str.length() > 8) {
        if (DEBUG) cout << "file size length is greater than 8\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "file size is not numeric\n";
    return aux;
}

bool isName(const string str) {
    if (str.length() > 10) {
        if (DEBUG) cout << "name length is greater than 10\n";
        return false;
    }

    bool aux = isAlphanumeric(str);
    if (!aux && DEBUG) cout << "name is not alphanumeric\n";
    return aux;    
}

bool isValue(const string str) {
    if (str.length() > 6) {
        if (DEBUG) cout << "value length is greater than 6\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "value is not numeric\n";
    return aux;
}

bool isDuration(const string str) {
    if (str.length() > 5) {
        if (DEBUG) cout << "duration length is greater than 5\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "duration is not numeric\n";
    return aux;
}

// ###########################################################
// #                    UTILS FUNCTIONS                      #
// ###########################################################
bool user_exists(const string uid) {
    const string directory = "src/server/data/users/" + uid + "/";
    return fs::exists(directory);
}

bool user_loggged_in(const string uid) {
    const string path = "src/server/data/users/" + uid + "/login.txt";
    return fs::exists(path);
}

bool user_registered(const string uid) {
    const string path = "src/server/data/users/" + uid + "/pass.txt";
    return fs::exists(path);
}

bool passwordsMatch(const string uid, const string password) {

    const string path = "src/server/data/users/" + uid + "/pass.txt";

    ifstream inputPassFile(path);
    if (!inputPassFile.is_open()) {
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

    int id = getLastAid(directory);
    if (id == 999) return "ERR";
    
    ostringstream oss;
    oss << setw(3) << setfill('0') << id;
    return oss.str();
}

void createFile(const string path, const string content) {
    ofstream file(path);

    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }
    if (content != "") file << content;

    file.close();
}

void saveImage(int socket, const string file, int size) {
    char buffer[BUFFER_SIZE] = "\0";
    int bytes_read = 0;
    int count = 0;

    ofstream assetFile(file, ios::binary);
    if (!assetFile.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }

    while ((bytes_read = read(socket, buffer, sizeof(buffer))) > 0) {

        if (buffer[bytes_read - 1] == '\n') {
            bytes_read--;
            count += bytes_read;
            assetFile.write(buffer, bytes_read);
            break;
        }

        count += bytes_read;
        assetFile.write(buffer, bytes_read);
        memset(buffer, 0, BUFFER_SIZE);
    }

    assetFile.close();

    if (count != size && DEBUG) cout << "saveImage: error saving image\n";
}


string start_datetime(time_t timestamp) {
    struct tm *timeinfo;
    char buffer[20]; // Sufficiently large buffer to hold the formatted date and time

    timeinfo = localtime(&timestamp);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    return string(buffer);
}

void sendImage(int sockett, const string aid) {
    string path = "src/server/data/auctions/" + aid + "/asset/";

    // Check if the directory exists
    if (!fs::exists(path) || !fs::is_directory(path)) {
        cerr << "Directory does not exist or is not a directory" << endl;
        exit(-1);
    }

    // Get the first file in the directory
    fs::directory_iterator itr(path);
    if (itr == fs::end(itr)) {
        cerr << "No files found in the directory" << endl;
        exit(-1);
    }

    string filePath = itr->path().string(); // Get the file path

    ifstream assetFile(filePath, ios::binary);
    if (!assetFile.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }

    string filename = fs::path(filePath).filename().string();
    string filesize = to_string(fs::file_size(filePath));

    // Print file name and file size
    write(sockett, filename.c_str(), filename.length());
    write(sockett, " ", 1);
    write(sockett, filesize.c_str(), filesize.length());
    write(sockett, " ", 1);

    char buffer[BUFFER_SIZE];
    int bytes_read = 0;

    while (!assetFile.eof()) {
        bytes_read = assetFile.read(buffer, sizeof(buffer)).gcount();
        write(sockett, buffer, bytes_read);
        memset(buffer, 0, sizeof(buffer));
    }

    write(sockett, "\n", 1);

    assetFile.close();
}