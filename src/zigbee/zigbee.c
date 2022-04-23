//
// Created by Hami Lemon on 2022/4/22.
//

#include "zigbee.h"

byte_t send_data[ZIGBEE_BUFFER_SIZE];

void init_send_data() {
    //置零
    for (int i = 0; i < ZIGBEE_BUFFER_SIZE; i++) {
        send_data[i] = 0;
    }
    send_data[0] = 0xdd;
    send_data[2] = 0x24;
}

//连接ZIGBEE
zigbee_t *zigbee_connect(char *name) {
    serial_option_t option;
    option.name = name;
    option.baud = B115200;
    serial_t serial = serial_open(option);
    if (serial == -1) {
        return NULL;
    }
    zigbee_t *zigbee = malloc(sizeof(zigbee_t));
    zigbee->serial = serial;
    byte_t buffer[ZIGBEE_BUFFER_SIZE] = {0};
    if (zigbee_read(zigbee, buffer) == 0) {
        return NULL;
    }
    zigbee->device_id = buffer[1];
    init_send_data();
    return zigbee;
}

//读取数据
int zigbee_read(zigbee_t *zigbee, byte_t *dst) {
    if (zigbee == NULL) {
        return 0;
    }
    serial_t serial = zigbee->serial;
    int len;
    do {
        len = (int) serial_read(serial, dst, ZIGBEE_BUFFER_SIZE);
        if (len <= 0) {
            return 0;
        }
    } while (dst[0] != 0xbb);
    return len;
}

//发送命令
bool zigbee_operation(zigbee_t *zigbee, byte_t operation) {
    if (zigbee == NULL) {
        return false;
    }
    send_data[1] = zigbee->device_id;
    send_data[4] = operation;
    size_t len = serial_write(zigbee->serial, send_data, 0, ZIGBEE_BUFFER_SIZE);
    if (len < ZIGBEE_BUFFER_SIZE) {
        return false;
    }
    return true;
}

//断开连接
void zigbee_disconnect(zigbee_t *zigbee) {
    if (zigbee == NULL) {
        return;
    }
    serial_close(zigbee->serial);
    free(zigbee);
}
