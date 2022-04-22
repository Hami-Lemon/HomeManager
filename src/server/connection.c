#include "connection.h"
#include "../util.h"
#include <stdlib.h>
#include <unistd.h>

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回-1
TCP_CONNECTION_DATA *tcp_connection_read(TCP_CONNECTION connection) {
    if (connection == -1) {
        return NULL;
    }
    char dst[TCP_CONNECTION_BUFFER_SIZE] = {0};
    size_t length = read(connection, dst, TCP_CONNECTION_BUFFER_SIZE);
    //连接断开
    if (length <= 0) {
        return NULL;
    }
    TCP_CONNECTION_DATA *data = malloc(sizeof(TCP_CONNECTION_DATA));
    if (length < 2) {
        return NULL;
    }
    data->device = dst[0];
    data->operation = dst[1];

    data->data = malloc(sizeof(char) * (length - 2));
    data->data_length = length - 2;

    data_copy(data->data, dst, 2, (int) length);
    return data;
}

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(TCP_CONNECTION connection, const char *data, int offset, size_t size) {
    if (connection == -1) {
        return 0;
    }
    unsigned long end = offset + size;
    char *buffer = malloc(sizeof(char) * size);
    data_copy(buffer, data, offset, (int) end);
    size_t length = write(connection, buffer, size);
    free(buffer);

    if (length < size) {
        return 0;
    }
    return length;
}

//关闭连接
void tcp_connection_close(TCP_CONNECTION connection) {
    if (connection == -1) {
        return;
    }
    close(connection);
}

//释放数据体
void tcp_connection_data_release(TCP_CONNECTION_DATA *data) {
    if (data == NULL) {
        return;
    }
    free(data->data);
    free(data);
}