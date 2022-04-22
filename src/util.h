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

#endif //MANAGER_UTIL_H
