#include "connection.h"
#include "../util.h"

//读取数据，返回读取的数据长度，内容保存在dst中，如果连接关闭，返回-1
tcp_connection_data_t *tcp_connection_read(tcp_connection_t connection) {
    if (connection == -1) {
        return NULL;
    }
    byte_t dst[TCP_CONNECTION_BUFFER_SIZE] = {0};
    size_t length = read(connection, dst, TCP_CONNECTION_BUFFER_SIZE);
    //连接断开
    if (length <= 0) {
        return NULL;
    }
    tcp_connection_data_t *data = malloc(sizeof(tcp_connection_data_t));
    if (length < 2) {
        return NULL;
    }
    data->device = dst[0];
    data->operation = dst[1];

    data->data = malloc(sizeof(byte_t) * (length - 2));
    data->data_length = length - 2;

    data_copy(data->data, dst, 2, (int) length);
    return data;
}

//发送数据，offset为偏移量，size为数据长度，返回发送的数据长度
size_t tcp_connection_write(tcp_connection_t connection, const byte_t *data, int offset, size_t size) {
    if (connection == -1) {
        return 0;
    }
    unsigned long end = offset + size;
    byte_t *buffer = malloc(sizeof(byte_t) * size);
    data_copy(buffer, data, offset, (int) end);
    size_t length = write(connection, buffer, size);
    free(buffer);

    if (length < size) {
        return 0;
    }
    return length;
}

//关闭连接
void tcp_connection_close(tcp_connection_t connection) {
    if (connection == -1) {
        return;
    }
    close(connection);
}

//释放数据体
void tcp_connection_data_release(tcp_connection_data_t *data) {
    if (data == NULL) {
        return;
    }
    free(data->data);
    free(data);
}