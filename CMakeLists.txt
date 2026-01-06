cmake_minimum_required(VERSION 3.26)
project(SO2)

set(CMAKE_CXX_STANDARD 23)
add_custom_target(SO2 DEPENDS ChatServer ChatClient)
# 1. Definiujemy pliki wspólne (protokół, wiadomości itp.)
set(COMMON_SOURCES
        Common/Message.cpp
        Common/Message.h
        Common/Protocol.h
)

# 2. Definiujemy plik wykonywalny SERWERA
add_executable(ChatServer
        ${COMMON_SOURCES}
        Server/ChatServer.cpp
        Server/ChatServer.h
        Server/ClientHandler.cpp
        Server/ClientHandler.h
        Server/MessageLogger.cpp
        Server/MessageLogger.h
        Server/UserManager.cpp
        Server/UserManager.h
        # Jeśli main serwera jest w osobnym pliku, dodaj go tutaj.
        # Jeśli jest w ChatServer.cpp, to powyższe wystarczy.
)

# 3. Definiujemy plik wykonywalny KLIENTA
add_executable(ChatClient
        ${COMMON_SOURCES}
        Client/ChatClient.cpp
        Client/ChatClient.h
        Client/ServerListener.cpp
        Client/ServerListener.h
        # Jeśli main klienta jest w osobnym pliku, dodaj go tutaj.
)

# 4. Linkowanie bibliotek dla obu programów
if(WIN32)
    target_link_libraries(ChatServer ws2_32)
    target_link_libraries(ChatClient ws2_32)
else()
    # Na Linuxie czasem potrzebujesz biblioteki pthread
    target_link_libraries(ChatServer pthread)
    target_link_libraries(ChatClient pthread)
endif()