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

class Message {
public:
    MessageType type;
    string sender,recipient,content;



    Message(string s,string r,string c,MessageType t = MessageType::GROUP): type(t),sender(s),recipient(r),content(c){}

    string getSender();
    string getRecipient();
    string getContent();
};


#endif //SO2_MESSAGE_H
