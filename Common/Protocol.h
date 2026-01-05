//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_PROTOCOL_H
#define SO2_PROTOCOL_H

#include <cstdint>

namespace Protocol {

    const uint16_t DEFAULT_PORT = 8888;
    const char* const DEFAULT_SERVER_IP = "127.0.0.1";


    const size_t MAX_NAME_LEN = 32;
    const size_t MAX_CONTENT_LEN = 512;


    enum class Status : uint8_t {
        SUCCESS = 0,
        ERR_NAME_TAKEN = 1,
        ERR_UNKNOWN_RECIPIENT = 2,
        ERR_SERVER_FULL = 3
    };


    const char CMD_SEPARATOR = ':';
    const char* const CMD_EXIT = "/quit";
}
#endif //SO2_PROTOCOL_H
