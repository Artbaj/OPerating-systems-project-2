//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CLIENTHANDLER_H
#define SO2_CLIENTHANDLER_H

#include <string>
#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include "../Common/Message.h"
#include <semaphore>
#include <condition_variable>
using namespace std;
class ChatServer;

class ClientHandler {
public:

    ClientHandler(int socket,string cName,ChatServer *server): clientSocket(socket), clientName(cName), msg_semaphore(1), server(server){};

    ~ClientHandler();


    void start();


    void sendMessage(Message msg);


    string getClientName() const { return clientName; }
    int getClientSocket()const {return clientSocket;}

private:
    condition_variable empty;
    ChatServer* server;
    int clientSocket;
    string clientName;
    binary_semaphore msg_semaphore;
    vector<Message> msgs;
    atomic<bool> is_active;
    atomic<bool> ready{false};
    thread readingThread,parsingThread;
    void run();


    void handleIncomingMessage(vector<Message> &msgs, atomic<bool> &is_active, binary_semaphore &msg_semaphore,
                               ChatServer *server, atomic<bool> &ready);

    void handlerLoop(const int &socket, binary_semaphore &msg_semaphore, vector<Message> &msgs, atomic<bool> &is_active,
                     atomic<bool> &ready);
};


#endif //SO2_CLIENTHANDLER_H
