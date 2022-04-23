//
// Created by Hami Lemon on 2022/4/23.
//

#include "setting.h"
#include "util.h"

//4KB
const size_t buffer_size = 1024 << 2;

char *copy_str(const char *src) {
    unsigned long len = strlen(src);
    char *dst = malloc(sizeof(char) * len);
    strcpy(dst, src);
    return dst;
}

//读取设置
setting_t *read_setting() {
    camera_setting_t *default_camera_setting = malloc(sizeof(camera_setting_t));
    default_camera_setting->name = copy_str("/dev/video0");
    default_camera_setting->fps = 30;
    default_camera_setting->width = 1920;
    default_camera_setting->height = 1080;

    server_setting_t *default_server_setting = malloc(sizeof(server_setting_t));
    default_server_setting->ip = copy_str("0.0.0.0");
    default_server_setting->port = 8888;

    setting_t *default_setting = malloc(sizeof(setting_t));
    default_setting->zigbee = copy_str("/dev/ttyUSB0");
    default_setting->server = default_server_setting;
    default_setting->camera = default_camera_setting;

    char buffer[buffer_size];
    int fd = open(SETTING_FILE, O_RDONLY);
    if (fd == -1) {
        log_warnln("use default setting");
        return default_setting;
    }

    size_t len = read(fd, buffer, buffer_size - 1);
    if (len <= 0) {
        log_warnln("use default setting");
        return default_setting;
    }

    cJSON *json = cJSON_ParseWithLength(buffer, len);
    cJSON *zigbee = cJSON_GetObjectItem(json, "zigbee");
    if (zigbee != NULL) {
        default_setting->zigbee = copy_str(cJSON_GetStringValue(zigbee));
    }

    cJSON *camera = cJSON_GetObjectItem(json, "camera");
    if (camera != NULL) {
        cJSON *camera_name = cJSON_GetObjectItem(camera, "name");
        cJSON *camera_width = cJSON_GetObjectItem(camera, "width");
        cJSON *camera_height = cJSON_GetObjectItem(camera, "height");
        cJSON *camera_fps = cJSON_GetObjectItem(camera, "fps");
        if (camera_name != NULL) {
            default_camera_setting->name = copy_str(cJSON_GetStringValue(camera_name));
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
            default_server_setting->ip = copy_str(cJSON_GetStringValue(server_ip));
        }
        cJSON *server_port = cJSON_GetObjectItem(server, "port");
        if (server_port != NULL) {
            default_server_setting->port = (int) cJSON_GetNumberValue(server_port);
        }
    }

    default_setting->server = default_server_setting;
    default_setting->camera = default_camera_setting;

    cJSON_Delete(json);
    return default_setting;
}

void setting_print(setting_t *setting) {
    camera_setting_t *camera = setting->camera;
    server_setting_t *server = setting->server;

    printf("setting:\n");
    printf("  zigbee:%s\n", setting->zigbee);

    printf("  camera:\n");
    printf("      name:%s\n", camera->name);
    printf("      width:%d\n", camera->width);
    printf("      height:%d\n", camera->height);
    printf("      fps:%d\n", camera->fps);

    printf("  server:\n");
    printf("      ip:%s\n", server->ip);
    printf("      port:%d\n", server->port);
}

void setting_free(setting_t *setting) {
    free(setting->camera->name);
    free(setting->server->ip);
    free(setting->camera);
    free(setting->server);
    free(setting->zigbee);
    free(setting);
}