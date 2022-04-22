//
// Created by Hami Lemon on 2022/4/22.
//

#include "util.h"

void data_copy(char *dst, const char *src, int start, int end) {
    int index = 0;
    for (int i = start; i < end; i++, index++) {
        dst[index] = src[i];
    }
}