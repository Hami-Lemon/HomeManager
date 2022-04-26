//
// Created by Hami Lemon on 2022/4/26.
//

#include "voice.h"
#include "../logger/logger.h"
#include <endian.h>

const byte_t GBK = 0x01;
const byte_t UTF8 = 0x02;
const byte_t UNICODE = 0x03;

//utf8转unicode
size_t utf8_to_unicode(byte_t *unicode, const byte_t *utf8, size_t size, int endian) {
    size_t index = 0;
    for (int i = 0; i < size; index += 2) {
        byte_t low = 0, high = 0;
        byte_t data = utf8[i];
        if ((data & 0b10000000) == 0) {
            //一个字节
            //0xxx xxxx
            low = data;
            i++;
        } else if ((data & 0b11100000) == 0b11000000) {
            //两个字节
            //110x xxxx 10xx xxxx
            if ((i + 1) >= size) {
                //越界
                low = 0xfd;
                high = 0xff;
            } else {
                byte_t data1 = utf8[i + 1];
                low = (data1 & 0b00111111);
                low |= (data << 6) & 0b11000000;

                high = (data >> 2) & 0b00000111;
            }
            i += 2;
        } else if ((data & 0b11110000) == 0b11100000) {
            //三个字节
            //1110 xxxx 10xx xxxx 10xx xxxx
            if ((i + 2) >= size) {
                //越界
                low = 0xfd;
                high = 0xff;
            } else {
                byte_t data1 = utf8[i + 1];
                byte_t data2 = utf8[i + 2];

                low = (data2 & 0b00111111);
                low |= (data1 << 6) & 0b11000000;

                high = (data1 >> 2) & 0b00001111;
                high |= (data << 4) & 0b11110000;
            }
            i += 3;
        } else {
            //不处理
            logger_warn(LOGGER("utf-8 string has more than 3 bytes"));
            return 0;
        }
        if (endian == BIG_ENDIAN) {
            //大端序
            unicode[index] = high;
            unicode[index + 1] = low;
        } else {
            //小端序
            unicode[index] = low;
            unicode[index + 1] = high;
        }
    }
    return index;
}

//连接语音模块
voice_t *voice_attach(const char *name) {
    if (name == NULL) {
        logger_warn(LOGGER("name if NULL"));
        return NULL;
    }
    serial_option_t opt;
    opt.name = name;
    opt.baud = B9600;
    serial_t serial = serial_open(opt);
    if (serial == -1) {
        logger_error(LOGGER("attach voice fail"));
        return NULL;
    }
    voice_t *voice = malloc(sizeof(voice_t));
    voice->serial = serial;
    voice->charset = UTF8;
    //唤醒设备
    byte_t buf[4] = {0xfd, 0x00, 0x01, 0xff};
    size_t wn = serial_write(serial, buf, 0, 4);
    logger_debug(LOGGER("voice: send %d bytes"), wn);
    //音量设为最大
    voice->charset = GBK;
    voice_play(voice, "[v10]", 5);
    voice->charset = UTF8;
    return voice;
}

//播放语音
bool voice_play(voice_t *voice, const char *text, size_t size) {
    byte_t *buf;
    //数据体长度
    size_t length = 0;
    //utf8转unicode
    if (voice->charset == UTF8) {
        byte_t *unicode = calloc(size * 2, sizeof(byte_t));
        length = utf8_to_unicode(unicode, (byte_t *) text, size, LITTLE_ENDIAN);
        buf = calloc(length + 5, sizeof(byte_t));
        memcpy(buf + 5, unicode, length);
        buf[4] = UNICODE;
        length += 2;
        free(unicode);
    } else {
        buf = calloc(size + 5, sizeof(byte_t));
        memcpy(buf + 5, text, size);
        buf[4] = voice->charset;//编码
        length = size + 2;
    }
    buf[0] = 0xfd;
    buf[1] = (length >> 8) & 0xff;//高字节
    buf[2] = length & 0xff; //低字节
    buf[3] = 0x01;//命令字

    size_t wn = serial_write(voice->serial, buf, 0, length + 3);
    logger_debug(LOGGER("voice: send %d bytes"), wn);
    if (wn < 5) {
        logger_error(LOGGER("voice: send data num less than src"));
        return false;
    }
    size_t rn = serial_read(voice->serial, buf, 1);
    if (rn <= 0) {
        logger_warn(LOGGER("voice: read return data fail"));
        return false;
    }
    if (buf[0] == 0x45) {
        logger_warn(LOGGER("voice: play fail, return 0x45"));
        return false;
    }
    free(buf);
    return true;
}

//断开连接
void voice_detach(voice_t *voice) {
    if (voice != NULL) {
        serial_close(voice->serial);
        free(voice);
    }
}