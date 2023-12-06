#include "../header/myauctions.hpp"


void myauctions() {
    string request = "LMA " + current_uid +"\n";
    string response = send_udp_message(request);
    string status = response.substr(0, 7);
    if (status == "RMA NOK") {
        cout << "user UID has no ongoing auctions" << endl;
    } else if (status == "RMA NLG") {
        cout << "user is not logged" << endl;
    } else if (status == "RMA OK ") {
        string auctions = get_auctions_bids(response);
        cout << auctions << endl;
    } 
};