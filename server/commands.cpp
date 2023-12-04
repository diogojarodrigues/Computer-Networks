#include "./utils.hpp"

void login() {

    sendto(udp_socket, "RLI OK\n", 8, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
}

void logout(){};
void unregister(){};
void my_auctions(){};
void my_bids(){};
void list(){};
void show_record(){};


// ################ TCP COMMANDS ################
void openn(){};
void closee(){};
void show_asset(){};
void bid(){};