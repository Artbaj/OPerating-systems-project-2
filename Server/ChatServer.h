//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CHATSERVER_H
#define SO2_CHATSERVER_H

#include <map>
#include <mutex>
#include <memory>
#include <list>
#include <thread>
#include "../Common/Protocol.h"
#include "../Common/Message.h"
#include "ClientHandler.h"
#include "MessageLogger.h"
#include "UserManager.h"

using namespace std;
class ChatServer {
private:
    uint16_t port;
    string IPA;
    int Serversocket;
    sockaddr_in server_address;
    UserManager manager;
    MessageLogger logger;
    int setupSocket();
    int acceptConnection(int clientSocket);
    list<thread> workers;

public:
    ChatServer(MessageLogger log,uint16_t p = Protocol::DEFAULT_PORT):manager(),port(p),logger(log){};
    void start();
    void stop();
    void broadCastMsg( Message& msg);
    void sendPrivate(Message msg);
    void registerClient(int clientSocket,string name);
    void unregisterClient(string name);
    void printServerInfo();

};


#endif //SO2_CHATSERVER_H
