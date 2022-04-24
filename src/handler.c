//
// Created by Hami Lemon on 2022/4/24.
//

#include "handler.h"
#include "logger/logger.h"

void zigbee_handler(tcp_connection_t conn, byte_t *buffer) {
    logger_debug(LOGGER("send operation:%02X to zigbee"), buffer[1]);
}

void camera_handler(tcp_connection_t conn, byte_t *buffer) {

}

void voice_handler(tcp_connection_t conn, byte_t *buffer) {

}