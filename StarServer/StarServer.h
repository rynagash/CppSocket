#ifndef ___C_SERVER_H
#define ___C_SERVER_H

#define DEBUG
//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class P2PServer {
private:
    int srcSocket, dstSocket;
    struct sockaddr_in srcAddr, dstAddr;
    int dstAddrSize;
    int status;

    int numrcv;
    char *buffer;
    int bufferSize;
    int port;
    // bool ready;

public:
    P2PServer(int _port = 1111, int _bs = 1024);
    ~P2PServer();
    bool standby(int timeout = 5);
    int receive(char* buf, int len);
    int receiveSmall(char *buf, int buffer_size);
    bool idle();
    void sendStr(const std::string& str);
    void sendData(char *buf, int size);
    // bool isReady() { return ready; }
};
#endif
