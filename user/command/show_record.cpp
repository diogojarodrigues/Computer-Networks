#include "../header/show_record.hpp"

void show_record() {
    
    if (command.size()!=2) {
        cout << "show_record: format not valid!" << endl;
        return;
    }
    
    string aid = command[1];
    if(aid.length()!=3 || !isNumeric(aid)){
        cout << "show_record: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "SRC " + aid +"\n";
    string response = send_tcp_message(request, DEFAULT, nullptr); 

    printf("%s\n", response.c_str()); 
};