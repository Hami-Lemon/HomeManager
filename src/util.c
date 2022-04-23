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

void log_info(const char *msg) {
    printf("[INFO] %s", msg);
}

void log_infoln(const char *msg) {
    printf("[INFO] %s\n", msg);
}

void log_perror(const char *msg) {
    fprintf(stderr, "[ERROR] %s", msg);
    perror("");
}

void log_error(const char *msg) {
    fprintf(stderr, "[ERROR] %s", msg);
}

void log_errorln(const char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);

}

void log_warn(const char *msg) {
    printf("[WARN] %s", msg);
}

void log_warnln(const char *msg) {
    printf("[WARN] %s\n", msg);
}

void info(const char *msg) {
    fprintf(stdout, "%s", msg);
}

void infoln(const char *msg) {
    fprintf(stdout, "%s\n", msg);
}

void error(const char *msg) {
    fprintf(stderr, "%s", msg);
}

void errorln(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

void warn(const char *msg) {
    fprintf(stdout, "%s", msg);
}

void warnln(const char *msg) {
    fprintf(stdout, "%s\n", msg);
}