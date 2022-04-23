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
    serial_t serial; //对应的串口
    int device_id; //设备id
} zigbee_t;

//连接ZIGBEE
zigbee_t *zigbee_connect(char *name);

//读取数据
int zigbee_read(zigbee_t *zigbee, byte_t *dst);

//发送命令
bool zigbee_operation(zigbee_t *zigbee, byte_t operation);

//断开连接
void zigbee_disconnect(zigbee_t *zigbee);

#endif //MANAGER_ZIGBEE_H
