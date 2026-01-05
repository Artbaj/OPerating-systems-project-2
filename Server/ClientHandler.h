//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CLIENTHANDLER_H
#define SO2_CLIENTHANDLER_H

#include <string>
#include <atomic>
#include <vector>
#include "../Common/Message.h"

using namespace std;
class ChatServer;

class ClientHandler {
public:

    ClientHandler(int socket, ChatServer* server,string cName):clientSocket(socket),server(server),clientName(cName){};



    void start();


    void sendMessage(const Message& msg);


    string getClientName() const { return clientName; }

private:
    int clientSocket;
    ChatServer* server;
    string clientName;
    atomic<bool> connected;


    void run();


    void handleIncomingMessage(const Message& msg);
};


#endif //SO2_CLIENTHANDLER_H
