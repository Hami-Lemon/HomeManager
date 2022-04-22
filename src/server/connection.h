#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "const.h"
#include <stddef.h>
//缓冲区大小
#define TCP_CONNECTION_BUFFER_SIZE 1024

//一个tcp连接
typedef int TCP_CONNECTION;

//从连接中读取的数据体
typedef struct TCP_CONNECTION_DATA {
    char device;        //设备号
    char operation;     //操作码
    char *data;         //数据体
    size_t data_length; //数据体长度
} TCP_CONNECTION_DATA;

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回-1
TCP_CONNECTION_DATA *tcp_connection_read(TCP_CONNECTION connection);

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(TCP_CONNECTION connection, const char *data, int offset, size_t size);

//关闭连接
void tcp_connection_close(TCP_CONNECTION connection);

//释放数据体
void tcp_connection_data_release(TCP_CONNECTION_DATA *data);

#endif