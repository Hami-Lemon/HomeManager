//
// Created by Hami Lemon on 2022/4/22.
//

#include "util.h"

void data_copy(byte_t *dst, const byte_t *src, int start, int end) {
    int index = 0;
    for (int i = start; i < end; i++, index++) {
        dst[index] = src[i];
    }
}