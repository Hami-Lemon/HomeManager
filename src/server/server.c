#include "server.h"
#include "../logger/logger.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int TCP_SERVER_MAX_CONNECTION = 10;

//开启tcp服务
tcp_server_t tcp_server_listen(char *ip, int port) {
    //默认ip和端口号
    if (ip == NULL) {
        ip = "0.0.0.0";
        logger_warn(LOGGER("use default ip:0.0.0.0"));
    }
    if (port == 0) {
        port = 8888;
        logger_warn(LOGGER("use default port:8888"));
    }

    int sock_ser = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock_ser) {
        logger_error(LOGGER("open socket error"));
        perror("");
        return -1;
    }

    int opt = 1;
    int ret = setsockopt(sock_ser, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(opt));
    if (-1 == ret) {
        logger_error(LOGGER("set socket option error!"));
        perror("");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;            //填写网络协议
    addr.sin_port = htons(port);          //填写端口号
    addr.sin_addr.s_addr = inet_addr(ip); //设置监听的ip地址
    ret = bind(sock_ser, (struct sockaddr *) &addr, sizeof(addr));
    if (-1 == ret) {
        logger_error(LOGGER("bind address error"));
        perror("");
        return -1;
    }
    ret = listen(sock_ser, TCP_SERVER_MAX_CONNECTION);
    if (-1 == ret) {
        logger_error(LOGGER("listen address error"));
        perror("");
        return -1;
    }

    return sock_ser;
}

//等待连接
tcp_connection_t tcp_server_accept(tcp_server_t server) {
    if (server == -1) {
        return -1;
    }
    struct sockaddr_in accept_addr;
    socklen_t addr_len = sizeof(accept_addr);
    int sock_conn = accept(server, (struct sockaddr *) &accept_addr, &addr_len);
    if (sock_conn == -1) {
        logger_error(LOGGER("get connection error"));
        perror("");
    }
    return sock_conn;
}

//停止服务
void tcp_server_close(tcp_server_t server) {
    if (server != -1) {
        close(server);
    }
}

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回0
size_t tcp_connection_read(tcp_connection_t connection, byte_t *dst, size_t size) {
    if (connection == -1 || dst == NULL) {
        return 0;
    }
    ssize_t length = read(connection, dst, size);
    //连接断开
    if (length <= 0) {
        return 0;
    }
    return length;
}

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(tcp_connection_t connection, const byte_t *data, int offset, size_t size) {
    if (connection == -1) {
        return 0;
    }
    unsigned long end = offset + size;
    byte_t *buffer = malloc(sizeof(byte_t) * size);
    for (int index = 0, i = offset; i < end; ++i, ++index) {
        buffer[index] = data[i];
    }
    ssize_t length = write(connection, buffer, size);
    free(buffer);

    if (length < size) {
        return 0;
    }
    return length;
}

//关闭连接
void tcp_connection_close(tcp_connection_t connection) {
    if (connection != -1) {
        close(connection);
    }
}