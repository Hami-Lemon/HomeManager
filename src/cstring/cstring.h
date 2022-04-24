//字符串
#ifndef MANAGER_CSTRING_H
#define MANAGER_CSTRING_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *c_str;
    unsigned int length;
} string_t;

//创建string， length为长度，如果为-1，则会通过strlen获取
string_t *string_new(char *src, int length);

//字符串拼接
string_t *string_cat(string_t *str1, string_t *str2);

//获取子串
string_t *string_substring(string_t *src, int start, int end);

//检查是否包含子串
bool string_str(string_t *src, string_t *sub);

//比较字符串是否相等
bool string_equal(string_t *str1, string_t *str2);

//字符串复制
string_t *string_copy(string_t *src);
//释放
void string_free(string_t *src);

#endif //MANAGER_CSTRING_H
