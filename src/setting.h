//
// Created by Hami Lemon on 2022/4/23.
//

#ifndef MANAGER_SETTING_H
#define MANAGER_SETTING_H

#include "constdef.h"
#include <cJSON.h>

#define SETTING_FILE "./setting.json"

//摄像头设置
typedef struct CAMERA_SETTING {
    char *name;
    uint32_t width;
    uint32_t height;
    uint32_t fps;
} camera_setting_t;

//服务端设置
typedef struct SERVER_SETTING {
    char *ip;
    int port;
} server_setting_t;

//设置
typedef struct SETTING {
    char *zigbee;
    server_setting_t *server;
    camera_setting_t *camera;
} setting_t;

//读取设置
setting_t *read_setting();

void setting_print(setting_t *setting);

void setting_free(setting_t *setting);

#endif //MANAGER_SETTING_H
