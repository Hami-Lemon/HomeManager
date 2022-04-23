//
// Created by Hami Lemon on 2022/4/22.
//
/*
 * 一些常用的函数
 */
#ifndef MANAGER_UTIL_H
#define MANAGER_UTIL_H

#include "constdef.h"

//数据复制，src复制到dst中，从start开始，end结束，左开右闭区间
void data_copy(byte_t *dst, const byte_t *src, int start, int end);

/*
 * 输出信息
 */

void log_info(const char *msg);

void log_infoln(const char *msg);

void log_perror(const char *msg);

void log_error(const char *msg);

void log_errorln(const char *msg);

void log_warn(const char *msg);

void log_warnln(const char *msg);

void info();

void infoln();

void error();

void errorln();

void warn();

void warnln();

#endif //MANAGER_UTIL_H
