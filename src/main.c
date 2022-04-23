#include "server/server.h"
#include "server/connection.h"
#include "serial/serial.h"
#include "constdef.h"
#include "zigbee/zigbee.h"
#include "camera/camera.h"
#include "util.h"

int main() {
    /*TCP_SERVER *server = tcp_server_listen(NULL, 8888);
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
    return 0;*/
    /*zigbee_t *zigbee = zigbee_connect("/dev/ttyUSB0");
    if (zigbee == NULL) {
        perror("zigbee connect error!\n");
    }
    while (true) {
        int operation;
        scanf("%d", &operation);
        if (operation == 11) {
            break;
        }
        bool ok = zigbee_operation(zigbee, operation);
        if (!ok) {
            break;
        }
    }
    zigbee_disconnect(zigbee);*/
    camera_t *camera = camera_attach("/dev/video0");
    if (camera == NULL) {
        return 1;
    }
    camera_option_t opt;
    opt.width = 1920;
    opt.height = 1080;
    opt.format = 0;
    opt.fps = 30;

    camera_config(camera, opt);
    printf("%d %d\n", camera->option.height, camera->option.width);
    printf("%d %d\n", camera->option.format, camera->option.fps);
    close(camera->fd);
    free(camera);
    return 0;
}