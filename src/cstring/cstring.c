
#include "cstring.h"

//创建string， length为长度，如果为-1，则会通过strlen获取
string_t *string_new(char *src, int length) {
    unsigned int len = length < 0 ? strlen(src) : length;
    string_t *str = malloc(sizeof(string_t));
    str->c_str = malloc(sizeof(char) * (len + 1));
    str->length = len;
    for (int i = 0; i < len && src[i] != '\0'; ++i) {
        str->c_str[i] = src[i];
    }
    str->c_str[len] = '\0';
    return str;
}

//字符串拼接
string_t *string_cat(string_t *str1, string_t *str2) {
    unsigned int len = str1->length + str2->length;
    string_t *str = malloc(sizeof(string_t));
    str->c_str = malloc((sizeof(char) * (len + 1)));
    str->length = len;
    int index = 0;
    for (int i = 0; i < str1->length; ++i, ++index) {
        str->c_str[index] = str1->c_str[i];
    }
    for (int i = 0; i < str2->length; ++i, ++index) {
        str->c_str[index] = str2->c_str[i];
    }
    str->c_str[len] = '\0';
    return str;
}

//获取子串
string_t *string_substring(string_t *src, int start, int end) {
    string_t *str = malloc(sizeof(string_t));
    str->c_str = malloc(sizeof(char) * (end - start + 1));
    int index = 0;
    for (int i = start; i < src->length && i < end; ++i) {
        str->c_str[index] = src->c_str[i];
    }
    str->c_str[index] = '\0';
    str->length = index;
    return str;
}

//检查是否包含子串
bool string_str(string_t *src, string_t *sub) {
    if (strstr(src->c_str, sub->c_str) == NULL) {
        return false;
    } else {
        return true;
    }
}

//比较字符串是否相等
bool string_equal(string_t *str1, string_t *str2) {
    if (str1->length != str2->length) {
        return false;
    }
    for (int i = 0; i < str1->length; i++) {
        if (str1->c_str[i] != str2->c_str[i]) {
            return false;
        }
    }
    return true;
}

//字符串复制
string_t *string_copy(string_t *src) {
    string_t *str = malloc(sizeof(string_t));
    str->c_str = malloc(sizeof(char) * (src->length + 1));
    for (int i = 0; i < src->length; i++) {
        str->c_str[i] = src->c_str[i];
    }
    return str;
}

//释放
void string_free(string_t *src) {
    if (src == NULL) {
        return;
    }
    free(src->c_str);
    free(src);
}