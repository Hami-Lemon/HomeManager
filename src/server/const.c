#include "const.h"

//设备号
const char DEVICE_VOICE = 0x00;  // 语音模块
const char DEVICE_CAMERA = 0x02; //相机模块
const char DEVICE_ZIGBEE = 0x04; // m0模块
//语音模块操作
const char OPERATION_VOICE_RECORD = 0x00; // 录音
const char OPERATION_VOICE_PLAY = 0x02;   // 播放语音
//相机模块操作

// zigbee模块操作
const char OPERATION_ZIGBEE_LIGHT_ON = 0x00;    //开灯
const char OPERATION_ZIGBEE_LIGHT_OFF = 0x01;   //关灯
const char OPERATION_ZIGBEE_BUZZER_ON = 0x02;   //开蜂鸣器
const char OPERATION_ZIGBEE_BUZZER_OFF = 0x03;  //关蜂鸣器
const char OPERATION_ZIGBEE_FANS_ON = 0x04;     //开风扇
const char OPERATION_ZIGBEE_FANS_SLOW = 0x05;   //风扇慢速
const char OPERATION_ZIGBEE_FANS_MID = 0x06;    //风扇中速
const char OPERATION_ZIGBEE_FANS_FAST = 0x07;   //风扇快速
const char OPERATION_ZIGBEE_FANS_OFF = 0x08;    //风扇关
const char OPERATION_ZIGBEE_DIGITAL_ON = 0x09;  //开数码管
const char OPERATION_ZIGBEE_DIGITAL_OFF = 0x0a; //关数码管