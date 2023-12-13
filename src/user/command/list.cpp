#include "../header/list.hpp"


void list() {
    string request = "LST\n";
    string response = send_udp_request(request);
    string r = response.substr(0, 7);
    if (r == "RLS NOK") {
        cout << "no auction was yet started" << endl;
        return;
    } else if (r == "RLS OK ") {
        string auctions= get_auctions_bids(response);   
        cout << auctions ;
        return;
    } else 
        cout << "unknown error" << endl;

};