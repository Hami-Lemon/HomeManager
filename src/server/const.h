#ifndef SERVER_CONST_H
#define SERVER_CONST_H

//设备号
extern const char DEVICE_VOICE;  // 语音模块
extern const char DEVICE_CAMERA; //相机模块
extern const char DEVICE_ZIGBEE; // m0模块
//语音模块操作
extern const char OPERATION_VOICE_RECORD; // 录音
extern const char OPERATION_VOICE_PLAY;   // 播放语音
//相机模块操作
// zigbee模块操作
extern const char OPERATION_ZIGBEE_LIGHT_ON;    //开灯
extern const char OPERATION_ZIGBEE_LIGHT_OFF;   //关灯
extern const char OPERATION_ZIGBEE_BUZZER_ON;   //开蜂鸣器
extern const char OPERATION_ZIGBEE_BUZZER_OFF;  //关蜂鸣器
extern const char OPERATION_ZIGBEE_FANS_ON;     //开风扇
extern const char OPERATION_ZIGBEE_FANS_SLOW;   //风扇慢速
extern const char OPERATION_ZIGBEE_FANS_MID;    //风扇中速
extern const char OPERATION_ZIGBEE_FANS_FAST;   //风扇快速
extern const char OPERATION_ZIGBEE_FANS_OFF;    //风扇关
extern const char OPERATION_ZIGBEE_DIGITAL_ON;  //开数码管
extern const char OPERATION_ZIGBEE_DIGITAL_OFF; //关数码管

#endif