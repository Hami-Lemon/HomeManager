#include "server/server.h"
#include "server/connection.h"
#include "const.h"
#include <stdio.h>

int main() {
    TCP_SERVER *server = tcp_server_listen(NULL, 8888);
    if (server == NULL) {
        return 1;
    }
    while (TRUE) {
        TCP_CONNECTION connection = tcp_server_accept(server);
        if (connection == -1) {
            break;
        }
        TCP_CONNECTION_DATA *data = tcp_connection_read(connection);
        if (data == NULL) {
            break;
        }
        printf("device:%d operation:%d length:%ld\n",
               data->device, data->operation, data->data_length);
        printf("%s\n", data->data);
        tcp_connection_write(connection, data->data, 0, data->data_length);

        tcp_connection_data_release(data);
        tcp_connection_close(connection);
    }

    tcp_server_close(server);
    return 0;
}