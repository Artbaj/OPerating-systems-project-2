cmake_minimum_required(VERSION 3.26)
project(SO2)

set(CMAKE_CXX_STANDARD 23)

add_executable(SO2 main.cpp
        Common/Message.cpp
        Common/Message.h
        Common/Protocol.h
        Server/ChatServer.cpp
        Server/ChatServer.h
        Server/ClientHandler.cpp
        Server/ClientHandler.h
        Server/MessageLogger.cpp
        Server/MessageLogger.h
        Server/UserManager.cpp
        Server/UserManager.h
        Client/ChatClient.cpp
        Client/ChatClient.h
        Client/ServerListener.cpp
        Client/ServerListener.h
        Server/string.cpp
        Server/string.h)
