#include <iostream>
#include <ostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Linkowanie biblioteki w MSVC
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "ChatClient.h"

void ChatClient::start() {
  cout<<"daj adres";
  cin>>ChatClient::IPA;
    setupConnection();
}

void ChatClient::setupConnection(uint16_t server_port) {
    ChatClient::clientSocket =  socket(AF_INET, SOCK_STREAM, 0);
    if (ChatClient::clientSocket < 0) {

        cout << "Error creating socket " <<errno<< endl;
        exit(errno);
    }

    sockaddr_in server_address;
    int result =  inet_pton(AF_INET, IPA.c_str(), &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Protocol::DEFAULT_PORT);
    if(result<=0){
        exit(2115);
    }


   cout<<connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address));
    int msglen = sizeof(ChatClient::name);
    if(send(clientSocket,name.c_str(),Protocol::MAX_NAME_LEN,0)<0){
        cout<<"blad w wysylaniu";
    }
}


int main(){
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    ChatClient client = ChatClient("abba",4000);
    client.start();

    WSACleanup();
}