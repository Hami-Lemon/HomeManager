//
// Created by Hami Lemon on 2022/4/24.
//

#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//日志格式最大长度
#define LOGGER_FMT_LENGTH 256
#define BUFFER_SIZE 1024 //日志信息缓冲区大小
static log_level current_level = -1; //当前日志级别
static char logger_fmt[LOGGER_FMT_LENGTH];//日志信息格式
static size_t format_len = 0;//日志信息格式的实际长度

const char *const LOGGER_LEVEL_TABLE[] = {
        "[DEBUG]", "[INFO]", "[WARN]", "[ERROR]"
};

//设置日志级别，只能程序启动时设置
void set_logger_level(log_level level) {
    if (current_level == -1) {
        current_level = level;
    }
}

/**
 * 设置日志格式
 * %%: 转义%
 * %f: 文件名
 * %n: 行号
 * %m: 日志信息
 * %d: 日期 MM-dd
 * %t：时间 HH:mm:ss
 * %l: 日志级别
 */
void set_logger_fmt(const char *fmt) {
    format_len = strlen(fmt);
    if (format_len > LOGGER_FMT_LENGTH) {
        fprintf(stderr, "format too long!\n");
        return;
    }
    strcpy(logger_fmt, fmt);
}

//获取文件名
const char *logger_basename(const char *path) {
    const char *slash = strrchr(path, '/');
    if (slash) {
        path = slash + 1;
    }
    return path;
}

//从offset开始，向buffer中追加str，返回追加后buffer的长度
size_t logger_str_append(char *buffer, size_t offset, const char *str) {
    size_t len = strlen(str);
    size_t index = offset;
    for (int i = 0; i < len; ++i, ++index) {
        buffer[index] = str[i];
    }
    buffer[index] = '\0';
    return index;
}

size_t logger_str_append_int(char *buffer, size_t offset, long int num) {
    char buf[32];
    snprintf(buf, 32, "%ld", num);
    return logger_str_append(buffer, offset, buf);
}

size_t logger_str_append_date(char *buffer, size_t offset, struct tm *lt) {
    char buf[128];
    size_t len = strftime(buf, 128, "%m-%d", lt);
    if (len > 0) {
        return logger_str_append(buffer, offset, buf);
    } else {
        return offset;
    }
}

size_t logger_str_append_time(char *buffer, size_t offset, struct tm *lt) {
    char buf[128];
    size_t len = strftime(buf, 128, "%H:%M:%S", lt);
    if (len > 0) {
        return logger_str_append(buffer, offset, buf);
    } else {
        return offset;
    }
}

//格式化日志信息
void logger_format(char *buffer, size_t buf_size,
                   const char *s_file, int s_line, log_level level, const char *msg) {
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);//时间
    s_file = logger_basename(s_file); //文件名

    size_t index = 0;
    buffer[index] = '\0';
    for (int i = 0; i < format_len; ++i) {
        //找到起始的%
        while (i < format_len && logger_fmt[i] != '%') {
            buffer[index] = logger_fmt[i];
            ++i;
            ++index;
        }
        i++;
        //到达末尾
        if (i >= format_len) {
            break;
        }
        //格式化
        switch (logger_fmt[i]) {
            case '%':
                buffer[index] = '%';
                index++;
                break;
            case 'f':
                index = logger_str_append(buffer, index, s_file);
                break;
            case 'n':
                index = logger_str_append_int(buffer, index, s_line);
                break;
            case 'm':
                index = logger_str_append(buffer, index, msg);
                break;
            case 'd':
                index = logger_str_append_date(buffer, index, lt);
                break;
            case 't':
                index = logger_str_append_time(buffer, index, lt);
                break;
            case 'l':
                index = logger_str_append(buffer, index, LOGGER_LEVEL_TABLE[level]);
                break;
        }
        if (index >= buf_size) {
            fprintf(stderr, "log msg to long!\n");
            index = buf_size - 1;
            break;
        }
    }
    buffer[index] = '\0';
}

void logger_log(const char *s_file, int s_line, log_level level, const char *fmt, va_list ap) {
    char buffer[BUFFER_SIZE];
    int msg_len = vsnprintf(buffer, BUFFER_SIZE, fmt, ap);
    if ((size_t) msg_len >= BUFFER_SIZE) {
        //日志信息过长
        fprintf(stderr, "log msg to long!\n");
        return;
    }
    char msg_buffer[BUFFER_SIZE];
    logger_format(msg_buffer, BUFFER_SIZE, s_file, s_line, level, buffer);
    if (level == LOG_ERROR) {
        //error级别输出到错误流中
        fprintf(stderr, "%s\n", msg_buffer);
    } else {
        fprintf(stdout, "%s\n", msg_buffer);
    }
}

void logger_debug(const char *s_file, int s_line, const char *fmt, ...) {
    if (LOG_DEBUG < current_level) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    logger_log(s_file, s_line, LOG_DEBUG, fmt, ap);
    va_end(ap);
}

void logger_info(const char *s_file, int s_line, const char *fmt, ...) {
    if (LOG_INFO < current_level) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    logger_log(s_file, s_line, LOG_INFO, fmt, ap);
    va_end(ap);
}

void logger_warn(const char *s_file, int s_line, const char *fmt, ...) {
    if (LOG_WARN < current_level) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    logger_log(s_file, s_line, LOG_WARN, fmt, ap);
    va_end(ap);
}

void logger_error(const char *s_file, int s_line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    logger_log(s_file, s_line, LOG_ERROR, fmt, ap);
    va_end(ap);
}
