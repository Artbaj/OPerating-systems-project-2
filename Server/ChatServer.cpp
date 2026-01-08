#include <iostream>
#include <ostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib") // Linkowanie biblioteki w MSVC
#else
#include <sys/socket.h>
#include <netinet/in.hg>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "ChatServer.h"
#include "../Common/Protocol.h"


int ChatServer::setupSocket() {
    ChatServer::Serversocket  = socket(AF_INET, SOCK_STREAM, 0);
    if (ChatServer::Serversocket < 0) {
        cout << "Error creating socket" <<errno<< endl;
        return -1;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(ChatServer::port);
    if (bind(ChatServer::Serversocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cout << "Error binding socket! Windows Error Code: " <<WSAGetLastError() << endl;
        return -1;
    }
    return 0;
}

void ChatServer::start() {
    if(ChatServer::setupSocket()) exit(WSAGetLastError());
    printServerInfo();
    if (listen(ChatServer::Serversocket, Protocol::DEFAULT_QUEUE_SIZE) < 0) {
        cout << "Error listening socket" <<WSAGetLastError()<< endl;
        exit(errno);
    }

    cout<<"listening"<<endl;
    sockaddr_in client_address = {};
    auto addrlen = sizeof(sockaddr_in);
    while(true){
        int clientSocket = accept(ChatServer::Serversocket,(struct sockaddr*)&client_address,(socklen_t*)&addrlen);

        if (clientSocket < 0) {
            cout << "Error accepting connection" <<errno<< endl;
            exit(errno);
        }

       if(acceptConnection(clientSocket)) cout<<"imie zajete";

    }



}

void ChatServer::printServerInfo() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct addrinfo hints = {}, *res;
        hints.ai_family = AF_INET; // tylko IPv4

        if (getaddrinfo(hostname, nullptr, &hints, &res) == 0) {
            std::cout << "dostepne adresy" << std::endl;
            for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
                sockaddr_in* ipv4 = (sockaddr_in*)p->ai_addr;
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
                std::cout << " -> " << ip << std::endl;
            }
            freeaddrinfo(res);
        }
    }
}

int ChatServer::acceptConnection(int clientSocket) {
    char msg[5];


    memset(msg,0,4);
    if(recv(clientSocket,msg,4,0)<0){
        cout<<WSAGetLastError();
    }
    for(auto c :msg)cout<<c;
    msg[5] = '\0';
    if(!manager.isUsernameTaken(msg)) {

        workers.emplace_back(&ChatServer::registerClient,this,clientSocket,msg);


        return 0;
    }
    else return 1;

}

void ChatServer::sendPrivate(Message msg) {
    thread sender([this,msg]() {
        cout<<msg.recipient;
        ClientHandler* recipient = manager.getHandler(msg.recipient);

        recipient->sendMessage(msg);
    });
    sender.join();


}

void ChatServer::registerClient(int clientSocket,string name) {

    auto* handler = new ClientHandler(clientSocket,name,this);
    manager.addUser(name,handler);
}

void ChatServer::stop() {
    for(auto& t:workers){
        if(t.joinable()) t.join();
    }
}

int main(){
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    MessageLogger logger = MessageLogger("./");
    ChatServer server = ChatServer(logger);
    server.start();
    WSACleanup();
}