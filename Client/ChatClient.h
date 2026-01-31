//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CHATCLIENT_H
#define SO2_CHATCLIENT_H
#include <cstdint>
#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include "../Common/Protocol.h"
#include "../Common/Message.h"


class ServerListener;

using namespace std;
class ChatClient {
    uint16_t port;
    uint16_t server_port;
    string name;
    string IPA;
    atomic<bool> ready = false;
    atomic<bool> isActive;
    thread serverListener;
    vector<Message> msgs;
    int clientSocket;
    binary_semaphore msg_semaphore;
public:
    ChatClient(string name,uint16_t port): name(name), port(port), msg_semaphore(1) {};
    ~ChatClient();
    void start();
    void setupConnection(uint16_t server_port = Protocol::DEFAULT_PORT);
    void showMenu();
    void writeMessage();
    void sendMessage(Message msg);
    void recieveMessage(int port,atomic<bool>& reday,vector<Message>&smgs);
    void readMessages();
    void disconnect();

    void startSpamming();
};


#endif //SO2_CHATCLIENT_H
