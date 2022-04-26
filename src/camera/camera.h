//
// Created by Hami Lemon on 2022/4/23.
//

#ifndef MANAGER_CAMERA_H
#define MANAGER_CAMERA_H

#include "../constdef.h"

#define CAMERA_BUFFER_COUNT 4
//摄像头相关配置
typedef struct CAMERA_OPTION {
    uint32_t width; //采集图像的宽度
    uint32_t height; //采集图像的高度
    uint32_t fps;//每秒采集的帧数
} camera_option_t;

typedef struct CAMERA_BUFFER {
    uint8_t *start;
    size_t length;
} camera_buffer_t;

typedef struct CAMERA {
    int fd;//对应的文件描述符
    camera_option_t option;
    camera_buffer_t *buffer;
    camera_buffer_t curr; // 当前捕获的一张图片
    size_t buffer_count;
    bool capturing; //是否已经开启采集
} camera_t;

//连接摄像头
camera_t *camera_attach(const char *device);

//配置摄像头
bool camera_config(camera_t *camera, camera_option_t option);

//断开摄像头连接
void camera_detach(camera_t *camera);

//开始采集
bool camera_start(camera_t *camera);

//停止采集
bool camera_stop(camera_t *camera);

//捕获一张图片
bool camera_capture(camera_t *camera);

#endif //MANAGER_CAMERA_H
