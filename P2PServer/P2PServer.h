#ifndef ___C_SERVER_H
#define ___C_SERVER_H

#define DEBUG
//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2

class CServer {
private:
    int srcSocket, dstSocket;
    struct sockaddr_in srcAddr, dstAddr;
    int dstAddrSize;
    int status;

    int numrcv;
    char *buffer;
    int bufferSize;
    int port;
    bool ready;

public:
    CServer(int _port = 1111, int _bs = 1024);
    ~CServer();
    bool standby();
    int receive(char* buf, int len);
    bool idle();
    void sendStr(const std::string& str);
    void sendData(char *buf, int size);
    bool isReady() { return ready; }
};
#endif
