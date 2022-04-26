//
// Created by Hami Lemon on 2022/4/23.
//

#include "setting.h"
#include "logger/logger.h"

//4KB
const size_t buffer_size = 1024 << 2;

//读取设置
setting_t *read_setting() {
    camera_setting_t *default_camera_setting = malloc(sizeof(camera_setting_t));
    default_camera_setting->name = string_new("/dev/video0", -1);
    default_camera_setting->fps = 30;
    default_camera_setting->width = 1920;
    default_camera_setting->height = 1080;

    server_setting_t *default_server_setting = malloc(sizeof(server_setting_t));
    default_server_setting->ip = string_new("0.0.0.0", -1);
    default_server_setting->port = 8888;

    logger_setting_t *default_logger_setting = malloc(sizeof(logger_setting_t));
    default_logger_setting->fmt = string_new("%l %d %t %f:%n %m", -1);
    default_logger_setting->level = LOG_INFO;

    setting_t *default_setting = malloc(sizeof(setting_t));
    default_setting->zigbee = string_new("/dev/ttyUSB0", -1);
    default_setting->voice = string_new("/dev/ttyUSB1", -1);
    default_setting->server = default_server_setting;
    default_setting->camera = default_camera_setting;
    default_setting->logger = default_logger_setting;

    char buffer[buffer_size];
    int fd = open(SETTING_FILE, O_RDONLY);
    if (fd == -1) {
        printf("use default setting!\n");
        return default_setting;
    }

    size_t len = read(fd, buffer, buffer_size - 1);
    if (len <= 0) {
        printf("use default setting!\n");
        return default_setting;
    }

    cJSON *json = cJSON_ParseWithLength(buffer, len);
    cJSON *zigbee = cJSON_GetObjectItem(json, "zigbee");
    if (zigbee != NULL) {
        string_free(default_setting->zigbee);
        default_setting->zigbee = string_new(cJSON_GetStringValue(zigbee), -1);
    }
    cJSON *voice = cJSON_GetObjectItem(json, "voice");
    if (voice != NULL) {
        string_free(default_setting->voice);
        default_setting->voice = string_new(cJSON_GetStringValue(voice), -1);
    }

    cJSON *camera = cJSON_GetObjectItem(json, "camera");
    if (camera != NULL) {
        cJSON *camera_name = cJSON_GetObjectItem(camera, "name");
        cJSON *camera_width = cJSON_GetObjectItem(camera, "width");
        cJSON *camera_height = cJSON_GetObjectItem(camera, "height");
        cJSON *camera_fps = cJSON_GetObjectItem(camera, "fps");
        if (camera_name != NULL) {
            string_free(default_camera_setting->name);
            default_camera_setting->name = string_new(cJSON_GetStringValue(camera_name), -1);
        }
        if (camera_width != NULL) {
            default_camera_setting->width = (uint32_t) cJSON_GetNumberValue(camera_width);
        }
        if (camera_height != NULL) {
            default_camera_setting->height = (uint32_t) cJSON_GetNumberValue(camera_height);
        }
        if (camera_fps != NULL) {
            default_camera_setting->fps = (uint32_t) cJSON_GetNumberValue(camera_fps);
        }
    }

    cJSON *server = cJSON_GetObjectItem(json, "server");
    if (server != NULL) {
        cJSON *server_ip = cJSON_GetObjectItem(server, "ip");
        if (server_ip != NULL) {
            string_free(default_server_setting->ip);
            default_server_setting->ip = string_new(cJSON_GetStringValue(server_ip), -1);
        }
        cJSON *server_port = cJSON_GetObjectItem(server, "port");
        if (server_port != NULL) {
            default_server_setting->port = (int) cJSON_GetNumberValue(server_port);
        }
    }

    cJSON *logger = cJSON_GetObjectItem(json, "logger");
    if (logger != NULL) {
        cJSON *logger_fmt = cJSON_GetObjectItem(logger, "fmt");
        if (logger_fmt != NULL) {
            string_free(default_logger_setting->fmt);
            default_logger_setting->fmt = string_new(cJSON_GetStringValue(logger_fmt), -1);
        }
        cJSON *logger_level = cJSON_GetObjectItem(logger, "level");
        if (logger_level != NULL) {
            default_logger_setting->level = (int) cJSON_GetNumberValue(logger_level);
        }
    }

    cJSON_Delete(json);
    return default_setting;
}

void setting_print(setting_t *setting) {
    camera_setting_t *camera = setting->camera;
    server_setting_t *server = setting->server;
    logger_setting_t *logger = setting->logger;

    printf("setting:\n");
    printf("  zigbee:%s\n", setting->zigbee->c_str);
    printf("  voice:%s\n", setting->voice->c_str);

    printf("  camera:\n");
    printf("      name:%s\n", camera->name->c_str);
    printf("      width:%d\n", camera->width);
    printf("      height:%d\n", camera->height);
    printf("      fps:%d\n", camera->fps);

    printf("  server:\n");
    printf("      ip:%s\n", server->ip->c_str);
    printf("      port:%d\n", server->port);

    printf("  logger:\n");
    printf("    fmt:%s\n", logger->fmt->c_str);
    printf("    level:%d\n", logger->level);
}

void setting_free(setting_t *setting) {
    string_free(setting->camera->name);
    free(setting->camera);
    string_free(setting->server->ip);
    free(setting->server);
    string_free(setting->zigbee);
    free(setting);
}