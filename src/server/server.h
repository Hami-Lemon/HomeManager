/*
 * TCP服务器
 */

#ifndef __SERVER_H
#define __SERVER_H

#include "connection.h"
#include "../constdef.h"

//最大的tcp连接数
extern const int TCP_SERVER_MAX_CONNECTION;

typedef struct TCP_SERVER {
    //套接字
    int sock_server;
    //ip地址
    char *ip;
    //端口号
    int port;
} tcp_server_t;

//开启tcp服务
tcp_server_t *tcp_server_listen(char *ip, int port);

//等待连接
tcp_connection_t tcp_server_accept(tcp_server_t *server);

//停止服务
void tcp_server_close(tcp_server_t *server);

#endif