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

string bid_record(string message){
    //"RRC OK 104010 test DONT_READ_ME.txt 1 2023-11-24 20:27:39 10000 B 102484 123 2023-11-24 20:34:35 416 E 2023-11-24 23:14:19 10000"
    //host_UID auction_name asset_fname start_value start_date-time timeactive B bidder_UID bid_value bid_date-time bid_sec_time]* [ E end_date-time end_sec_time]
    message = message.substr(7, message.size()-7);
    int counter=0;
    int already_write=0;
    char type = 'C';
    string bid_record="";
    for(char c:message){
        if(type=='C'){
            if(c==' '){
                counter++;
                bid_record += "   ";
                already_write=0;
                if(counter==7){
                    bid_record += "\n";
                    type='B';
                    counter=-1;
                }
                continue;
            }
            if(counter==0){
                if(!already_write){
                    bid_record +="Host UID:";
                    already_write=1;
                }
                bid_record += c; 
            }
            if(counter==1){
                if(!already_write){
                    bid_record +="Auction name:";
                    already_write=1;
                }
                bid_record += c; 
            }
            if(counter==2){
                if(!already_write){
                    bid_record +="Asset filename:";
                    already_write=1;
                }
                bid_record += c; 
            }
            if(counter==3){
                if(!already_write){
                    bid_record +="Start value:";
                    already_write=1;
                }
                bid_record += c; 
            }
            if(counter==4){
                if(!already_write){
                    bid_record +="Start date-time:";
                    already_write=1;
                }
                bid_record += c; 
            }
            if(counter==5){
                bid_record += c; 
            }
            if (counter==6){
                if(!already_write){
                    bid_record +="Time active: ";
                    already_write=1;
                }
                bid_record += c; 
            }

        }if(type=='B'){
            if(c==' '){
                if(counter==-1){
                    counter=0;
                    continue;
                }
                counter++;
                bid_record += "   ";
                already_write=0;
                if(counter==5){
                    bid_record += "\n";
                    counter=0;
                }
                continue;
            }
            else if(counter==0 && c=='B'){
                counter=-1;
            }
            else if(counter==0 && c=='E'){
                type='E';
                counter=-1;
            }
            else if(counter==1){
                if(!already_write){
                    bid_record +="Bidder UID:";
                    already_write=1;
                }
                bid_record += c; 
            }
            else if(counter==2){
                if(!already_write){
                    bid_record +="Bid value:";
                    already_write=1;
                }
                bid_record += c; 
            }
            else if(counter==3){
                if(!already_write){
                    bid_record +="Bid date-time:";
                    already_write=1;
                }
                bid_record += c; 
            }
            else if (counter==4){
                if(!already_write){
                    bid_record +="Bid sec time:";
                    already_write=1;
                }
                bid_record += c; 
            }
        }if(type=='E'){
            if(c==' '){
                if(counter==-1){
                    counter=0;
                    continue;
                }
                counter++;
                bid_record += "   ";
                already_write=0;
                if(counter==3){
                    bid_record += "\n";
                    counter=0;
                }
                continue;
            }
            else if(counter==0){
                if(!already_write){
                    bid_record +="End date-time:";
                    already_write=1;
                }
                bid_record += c; 
            }
            else if(counter==1){
                if(!already_write){
                    bid_record +="End sec time:";
                    already_write=1;
                }
                bid_record += c; 
            }
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
