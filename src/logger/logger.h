//输出日志到控制台

#ifndef MANAGER_LOGGER_H
#define MANAGER_LOGGER_H
#define LOGGER(fmt) __FILE__, __LINE__, fmt

//日志级别枚举
typedef enum log_level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} log_level;

//设置日志级别，只能程序启动时设置
void set_logger_level(log_level level);

void set_logger_fmt(const char *fmt);

void logger_debug(const char *s_file, int s_line, const char *fmt, ...);

void logger_info(const char *s_file, int s_line, const char *fmt, ...);

void logger_warn(const char *s_file, int s_line, const char *fmt, ...);

void logger_error(const char *s_file, int s_line, const char *fmt, ...);

#endif //MANAGER_LOGGER_H
