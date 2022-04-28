#include "server/server.h"
#include "constdef.h"
#include "zigbee/zigbee.h"
#include "setting.h"
#include "handler.h"
#include "logger/logger.h"
#include "camera/camera.h"
#include "voice/voice.h"
#include <signal.h>
#include <pthread.h>

#define HANDLER_NUM 3

static setting_t *setting;
static tcp_server_t server;
static zigbee_t *zigbee;
static camera_t *camera;
static voice_t *voice;
static handler_t handler_table[HANDLER_NUM];//设备操作表

//连接语音模块
bool connect_voice() {
    voice = voice_attach(setting->voice->c_str);
    if (voice == NULL) {
        logger_error(LOGGER("connect voice fail"));
        return false;
    }
    logger_info(LOGGER("connect voice success"));
    return true;
}

//连接摄像头
bool connect_camera() {
    camera_setting_t *camera_setting = setting->camera;
    camera_option_t option;
    option.width = camera_setting->width;
    option.height = camera_setting->height;
    option.fps = camera_setting->fps;
    camera = camera_attach(camera_setting->name->c_str);
    if (camera == NULL) {
        logger_error(LOGGER("connect camera fail"));
        return false;
    }
    logger_info(LOGGER("connect camera success"));
    bool ok = camera_config(camera, option);
    if (!ok) {
        logger_error(LOGGER("config camera fail"));
        return false;
    }
    logger_info(LOGGER("width:%d height:%d fps:%d"),
                camera->option.width, camera->option.height, camera->option.fps);
    return true;
}
//连接M0模块
bool attach_zigbee() {
    zigbee = zigbee_connect(setting->zigbee->c_str);
    if (zigbee == NULL) {
        logger_error(LOGGER("connect zigbee fail"));
        return false;
    }
    logger_info(LOGGER("connect zigbee success"));
    logger_info(LOGGER("zigbee device id:%d"), zigbee->device_id);
    return true;
}
//启动tcp服务器
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
    if (arg == SIGSEGV) {
        logger_warn(LOGGER("memory error"));
    }
    logger_info(LOGGER("stop progress, release resource"));
    tcp_server_close(server);
    zigbee_disconnect(zigbee);
    camera_detach(camera);
    voice_detach(voice);
    exit(0);
}

//处理每一个连接
void *server_handler(void *args) {
    tcp_connection_t conn = *((tcp_connection_t *) args);
    byte_t *buffer = calloc(TCP_BUFFER_SIZE, sizeof(byte_t));
    size_t len;
    while (true) {
        //读取数据
        len = tcp_connection_read(conn, buffer, TCP_BUFFER_SIZE);
        if (len == 0) {
            break;
        }
        if (len < 2 || buffer[0] >= HANDLER_NUM) {
            logger_warn(LOGGER("read error data"));
            continue;
        }
        logger_debug(LOGGER("read conn %d bytes"), len);
        //根据请求数据中的设备号，选择对应的处理函数
        byte_t device_code = buffer[0];
        void *device = NULL;
        if (device_code == DEVICE_VOICE) {
            device = voice;
            printf("%s\n", buffer + 3);
        } else if (device_code == DEVICE_CAMERA) {
            device = camera;
        } else if (device_code == DEVICE_ZIGBEE) {
            device = zigbee;
        }
        handler_table[device_code](conn, device, buffer, len);
    }
    logger_debug(LOGGER("connection:%d close"), conn);
    free(buffer);
    tcp_connection_close(conn);
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
    if (!attach_zigbee()) {
//        goto FAIL;
    }
    if (!connect_voice()) {
//        goto FAIL;
    }
    if (!connect_camera()) {
//        goto FAIL;
    }
    if (!start_tcp_server()) {
        goto FAIL;
    }
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