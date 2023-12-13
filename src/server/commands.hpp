#ifndef COMMANDS_SERVER_HPP
#define COMMANDS_SERVER_HPP

extern bool debug;

// ################ UDP COMMANDS ################
void login(string request);
void logout(string request, bool unregister = false);
void my_auctions(string request);
void my_bids(string request);
void list(string request);
void show_record(string request);

// ################ TCP COMMANDS ################
void openn(string request);
void closee(string request);
void show_asset(string request);
void bid(string request);


#endif // COMMANDS_SERVER_HPP