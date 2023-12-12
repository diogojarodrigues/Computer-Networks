#include "../header/mybids.hpp"


void mybids() {
    string request = "LMB " + current_uid +"\n";
    string response = send_udp_request(request);
    string status = response.substr(0, 7);

    if (status == "RMB NOK") {
        cout << "user UID has no ongoing bids" << endl;
    } else if (status == "RMB NLG") {
        cout << "user is not logged" << endl;
    } else if (status == "RMB OK ") {
        string bids= get_auctions_bids(response);   
        cout << bids;
        return;
    } 
};