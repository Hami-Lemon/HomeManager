#ifndef __SERIAL_H
#define __SERIAL_H

#include <termio.h>
#include <stddef.h>

extern const int SERIAL_BUFFER_SIZE;
//串口配置
typedef struct SERIAL_OPTION {
    char *name; //串口文件
    speed_t baud;//波特率
} SERIAL_OPTION;

//串口
typedef int SERIAL;

//打开串口
SERIAL serial_open(SERIAL_OPTION option);

//从串口中读数据
size_t serial_read(SERIAL serial, char *dst, size_t size);

//向串口写数据
size_t serial_write(SERIAL serial, char *data, int offset, size_t size);

//关闭串口
void serial_close(SERIAL serial);

#endif