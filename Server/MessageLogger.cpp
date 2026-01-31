//
// Created by Lenovo on 04.01.2026.
//

#include "MessageLogger.h"


void MessageLogger::Log(Message& msg) {


    std::thread worker([this, msg]() {

        std::lock_guard<std::mutex> lock(logmtx);

        if (plik.is_open()) {
            plik << msg.converted << std::endl;
        }

    });

    worker.detach();

}
