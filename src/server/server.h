#ifndef __SERVER_H
#define __SERVER_H
#include "connection.h"

//最大的tcp连接数
extern const int TCP_SERVER_MAX_CONNECTION;

typedef struct TCP_SERVER
{
    //套接字
    int sock_server;
    //ip地址
    char *ip;
    //端口号
    int port;
}TCP_SERVER;
//开启tcp服务
TCP_SERVER *tcp_server_listen(char *ip, int port);
//等待连接
TCP_CONNECTION tcp_server_accept(TCP_SERVER *server);

//停止服务
void tcp_server_close(TCP_SERVER *server);
#endif