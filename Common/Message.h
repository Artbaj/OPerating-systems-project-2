//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_MESSAGE_H
#define SO2_MESSAGE_H
#include "Protocol.h"
#include <string>
using namespace std;
enum class MessageType {
    GROUP,
    PRIVATE,
    SYSTEM

};

struct Message {
    MessageType type;
    string sender,recipient,content,converted;
    int size;
    Protocol::Status status;
    Message(string content,int id):content(content),type(MessageType::SYSTEM){};
    Message(string sender,string content):sender(sender),content(content),type(MessageType::GROUP){};
    Message(string s,string r,string c): sender(s),recipient(r),content(c),type(MessageType::PRIVATE){}
    Message(string msg);
    string getSender();
    string getRecipient();
    string getContent();
    string toString();
    void print();
    MessageType getType();
    int getSize();

};


#endif //SO2_MESSAGE_H
