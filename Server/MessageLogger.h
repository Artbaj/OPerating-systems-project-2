//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_MESSAGELOGGER_H
#define SO2_MESSAGELOGGER_H

#include <string>
#include "../Common/Message.h"
using namespace std;
class MessageLogger {
private:
    string path;
public:
    MessageLogger(const string &p) : path(p) {}
    void Log(Message& msg);
};


#endif //SO2_MESSAGELOGGER_H
