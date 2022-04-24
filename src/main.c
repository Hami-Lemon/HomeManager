#include "server/server.h"
#include "constdef.h"
#include "zigbee/zigbee.h"
#include "setting.h"
#include "handler.h"
#include "logger/logger.h"
#include <signal.h>
#include <pthread.h>

#define HANDLER_NUM 3
setting_t *setting;
tcp_server_t server;
zigbee_t *zigbee;
handler_t handler_table[HANDLER_NUM];

bool attach_zigbee() {
    zigbee = zigbee_connect(setting->zigbee->c_str);
    if (zigbee == NULL) {
        logger_error(LOGGER("connect zigbee fail"));
        return false;
    }
    return true;
}

bool start_tcp_server() {
    server_setting_t *server_setting = setting->server;
    server = tcp_server_listen(server_setting->ip->c_str, server_setting->port);
    if (server == -1) {
        logger_error(LOGGER("start tcp server fail"));
        return false;
    }
    logger_info(LOGGER("listen: %s:%d"),
                server_setting->ip->c_str, server_setting->port);
    return true;
}

//释放资源
void release(int arg) {
    logger_info(LOGGER("stop progress, release resource"));
    tcp_server_close(server);
    zigbee_disconnect(zigbee);
    exit(0);
}

void *server_handler(void *args) {
    tcp_connection_t conn = *((tcp_connection_t *) args);
    byte_t buffer[TCP_BUFFER_SIZE];
    buffer[0] = 0;
    size_t len;
    while (true) {
        len = tcp_connection_read(conn, buffer, TCP_BUFFER_SIZE);
        if (len <= 0) {
            break;
        }
        if (len < 2 || buffer[0] >= HANDLER_NUM) {
            logger_warn(LOGGER("read error data"));
            continue;
        }
        handler_table[buffer[0]](conn, buffer);
    }
    tcp_connection_close(conn);
    logger_debug(LOGGER("connection:%d close"), conn);
    return NULL;
}

int main() {
    setting = read_setting();
    printf("===============================\n");
    setting_print(setting);
    printf("===============================\n");
    set_logger_level(setting->logger->level);
    set_logger_fmt(setting->logger->fmt->c_str);
    //启动各项服务
    if (!start_tcp_server()) {
        goto FAIL;
    }
    /*if (!attach_zigbee()) {
        goto FAIL;
    }*/
    //处理退出信号
    signal(SIGINT, release);
    signal(SIGQUIT, release);
    signal(SIGSEGV, release);
    signal(SIGTERM, release);
    setting_free(setting);
    handler_table[DEVICE_ZIGBEE] = zigbee_handler;
    handler_table[DEVICE_CAMERA] = camera_handler;
    handler_table[DEVICE_VOICE] = voice_handler;

    while (true) {
        tcp_connection_t conn = tcp_server_accept(server);
        if (conn == -1) {
            logger_error(LOGGER("error connection!"));
        } else {
            logger_debug(LOGGER("get connection:%d"), conn);
            //新建线程处理连接
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            int ok = pthread_create(&tid, &attr, server_handler, (void *) &conn);
            pthread_attr_destroy(&attr);
            if (ok != 0) {
                tcp_connection_close(conn);
                logger_error(LOGGER("create thread error, connection close"));
            }
        }
    }
    FAIL:
    release(0);
    setting_free(setting);
    return 1;
}