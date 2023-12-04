#include "./utils.hpp"

vector<string> command;
string current_uid;
string current_password;

bool isNumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false; // If any character is not a digit, return false
        }
    }
    return true; // All characters are digits
}

bool isAlphanumeric(const std::string& str) {
    for (char c : str) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

string get_auctions_bids(string response){
    int k=1; // 1 and 3 in aid, 0 in status
    string final = response.substr(7, response.size()-7);
    string auctions="";
    for (char c : final) {
        if(k>=1 && k<=4){
            auctions += c;
            k++;
            continue;
        }
        else if(c=='1')
            auctions += "is active\n";
        else if(c=='0')
            auctions += "is not active\n";
        else if(c==' ')
            k=1;
        else if(c=='\n')
            continue;
        }
        
    return auctions;  
}

void print_auctions(string auctions){
    
    printf("%s\n", auctions.c_str());

    int k=0; // 1 and 3 in aid, 0 in status
    for (char c : auctions) {
        if(k>=0 && k<=3){
            if (k==0)
                cout << "AID: ";
            cout << c;
            k++;
            continue;
        }
        else if(c=='1')
            cout << "is active\n";
        else if(c=='0')
            cout << "is not active\n";
        else if(c==' ')
            k=0;
        }
}


string bid_record(string messages){
    vector<string> bid_records;
    string message;
    messages = messages.substr(7, messages.size()-7);
    for(char c:messages){
        if(c==' ' or c=='\n'){
            bid_records.push_back(message);
            message="";
        }
        else
            message += c;
    }
    string bid_record="";
    bid_record += "Host UID:" + bid_records[0] + "  Auction name:" + bid_records[1] + "  Asset filename:" + bid_records[2] + "  Start value:" + bid_records[3] + "  Start date-time:" + bid_records[4] + ' ' +bid_records[5] + "  Time active:" + bid_records[6] + "\n";
    unsigned long int i=7;
    if(i==bid_records.size())
        return bid_record;
    while (true)
    {
       if(bid_records[i]=="B"){
           bid_record += "Bidder UID:" + bid_records[i+1] + " Bid value:" + bid_records[i+2] + " Bid date-time:" + bid_records[i+3] + bid_records[i+4] + " Bid sec time:" + bid_records[i+5] + "\n";
           i+=6;
       }
       else if(bid_records[i]=="E"){
           bid_record += "End date-time:" + bid_records[i+1] + bid_records[i+2]+ " End sec time:" + bid_records[i+3] + "\n";
           break;
       }else if(i>=bid_records.size()){
           break;
       }
    }
    return bid_record;
    

}

void create_file_copy(ifstream* source_file, const string& destination_filename) {

    source_file->clear(); // Clear any error flags that might affect reading
    source_file->seekg(0, ios::beg); // Move the file pointer to the beginning of the file

    string destination = "user/images/" + destination_filename;    //TODO: CHANGE THIS WHEN SUBMITTING
    std::ofstream destFile(destination, ios::binary);
    if (!destFile.is_open()) {
        std::cerr << "Error: Unable to open destination file." << std::endl;
        return;
    }

    // Define a buffer size for reading/writing data
    constexpr size_t bufferSize = 2048;
    char buffer[bufferSize];

    while (!source_file->eof() && !source_file->fail()) {
        source_file->read(buffer, bufferSize);
        std::streamsize bytesRead = source_file->gcount();

        if (bytesRead > 0) {
            destFile.write(buffer, bytesRead);
        }

        memset(buffer, 0, bufferSize);
    }

    if (source_file->fail() && !source_file->eof()) {
        std::cerr << "Error occurred while reading the source file." << std::endl;
    }

    destFile.close();
}
