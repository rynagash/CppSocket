#include "P2PClient.h"
#include <iostream>

using namespace std;

int main() {
    P2PClient  client;
    char buf[1024];

    if (client.connect()) {
        memset(buf, 0, 1024);
        while (client.receiveSmall(buf, 1024) > 0) {
            cout << buf << endl;
            client.sendStr("chu");
            Sleep(500);
        }
    }

    return 0;
}
