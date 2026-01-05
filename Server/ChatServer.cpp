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
#include "ChatServer.h"
#include "../Common/Protocol.h"
void ChatServer::start() {
    ChatServer::Serversocket  = socket(AF_INET, SOCK_STREAM, 0);
    if (ChatServer::Serversocket < 0) {
        cout << "Error creating socket" <<errno<< endl;
        exit(errno);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ChatServer::IPA.c_str());
    server_address.sin_port = htons(ChatServer::port);
    if (bind(ChatServer::Serversocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Error binding socket" <<errno<< endl;
        exit(errno);
    }
    if (listen(ChatServer::Serversocket, Protocol::DEFAULT_QUEUE_SIZE) < 0) {
        cout << "Error listening socket" <<errno<< endl;
        exit(errno);
    }
    auto addrlen = sizeof(sockaddr);
    int connection = accept(ChatServer::Serversocket,(struct sockaddr*)&server_address,(socklen_t*)&addrlen);
    if (connection < 0) {
        cout << "Error accepting connection" <<errno<< endl;
        exit(errno);
    }
    cout<<"connected"<<connection<<endl;

}
