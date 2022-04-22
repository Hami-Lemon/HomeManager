//
// Created by Hami Lemon on 2022/4/22.
//

#ifndef MANAGER_ZIGBEE_H
#define MANAGER_ZIGBEE_H

#include "../serial/serial.h"

typedef struct ZIGBEE {
    SERIAL serial; //对应的串口
    int device_id; //设备id
} ZIGBEE;

//连接ZIGBEE
ZIGBEE *zigbee_connect(SERIAL serial);

//发送命令
void zigbee_operation(ZIGBEE *zigbee, int operation);

#endif //MANAGER_ZIGBEE_H
