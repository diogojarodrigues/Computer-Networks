#include "../src/user/protocol.hpp"

#define USER " 999999 99999999\n"
#define USER_WRONG_PASSWORD " 999999 00000000\n"

bool test(string opcode, string user, string expected_awnser) {
    string request = opcode + user;

    if (send_udp_message(request) == expected_awnser) {
        return true;
    } else {
        return false;
    }
    printf("Fim\n");
}

void test01() {

    if(!test("LIN", USER, "RLI REG\n")) {
        printf("test01: error register\n");
        return; 
    }

    if (!test("LIN", USER_WRONG_PASSWORD, "RLI NOK\n")) {
        printf("test01: error wrong user\n");
        return;
    }

    if (!test("LIN", USER, "RLI OK\n")) {
        printf("test01: error login\n");
        return;
    }

    if (!test("UNR", USER, "RLO OK\n")) {
        printf("test01: error unregister\n");
        return;
    }
    printf("test01: success\n");
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

    //TODO: não está a funcionar
    
    printf("Running tests...\n");
    test01();
    // test02();
    // test03();
    printf("Tests finished\n");

    return 0;
}