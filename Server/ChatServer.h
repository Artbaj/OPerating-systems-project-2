//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CHATSERVER_H
#define SO2_CHATSERVER_H

#include <map>
#include <mutex>
#include "../Common/Protocol.h"
#include "../Common/Message.h"
#include "ClientHandler.h"
#include "MessageLogger.h"

using namespace std;
class ChatServer {
private:
    uint16_t port;
    string IPA;
    int Serversocket;
    sockaddr_in server_address;
    map<string,ClientHandler*> activeClients;
    mutex clientMutex;
    MessageLogger logger;
    int setupSocket();
    void acceptConnection();

public:
    ChatServer(MessageLogger log,uint16_t p = Protocol::DEFAULT_PORT):port(p),logger(log){};
    void start();
    void stop();
    void broadCastMsg(const Message& msg);
    void sendPrivate(const Message& msg);
    void registerClient(string name);
    void unregisterClient(string name);
    void printServerInfo(int serverSocket);

};


#endif //SO2_CHATSERVER_H
