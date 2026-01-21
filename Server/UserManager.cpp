//
// Created by Lenovo on 04.01.2026.
//

#include <iostream>
#include "UserManager.h"
#include "ClientHandler.h"
using namespace std;
void UserManager::addUser(const string &name, ClientHandler* handler) {
    usersMutex.lock();
    activeClients[name] =handler;

    activeClients[name]->start();
    usersMutex.unlock();
}

bool UserManager::isUsernameTaken(const string &name) {
    return activeClients.contains(name);
}

ClientHandler *UserManager::getHandler(const string &name) {
    std::lock_guard<std::mutex> lock(usersMutex); // Blokada przy odczycie
    auto it = activeClients.find(name);

    if (it != activeClients.end()) {

        return it->second;
    }

    return nullptr;

}

std::vector<ClientHandler *> UserManager::getAllHandlers() {
    vector<ClientHandler*> out;
    for(auto& [name,clientHandler]:activeClients){
        out.push_back(clientHandler);
    }
    return out;
}

void UserManager::removeUser( string name) {
    auto it = activeClients.find(name);

    if (it != activeClients.end()) {
        activeClients.erase(name);

        delete it->second;
    }
    else{
        cout<<"nie ma takiego uzytkownika"<<endl;
    }

}
