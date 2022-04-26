//
// Created by Hami Lemon on 2022/4/26.
//

#ifndef MANAGER_VOICE_H
#define MANAGER_VOICE_H

#include "../constdef.h"
#include "../serial/serial.h"

extern const byte_t GBK;
extern const byte_t UTF8;
extern const byte_t UNICODE;

typedef struct VOICE {
    serial_t serial;
    byte_t charset;//文本所用的字符集
} voice_t;

//连接语音模块
voice_t *voice_attach(const char *name);

//播放语音
bool voice_play(voice_t *voice, const char *text, size_t size);

//断开连接
void voice_detach(voice_t *voice);

#endif //MANAGER_VOICE_H
