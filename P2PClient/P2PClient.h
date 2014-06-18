#ifndef ___C_CLIENT_H
#define ___C_CLIENT_H

#include <iostream>
#include <vector>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


class CClient {
private:
    WSADATA wsaData;
    struct sockaddr_in server;
    SOCKET sock;
    int dstAddrSize;
    std::string server_ip;
    int port;
public:
    CClient(const std::string ip = "10.0.1.49", const int pt = 9999);
    ~CClient();
    bool connect();
    int receive(int len, char *buf);
    int sendData(const char *buf, int size);
    int sendStr(const std::string str);
};
#endif
