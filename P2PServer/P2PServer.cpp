#include "P2PServer.h"

P2PServer::P2PServer(int _port, int _bs) {
    ready = false;
    port = _port;
    bufferSize = _bs;
    buffer = new char[bufferSize];

    dstAddrSize = sizeof( dstAddr );

    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);

    memset(&srcAddr, 0, sizeof( srcAddr ));
    srcAddr.sin_port = htons(port);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //create a socket
    srcSocket = socket(AF_INET, SOCK_STREAM, 0);
    int ret;
    bool flag = true;
    ret = setsockopt(srcSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof( flag ));
    if (ret != 0) {
        printf("Couldn't setsockopt(TCP_NODELAY)\n");
        exit(-1);
    }
    std::cout << "server : " << port << ", " << srcSocket << std::endl;

    ::bind(srcSocket, ( struct sockaddr * ) &srcAddr, sizeof( srcAddr ));
    listen(srcSocket, 1);
}
P2PServer::~P2PServer() {
    WSACleanup();
    delete[] buffer;
    std::cout << "destuctor : CServer" << std::endl;
}
bool P2PServer::standby() {
    timeval tv = { 1, 0 };
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(srcSocket, &mask);
    //int rc = select((int)srcSocket + 1, &mask, NULL, NULL, &tv);
    int rc = select(0, &mask, NULL, NULL, &tv);
    if (FD_ISSET(srcSocket, &mask)) {
        dstSocket = accept(srcSocket, ( struct sockaddr * ) &dstAddr, &dstAddrSize);
    } else {
        std::cout << "timeout" << std::endl;
        return false;
    }

    //dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
    if (dstSocket == INVALID_SOCKET) {
        std::cout << "accept error!" << std::endl;
        return false;
    }
    int ret;
    bool flag = true;
    ret = setsockopt(dstSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof( flag ));
    if (ret == -1) {
        std::cerr << "Couldn't setsockopt(TCP_NODELAY)" << std::endl;
        exit(-1);
    }

    ready = true;

    return true;
}
int P2PServer::receive(char* buf, int len) {
    int revd_size;
    int tmp;
    revd_size = 0;
    while (revd_size < len) {
        tmp = recv(dstSocket, buf + revd_size, len - revd_size, 0);
        if (tmp == SOCKET_ERROR) { /* エラーが発生 */
            // len=revd_size;
            return SOCKET_ERROR;
        } else if (tmp == 0) { /* ソケットが切断された */
            // len=revd_size;
            return 0;
        } else revd_size += tmp;
    }
    // *len=revd_size;
    return revd_size;
}
bool P2PServer::idle() {
    memset(buffer, '\0', bufferSize);
    numrcv = recv(dstSocket, buffer, bufferSize, 0);

    if (numrcv == 0) {
        std::cout << "ソケットが切断されました" << std::endl;
        return false;
    } else if (numrcv == SOCKET_ERROR) {
        std::cout << "受信エラー" << std::endl;
        std::cout << "エラー" << WSAGetLastError() << "が発生しました" << std::endl;
        return false;
        // standby();
    } else {
        std::stringstream ss;
        ss << buffer;
        // std::cout << "recv data = [ " << buffer << " ]" << std::endl;
        std::string str;
        ss >> str;
        if (str == "ok") {
            ready = true;
        } else if (str == "99") {
            std::cout << "Client shutdonw" << std::endl;
            ready = false;
            return false;
        }
    }

    return true;
}

void P2PServer::sendStr(const std::string& str) {
    ready = false;
    send(dstSocket, str.c_str(), str.length(), 0);
}
void P2PServer::sendData(char *buf, int size) {
    ready = false;
    send(dstSocket, buf, size, 0);
}
#include "P2PServer.h"

P2PServer::CServer(int _port, int _bs) {
    ready = false;
    port = _port;
    bufferSize = _bs;
    buffer = new char[bufferSize];

    dstAddrSize = sizeof( dstAddr );

    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);

    memset(&srcAddr, 0, sizeof( srcAddr ));
    srcAddr.sin_port = htons(port);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //create a socket
    srcSocket = socket(AF_INET, SOCK_STREAM, 0);
    int ret;
    bool flag = true;
    ret = setsockopt(srcSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof( flag ));
    if (ret != 0) {
        printf("Couldn't setsockopt(TCP_NODELAY)\n");
        exit(-1);
    }
    std::cout << "server : " << port << ", " << srcSocket << std::endl;

    ::bind(srcSocket, ( struct sockaddr * ) &srcAddr, sizeof( srcAddr ));
    listen(srcSocket, 1);
}
P2PServer::~CServer() {
    WSACleanup();
    delete[] buffer;
    std::cout << "destuctor : CServer" << std::endl;
}
bool P2PServer::standby() {
    timeval tv = { 1, 0 };
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(srcSocket, &mask);
    //int rc = select((int)srcSocket + 1, &mask, NULL, NULL, &tv);
    int rc = select(0, &mask, NULL, NULL, &tv);
    if (FD_ISSET(srcSocket, &mask)) {
        dstSocket = accept(srcSocket, ( struct sockaddr * ) &dstAddr, &dstAddrSize);
    } else {
        std::cout << "timeout" << std::endl;
        return false;
    }

    //dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
    if (dstSocket == INVALID_SOCKET) {
        std::cout << "accept error!" << std::endl;
        return false;
    }
    int ret;
    bool flag = true;
    ret = setsockopt(dstSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof( flag ));
    if (ret == -1) {
        std::cerr << "Couldn't setsockopt(TCP_NODELAY)" << std::endl;
        exit(-1);
    }

    ready = true;

    return true;
}
int P2PServer::receive(char* buf, int len) {
    int revd_size;
    int tmp;
    revd_size = 0;
    while (revd_size < len) {
        tmp = recv(dstSocket, buf + revd_size, len - revd_size, 0);
        if (tmp == SOCKET_ERROR) { /* エラーが発生 */
            // len=revd_size;
            return SOCKET_ERROR;
        } else if (tmp == 0) { /* ソケットが切断された */
            // len=revd_size;
            return 0;
        } else revd_size += tmp;
    }
    // *len=revd_size;
    return revd_size;
}
bool P2PServer::idle() {
    memset(buffer, '\0', bufferSize);
    numrcv = recv(dstSocket, buffer, bufferSize, 0);

    if (numrcv == 0) {
        std::cout << "ソケットが切断されました" << std::endl;
        return false;
    } else if (numrcv == SOCKET_ERROR) {
        std::cout << "受信エラー" << std::endl;
        std::cout << "エラー" << WSAGetLastError() << "が発生しました" << std::endl;
        return false;
        // standby();
    } else {
        std::stringstream ss;
        ss << buffer;
        // std::cout << "recv data = [ " << buffer << " ]" << std::endl;
        std::string str;
        ss >> str;
        if (str == "ok") {
            ready = true;
        } else if (str == "99") {
            std::cout << "Client shutdonw" << std::endl;
            ready = false;
            return false;
        }
    }

    return true;
}

void P2PServer::sendStr(const std::string& str) {
    ready = false;
    send(dstSocket, str.c_str(), str.length(), 0);
}
void P2PServer::sendData(char *buf, int size) {
    ready = false;
    send(dstSocket, buf, size, 0);
}
