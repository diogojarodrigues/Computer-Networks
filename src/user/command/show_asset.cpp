#include "../header/show_asset.hpp"

void show_asset() {
    if (command.size()!=2) {
        cout << "close: format not valid!" << endl;
        return;
    }

    string aid;
    aid = command[1];
    if(aid.length()!=3 || !isNumeric(aid)){
        cout << "close: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "SAS " + aid +"\n";

    send_tcp_message(request, RECEIVE_TCP_IMAGE, nullptr);  
};