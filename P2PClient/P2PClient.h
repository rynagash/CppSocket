#ifndef ___C_CLIENT_H
#define ___C_CLIENT_H

#include <iostream>
#include <vector>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


class P2PClient {
private:
    WSADATA wsaData;
    struct sockaddr_in server;
    SOCKET sock;
    int dstAddrSize;
    std::string server_ip;
    int port;
public:
    P2PClient(const std::string ip = "127.0.0.1", const int pt = 1111);
    ~P2PClient();
    bool connect();
    int receive(int len, char *buf);
    int sendData(const char *buf, int size);
    int sendStr(const std::string str);
};
#endif
