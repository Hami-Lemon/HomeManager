//
// Created by Hami Lemon on 2022/4/24.
//

#ifndef MANAGER_HANDLER_H
#define MANAGER_HANDLER_H

#include "server/server.h"
#include "constdef.h"

typedef void (*handler_t)(tcp_connection_t, byte_t *);

void zigbee_handler(tcp_connection_t, byte_t *);

void camera_handler(tcp_connection_t, byte_t *);

void voice_handler(tcp_connection_t, byte_t *);

#endif //MANAGER_HANDLER_H
