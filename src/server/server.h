/*
 * TCP服务器
 */

#ifndef __SERVER_H
#define __SERVER_H

#include "../constdef.h"
#include "../cstring/cstring.h"

//默认的缓冲区大小
#define TCP_BUFFER_SIZE 1024
//最大的tcp连接数
extern const int TCP_SERVER_MAX_CONNECTION;

typedef int tcp_server_t;

//一个tcp连接
typedef int tcp_connection_t;

//开启tcp服务
tcp_server_t tcp_server_listen(char *ip, int port);

//等待连接
tcp_connection_t tcp_server_accept(tcp_server_t server);

//停止服务
void tcp_server_close(tcp_server_t server);

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回0
size_t tcp_connection_read(tcp_connection_t connection, byte_t *dst, size_t size);

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(tcp_connection_t connection, const byte_t *data, int offset, size_t size);

//关闭连接
void tcp_connection_close(tcp_connection_t connection);

#endif