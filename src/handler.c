//
// Created by Hami Lemon on 2022/4/24.
//

#include "handler.h"
#include "logger/logger.h"
#include "zigbee/zigbee.h"
#include "camera/camera.h"

//将src中[0, len)中的数据追加到dst中，从offset开始
void data_append(byte_t *dst, const byte_t *src, size_t offset, size_t len) {
    for (size_t i = 0; i < len; ++i, offset++) {
        dst[offset] = src[i];
    }
}

void zigbee_handler(tcp_connection_t conn, void *device, const byte_t *buffer) {
    byte_t operation = buffer[1];
    zigbee_t *zigbee = (zigbee_t *) device;
    if (operation == OPERATION_ZIGBEE_INFO) {
        //获取环境信息
        logger_debug(LOGGER("read info from zigbee"));
        byte_t zigbee_buf[ZIGBEE_BUFFER_SIZE];
        zigbee_buf[0] = 0;
        int len = zigbee_read(zigbee, zigbee_buf);
        if (len == 0) {
            logger_warn(LOGGER("read info from zigbee fail"));
        } else {
            size_t wn = tcp_connection_write(conn, zigbee_buf, 0, len);
            logger_debug(LOGGER("read %d bytes from zigbee, send %d bytes to conn"),
                         len, wn);
            if (wn < len) {
                logger_warn(LOGGER("read zigbee info: send bytes less than get bytes!"));
            }
        }
    } else {
        logger_debug(LOGGER("send operation:%02X to zigbee"), operation);
        zigbee_operation(zigbee, operation);
    }
}

void camera_handler(tcp_connection_t conn, void *device, const byte_t *buffer) {
    camera_t *camera = (camera_t *) device;
    byte_t operation = buffer[1];
    if (operation == OPERATION_CAMERA_START) {
        if (!camera_start(camera)) {
            logger_error(LOGGER("camera start capture fail"));
        }
    } else if (operation == OPERATION_CAMERA_CAPTURE) {
        if (!camera_capture(camera)) {
            logger_error(LOGGER("capture picture fail"));
            return;
        }
        size_t size = camera->curr.length + 2;
        byte_t *data = malloc(sizeof(byte_t) * size);
        data[0] = DEVICE_CAMERA;
        data[1] = OPERATION_CAMERA_CAPTURE;
        data_append(data, camera->curr.start, 2, camera->curr.length);
        size_t len = tcp_connection_write(conn, data, 0, size);
        if (len <= 0) {
            logger_warn(LOGGER("send data fail"));
        }
        free(data);
    } else if (operation == OPERATION_CAMERA_STOP) {
        if (!camera_stop(camera)) {
            logger_warn(LOGGER("camera stop capture fail"));
        }
    }
}

void voice_handler(tcp_connection_t conn, void *device, const byte_t *buffer) {

}