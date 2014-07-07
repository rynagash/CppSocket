
#include <iostream>
#include "StarServer.h"

using namespace std;

int main() {
    P2PServer server;
    char buf[1024];

    if (server.standby()) {
        while (true) {
            cout << "send = [ pika ] " << endl;
            server.sendStr("pika");
            memset(buf, 0, 1024);
            if (server.receiveSmall(buf, 1024) > 0) {
                cout << "  recv = [ " << buf << " ]" << endl;
            } else {
                break;
            }
        }
    }

    return 0;
}
