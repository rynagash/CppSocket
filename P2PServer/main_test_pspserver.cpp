
#include <iostream>
#include "P2PServer.h"

using namespace std;

int main() {
    P2PServer server;
    char buf[1024];

    if (server.standby()) {
        cout << "send = [ pika ] " << endl;
        server.sendStr("pika");
        while (server.receive(buf, 1024) > 0) {
            cout << "  recv = [ " << buf << " ]" << endl;
        }
    }

    return 0;
}
