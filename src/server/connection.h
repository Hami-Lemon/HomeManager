/*
 * TCP连接封装
 */
#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "../constdef.h"
#include "const.h"
//缓冲区大小
#define TCP_CONNECTION_BUFFER_SIZE 1024

//一个tcp连接
typedef int tcp_connection_t;

//从连接中读取的数据体
typedef struct TCP_CONNECTION_DATA {
    byte_t device;        //设备号
    byte_t operation;     //操作码
    byte_t *data;         //数据体
    size_t data_length; //数据体长度
} tcp_connection_data_t;

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回-1
tcp_connection_data_t *tcp_connection_read(tcp_connection_t connection);

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(tcp_connection_t connection, const byte_t *data, int offset, size_t size);

//关闭连接
void tcp_connection_close(tcp_connection_t connection);

//释放数据体
void tcp_connection_data_release(tcp_connection_data_t *data);

#endif