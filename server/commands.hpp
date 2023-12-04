#ifndef COMMANDS_SERVER_HPP
#define COMMANDS_SERVER_HPP

// ################ UDP COMMANDS ################
void login();
void logout();
void unregister();
void my_auctions();
void my_bids();
void list();
void show_record();


// ################ TCP COMMANDS ################
void openn();
void closee();
void show_asset();
void bid();


#endif // COMMANDS_SERVER_HPP