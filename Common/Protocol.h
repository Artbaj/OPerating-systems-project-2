//
// Created by Lenovo on 04.01.2026.
//

#ifndef SO2_PROTOCOL_H
#define SO2_PROTOCOL_H

#include <cstdint>
#include <source_location>
#include <iostream>
namespace Protocol {

    const uint16_t DEFAULT_PORT =  8080;
    const int  DEFAULT_QUEUE_SIZE = 20000;
    const int DEFAULT_MSG_QUEUE_SIZE = 4000;
    const size_t MAX_NAME_LEN = 32;
    const size_t MAX_CONTENT_LEN = 512;


    enum class Status : uint8_t {
        SUCCESS = 0,
        ERR_NAME_TAKEN = 1,
        ERR_UNKNOWN_RECIPIENT = 2,
        disconnect
    };


}
#endif //SO2_PROTOCOL_H
