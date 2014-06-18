#include "P2PClient.h"

P2PClient::P2PClient(const std::string ip, const int pt) {
    server_ip = ip;
    port = pt;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "\nsocket : " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str());

    if (server.sin_addr.S_un.S_addr == 0xffffffff) {
        struct hostent *host;

        host = gethostbyname(server_ip.c_str());
        if (host == NULL) {
            exit(EXIT_FAILURE);
        }
        server.sin_addr.S_un.S_addr = *(unsigned int *) host->h_addr_list[0];
    }
}

P2PClient::~P2PClient() {
    // winsock2の終了処理
    closesocket(sock);
    WSACleanup();
}

bool P2PClient::connect() {
    std::cout << "connect to " << server_ip.c_str();
    int ret = ::connect(sock, ( struct sockaddr * )&server, sizeof( server ));
    if (ret == SOCKET_ERROR) {
        std::cerr << "\nconnect : " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << " ... complete!" << std::endl;
    return true;
}


int P2PClient::receive(char *buf, int len) {
    // memset(&buffer2[0], '\0', len);
    int sum = 0;
    while (sum < len) {
        int ret = recv(sock, &buf[sum], len - sum, 0);
        if (ret == SOCKET_ERROR) { /* エラーが発生 */
            // len=revd_size;
            return SOCKET_ERROR;
        } else if (ret == 0) { /* ソケットが切断された */
            // len=revd_size;
            return 0;
        } else {
            sum += ret;
            //std::cout << " " << sum << "/" << len;
        }
    }
    return sum;
}

int P2PClient::sendData(const char *buf, int size) {
    return ::send(sock, buf, size, 0);
}

int P2PClient::sendStr(const std::string str) {
    return ::send(sock, str.c_str(), str.length(), 0);
}
