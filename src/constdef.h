/*
 * 自定义的常量
 */
#ifndef MANAGER_CONSTDEF_H
#define MANAGER_CONSTDEF_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef unsigned char byte_t;

//设备号
extern const byte_t DEVICE_VOICE;  // 语音模块
extern const byte_t DEVICE_CAMERA; //相机模块
extern const byte_t DEVICE_ZIGBEE; // m0模块
//语音模块操作
extern const byte_t OPERATION_VOICE_RECORD; // 录音
extern const byte_t OPERATION_VOICE_PLAY;   // 播放语音
//相机模块操作
extern const byte_t OPERATION_CAMERA_START; //摄像头开始采集
extern const byte_t OPERATION_CAMERA_CAPTURE; //捕获一张图片
extern const byte_t OPERATION_CAMERA_STOP; //摄像头停止采集
// zigbee模块操作
extern const byte_t OPERATION_ZIGBEE_LIGHT_ON;    //开灯
extern const byte_t OPERATION_ZIGBEE_LIGHT_OFF;   //关灯
extern const byte_t OPERATION_ZIGBEE_BUZZER_ON;   //开蜂鸣器
extern const byte_t OPERATION_ZIGBEE_BUZZER_OFF;  //关蜂鸣器
extern const byte_t OPERATION_ZIGBEE_FANS_ON;     //开风扇
extern const byte_t OPERATION_ZIGBEE_FANS_SLOW;   //风扇慢速
extern const byte_t OPERATION_ZIGBEE_FANS_MID;    //风扇中速
extern const byte_t OPERATION_ZIGBEE_FANS_FAST;   //风扇快速
extern const byte_t OPERATION_ZIGBEE_FANS_OFF;    //风扇关
extern const byte_t OPERATION_ZIGBEE_DIGITAL_ON;  //开数码管
extern const byte_t OPERATION_ZIGBEE_DIGITAL_OFF; //关数码管
extern const byte_t OPERATION_ZIGBEE_INFO; //获取环境信息
#endif