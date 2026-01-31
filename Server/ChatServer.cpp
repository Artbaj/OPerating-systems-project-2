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
    if (listen(ChatServer::Serversocket, SOMAXCONN)  < 0) {
        cout << "Error listening socket" <<WSAGetLastError()<< endl;
        exit(errno);
    }


    sockaddr_in client_address = {};
    auto addrlen = sizeof(sockaddr_in);
    while(true){
        int clientSocket = accept(ChatServer::Serversocket,(struct sockaddr*)&client_address,(socklen_t*)&addrlen);

        if (clientSocket < 0) {
            cout << "Error accepting connection" <<errno<< endl;
            exit(errno);
        }
        thread t(&ChatServer::handleClientHandshake, this, clientSocket);
        t.detach();


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





void ChatServer::handleClientHandshake(int clientSocket) {
    uint8_t size;
    vector<char> buff(1);


    int res = recv(clientSocket, buff.data(), sizeof(uint8_t), 0);

    if(res <= 0){

#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        return;
    }

    size = buff[0];
    buff.clear();
    buff.resize(size);

    if(recv(clientSocket, buff.data(), size, 0) <= 0){
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        return;
    }

    string name(buff.begin(), buff.end());


    cout << "connected: " << name << endl;

    if(!manager.isUsernameTaken(name)) {
        // Rejestracja właściwa
        registerClient(clientSocket, name);
    } else {
        cout << "Name taken: " << name << endl;
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
    }
}
void ChatServer::sendPrivate(Message msg) {


        ClientHandler* recipient = manager.getHandler(msg.recipient);
        if(recipient!= nullptr){

            recipient->sendMessage(msg);
        }
        else {
            Message error("Nie ma takiego uzytkownika",1);
            //recipient = manager.getHandler(msg.sender);
            //recipient->sendMessage(error);
        }



}

void ChatServer::registerClient(int clientSocket,string name) {

    auto* handler = new ClientHandler(clientSocket,name,this,this->logger);
    manager.addUser(name,handler);
}

void ChatServer::stop() {
    cout<<"stopping";
    for(auto& handler:manager.getAllHandlers()){
        manager.removeUser(handler->getClientName());
    }
    for(auto& t:workers){
        if(t.joinable()) t.join();
    }
}

void ChatServer::broadCastMsg(Message &msg) {



    for(auto& handler:manager.getAllHandlers()){
        handler->sendMessage(msg);
    }
}

void ChatServer::unregisterClient(string name) {
    manager.removeUser(name);

}

int main(){
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    MessageLogger logger("./logi.txt");
    ChatServer server = ChatServer(&logger);
    server.start();
    WSACleanup();
}