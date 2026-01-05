//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_CHATCLIENT_H
#define SO2_CHATCLIENT_H
#include <cstdint>
#include <string>
#import "../Common/Protocol.h"
#import "../Common/Message.h"
#import "ServerListener.h"
class ServerListener;
using namespace std;
class ChatClient {
    uint16_t port;
    uint16_t server_port;
    string IPA;
    string name;
    int clientSocket;
public:
    ChatClient(string name,uint16_t port,string IPAddress): name(name), IPA(IPAddress), port(port) {};
    void start();
    void setupConnection(uint16_t server_port = Protocol::DEFAULT_PORT);
    void writeMessage();
    void sendMessage(Message& msg);
    void recieveMessage(Message& msg);


};


#endif //SO2_CHATCLIENT_H
