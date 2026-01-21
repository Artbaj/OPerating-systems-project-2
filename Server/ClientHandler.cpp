//
// Created by Lenovo on 04.01.2026.
//
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Linkowanie biblioteki w MSVC
#else
#include <sys/socket.h>
#include <netinet/in.hg>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include "ChatServer.h"
#include "../Common/Protocol.h"

#include <thread>
#include <iostream>
#include "ClientHandler.h"

using namespace std;
void ClientHandler::handlerLoop(const int& socket,binary_semaphore& msg_semaphore,vector<Message>& msgs,atomic<bool>& is_active,atomic<bool> &ready){
    while(is_active&&msgs.size()<Protocol::DEFAULT_MSG_QUEUE_SIZE){
        int size;

        vector<char> buff(1);
        size = recv(socket,buff.data(),sizeof(int),0);

        if(size<0){
            cout<<WSAGetLastError();
        }
        size = buff[0];
        buff.clear();
        buff.resize(size);


        if(recv(socket,buff.data(),size,0)<0){
            cout<<WSAGetLastError();
        }

        string out(buff.begin(), buff.end());

        ready=false;

        msg_semaphore.acquire();

        msgs.emplace_back(out);
        msg_semaphore.release();
        ready= true;
    }

}

void ClientHandler::handleIncomingMessage(vector<Message>& msgs,atomic<bool>& is_active,binary_semaphore& msg_semaphore,ChatServer* server,atomic<bool> &ready) {
    while(is_active){

        if(ready==true){

            msg_semaphore.acquire();
            cout<<"aquired"<<endl;
            for(int i=msgs.size()-1;i>=0;i--){
               Message msg = msgs[i];
                cout<<i<<" "<<msg.content<<endl;
                if(msg.content=="disconnect") server->unregisterClient(msg.sender);
                if(msg.type==MessageType::PRIVATE) server->sendPrivate(msg);
                else if(msg.type==MessageType::GROUP) server->broadCastMsg(msg);
                msgs.pop_back();
            }
            ready=false;
            msg_semaphore.release();
            cout<<"released"<<endl;
        }

    }
}

void ClientHandler::start() {

    msgs.reserve(Protocol::DEFAULT_MSG_QUEUE_SIZE);
    is_active = true;
    readingThread = thread(&ClientHandler::handlerLoop,this,cref(clientSocket),ref(msg_semaphore),ref(msgs),ref(is_active),ref(ready));
    parsingThread = thread(&ClientHandler::handleIncomingMessage,this,ref(msgs),ref(is_active),ref(msg_semaphore),server,ref(ready));

}

void ClientHandler::sendMessage(Message msg) {

    cout<<endl<<endl<<endl;
    cout<<msg.toString()<<endl;
    cout<<endl<<endl<<endl;
    string sendContent = msg.toString();
    int scket =getClientSocket();
    string nme = getClientName();
    int size = msg.getSize();
    vector<char> buff(1);
    buff[0] = size;
    logger->Log(msg);

    send(clientSocket,buff.data(),sizeof(int),0);

    send(clientSocket,sendContent.c_str(),size,0);


}

ClientHandler::~ClientHandler() {
    is_active = false;
    Message msg("disconected",1);
    sendMessage(msg);
    readingThread.join();
    parsingThread.join();


}





