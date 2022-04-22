//
// Created by Hami Lemon on 2022/4/22.
//
/*
 * ZIGBEE通信的封装
 */
#ifndef MANAGER_ZIGBEE_H
#define MANAGER_ZIGBEE_H

#include "../serial/serial.h"
#include "../constdef.h"
//读写的缓冲区大小
#define ZIGBEE_BUFFER_SIZE 36

typedef struct ZIGBEE {
    SERIAL serial; //对应的串口
    int device_id; //设备id
} ZIGBEE;

//连接ZIGBEE
ZIGBEE *zigbee_connect(char *name);

//读取数据
int zigbee_read(ZIGBEE *zigbee, byte_t *dst);

//发送命令
bool_t zigbee_operation(ZIGBEE *zigbee, byte_t operation);

//断开连接
void zigbee_disconnect(ZIGBEE *zigbee);

#endif //MANAGER_ZIGBEE_H
