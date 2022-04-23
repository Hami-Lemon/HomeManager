/*
 * 串口操作的封装
 */
#ifndef __SERIAL_H
#define __SERIAL_H

#include "../constdef.h"
#include <termio.h>
#include <stddef.h>

//默认缓冲区大小
#define SERIAL_BUFFER_SIZE 64
//串口配置
typedef struct SERIAL_OPTION {
    char *name; //串口文件
    speed_t baud;//波特率
} serial_option_t;

//串口
typedef int serial_t;

//打开串口
serial_t serial_open(serial_option_t option);

//从串口中读数据
size_t serial_read(serial_t serial, byte_t *dst, size_t size);

//向串口写数据
size_t serial_write(serial_t serial, byte_t *data, int offset, size_t size);

//关闭串口
void serial_close(serial_t serial);

#endif