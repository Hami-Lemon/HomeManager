#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int TCP_SERVER_MAX_CONNECTION = 10;

//开启tcp服务
tcp_server_t *tcp_server_listen(char *ip, int port) {
    //默认ip和端口号
    if (ip == NULL) {
        ip = "0.0.0.0";
    }
    if (port == 0) {
        port = 8888;
    }

    int sock_ser = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock_ser) {
        perror("sock");
        exit(1);
    }

    int opt = 1;
    int ret = setsockopt(sock_ser, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(opt));
    if (-1 == ret) {
        perror("set socket option error!");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;            //填写网络协议
    addr.sin_port = htons(port);          //填写端口号
    addr.sin_addr.s_addr = inet_addr(ip); //自动填写ip地址
    ret = bind(sock_ser, (struct sockaddr *) &addr, sizeof(addr));
    if (-1 == ret) {
        perror("bind");
        exit(1);
    }
    ret = listen(sock_ser, TCP_SERVER_MAX_CONNECTION);
    if (-1 == ret) {
        perror("listen");
        exit(1);
    }

    tcp_server_t *server = malloc(sizeof(tcp_server_t));
    server->sock_server = sock_ser;
    server->ip = malloc(sizeof(char) * strlen(ip));
    strcpy(server->ip, ip);
    server->port = port;
    return server;
}

//等待连接
tcp_connection_t tcp_server_accept(tcp_server_t *server) {
    if (server == NULL) {
        return -1;
    }
    struct sockaddr_in accept_addr;
    socklen_t addr_len = sizeof(accept_addr);
    int sock_conn = accept(server->sock_server, (struct sockaddr *) &accept_addr, &addr_len);
    if (sock_conn == -1) {
        perror("get connection error!");
    }
    return sock_conn;
}

//停止服务
void tcp_server_close(tcp_server_t *server) {
    if (server == NULL) {
        return;
    }

    close(server->sock_server);
    free(server);
}