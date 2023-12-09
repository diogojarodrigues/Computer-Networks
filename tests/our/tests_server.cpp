#include "../../src/user/protocol.hpp"

#define USER " 999999 99999999\n"
#define USER_WRONG_PASSWORD " 999999 00000000\n"

bool test(string opcode, string user, string expected_awnser) {
    string request = opcode + user;

    if (send_udp_request(request) == expected_awnser) {
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
        send_udp_request("LIN 999999 99999999\n") == "RLO REG\n"     || 
        send_udp_request("LIN 999999 99999999\n") == "RLI NOK\n"     ||
        send_udp_request("LIN 999999 99999999\n") == "RLI NOK\n"
    ) {
        printf("test01: success\n");
    } else {
        printf("test01: failure\n");
    }

}

void test03() {

    string r1 = send_udp_request("LIN 123489 12341234\n");
    cout << r1 << endl;

    string r2 = send_tcp_request("OPA 5 5 5 5");
    cout << r2 << endl;

}

int main() {

    //TODO: não está a funcionar
    
    printf("Running tests...\n");
    // test01();
    // test02();
    test03();
    printf("Tests finished\n");

    return 0;
}