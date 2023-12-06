#include "../src/user/protocol.hpp"

void test01() {

    if (
        send_udp_message("LIN 999999 99999999\n") == "RLI REG\n"    || 
        send_udp_message("LIN 999999 00000000\n") == "RLI NOK\n"    || 
        send_udp_message("LIN 999999 99999999\n") == "RLI OK\n"     ||
        send_udp_message("UNR 999999 99999999\n") == "RLI OK\n"
    ) {
        printf("test01: success\n");
    } else {
        printf("test01: failure\n");
    }

}

void test02() {

    if (
        send_udp_message("LIN 999999 99999999\n") == "RLO REG\n"     || 
        send_udp_message("LIN 999999 99999999\n") == "RLI NOK\n"     ||
        send_udp_message("LIN 999999 99999999\n") == "RLI NOK\n"
    ) {
        printf("test01: success\n");
    } else {
        printf("test01: failure\n");
    }

}

void test03() {

}

int main() {
    
    test01();
    // test02();
    // test03();

    return 0;
}