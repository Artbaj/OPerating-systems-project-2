#include <iostream>
#include <ostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Linkowanie biblioteki w MSVC
#else
#include <sys/socket.h>
#include <netinet/in.hg>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "ChatClient.h"

void ChatClient::start() {
    ChatClient::clientSocket =  socket(AF_INET, SOCK_STREAM, 0);
    if (ChatClient::clientSocket < 0) {
        cout << "Error creating socket" <<errno<< endl;
        exit(errno);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(Protocol::DEFAULT_SERVER_IP);
    server_address.sin_port = htons(Protocol::DEFAULT_PORT);


    connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address));
}
