//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_USERMANAGER_H
#define SO2_USERMANAGER_H


#include <string>
#include <map>
#include <mutex>
#include <vector>
#include "../Common/Message.h"

// Forward declaration
class ClientHandler;

class UserManager {
public:
    UserManager();



    bool addUser(const std::string& name, ClientHandler* handler);


    void removeUser(const std::string& name);


    ClientHandler* getHandler(const std::string& name);


    std::vector<ClientHandler*> getAllHandlers();


    bool isUsernameTaken(const std::string& name);

private:

    std::map<std::string, ClientHandler*> users;


    std::mutex usersMutex;
};


#endif //SO2_USERMANAGER_H
