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
  cout<<"daj imie";
  cin>>ChatClient::name;

  msgs.reserve(Protocol::DEFAULT_MSG_QUEUE_SIZE);

  showMenu();

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
   int size = ChatClient::name.size();
   char buff[1];
   buff[0]=size;
   cout<<size;
    send(clientSocket,buff,1,0);
    send(clientSocket,name.c_str(),size,0);

    isActive=true;



}

void ChatClient::showMenu() {

    int choice;
    bool exitRequested = false;

    while (!exitRequested) {
        cout << "\n================================" << endl;
        cout << "   CLIENT: " << name << " (Port: " << port << ")" << endl;
        cout << "================================" << endl;
        cout << "1. Polacz z serwerem" << endl;
        cout << "2. Napisz wiadomosc (Interaktywnie)" << endl;
        cout << "3. Wypisz sie" << endl;
        cout << "4. Czytaj wiadomosci" << endl;
        cout << "5. Zakoncz" << endl;
        cout << "--------------------------------" << endl;
        cout << "Wybor: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                // Możesz tu dodać pytanie o port serwera, jeśli nie domyślny
                setupConnection();
                serverListener = thread(&ChatClient::recieveMessage,this,clientSocket,ref(ready),ref(msgs));
                break;
            case 2:
                writeMessage();
                break;
            case 3:
                disconnect();
                break;

            case 4:
                readMessages();
                break;
            case 5:
                exitRequested = true;
                cout << "[!] Zamykanie klienta..." << endl;
                break;
            default:
                cout << "[X] Niepoprawna opcja!" << endl;
        }
    }
}
void ChatClient::writeMessage() {
    string recipient,content;

    cout<<"podaj odbiorce wpisz 0 jesli broadcast"<<endl;
    cin>>recipient;
    cout<<"podaj content";
    cin.ignore();
    getline(cin,content);
    if(recipient=="0") {
        Message msg = Message(ChatClient::name,content);
        sendMessage(msg);
    }
    else{
        Message msg = Message(ChatClient::name,recipient,content);
        sendMessage(msg);
    }



}

void ChatClient::sendMessage(Message msg) {
    string sendContent = msg.toString();
    int size = msg.getSize();
    vector<char> buff(1);
    buff[0] = size;
    if(send(clientSocket,buff.data(),sizeof(int),0)<0){
       // cout<<"blad w wysylaniu rozmiaru wiadomosci";
    }
    if(send(clientSocket,sendContent.c_str(),size,0)<0){
        cout<<"blad w wysylaniu wiadomosci";
    }


}

void ChatClient::recieveMessage(int socket,atomic<bool>& ready,vector<Message>&smgs) {
    while(isActive&&msgs.size()<Protocol::DEFAULT_MSG_QUEUE_SIZE){

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
        if(msgs.back().content=="disconnected"){
            cout<<"DISCONECTED FROM THE SERVER";
            isActive=false;
        }
        msg_semaphore.release();
        ready= true;
    }
}

ChatClient::~ChatClient() {
    disconnect();

}

void ChatClient::readMessages() {

        if(!msgs.empty() && ready){
            msg_semaphore.acquire();
            for(int i=msgs.size()-1;i>=0;i--){
                Message msg = msgs[i];


                msg.print();
                msgs.pop_back();
            }
            msg_semaphore.release();
        }
        else cout<<"brak wiadomosci";
    }






void ChatClient::disconnect() {
    Message msg("disconnect",1);
    sendMessage(msg);
    cout<<"disconnected";
    if(serverListener.joinable()) serverListener.join();
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