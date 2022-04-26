//
// Created by Hami Lemon on 2022/4/23.
//

#include "camera.h"
#include "../logger/logger.h"
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

//查看驱动能力
bool check_capability(int fd) {
    struct v4l2_capability capability;
    int ret = ioctl(fd, VIDIOC_QUERYCAP, &capability);
    if (ret == -1) {
        logger_error(LOGGER("check camera capability error!"));
        return false;
    }
    //判断是否支持视频采集
    if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        logger_error(LOGGER("device can not support V4L2_CAP_VIDEO_CAPTURE"));
        return false;
    }
    /*判断设备是否支持视频流采集*/
    if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
        logger_error(LOGGER("device can not support V4L2_CAP_STREAMING"));
        return false;
    }

    struct v4l2_cropcap crop_cap;
    memset(&crop_cap, 0, sizeof(struct v4l2_cropcap));
    crop_cap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_CROPCAP, &crop_cap) == 0) {
        struct v4l2_crop crop;
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = crop_cap.defrect;
        ioctl(fd, VIDIOC_S_CROP, &crop);
    }
    return true;
}

//连接摄像头
camera_t *camera_attach(const char *device) {
    int fd = open(device, O_RDWR);
    if (fd == -1) {
        logger_error(LOGGER("open camera:%s error!"), device);
        perror("");
        return NULL;
    }
    if (!check_capability(fd)) {
        return NULL;
    }
    camera_t *camera = malloc(sizeof(camera_t));
    camera->fd = fd;
    camera->option.height = 0;
    camera->option.width = 0;
    camera->buffer = NULL;
    camera->buffer_count = 0;
    camera->curr.start = NULL;
    camera->curr.length = 0;
    camera->capturing = false;
    return camera;
}

void camera_format_id_to_name(uint32_t id, char *name) {
    name[0] = (char) (id & 0xff);
    name[1] = (char) ((id >> 8) & 0xff);
    name[2] = (char) ((id >> 16) & 0xff);
    name[3] = (char) ((id >> 24) & 0xff);
    name[4] = '\0';
}

//设置摄像头控制参数
void camera_control(camera_t *camera) {
    struct v4l2_control control;
    memset(&control, 0, sizeof(struct v4l2_control));
    //设置色温
    control.id = V4L2_CID_FOCUS_AUTO;
    control.value = 1;
    if (ioctl(camera->fd, VIDIOC_S_CTRL, &control) == 1) {
        logger_info(LOGGER("set auto fail"));
    }
    control.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
    control.value = 5100;
    if (ioctl(camera->fd, VIDIOC_S_CTRL, &control) == 1) {
        logger_info(LOGGER("set white balance temperature fail"));
    }
    //设置对比度
    control.id = V4L2_CID_CONTRAST;
    control.value = 80;
    if (ioctl(camera->fd, VIDIOC_S_CTRL, &control) == -1) {
        logger_info(LOGGER("set contrast fail"));
    }
    //设置饱和度
    control.id = V4L2_CID_SATURATION;
    control.value = 80;
    if (ioctl(camera->fd, VIDIOC_S_CTRL, &control) == -1) {
        logger_info(LOGGER("set saturation fail"));
    }
}

//配置摄像头
bool camera_config(camera_t *camera, camera_option_t option) {
    if (camera == NULL) {
        logger_error(LOGGER("camera is NULL"));
        return false;
    }
    int fd = camera->fd;
    char formatBuf[128], fmat[64];
    struct v4l2_fmtdesc fmtdesc; //V4L2的视频格式描述符类型
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    formatBuf[0] = '\0';
    //获取当前驱动支持的视频格式
    while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
        sprintf(fmat, "    %d.%s\n", fmtdesc.index + 1, fmtdesc.description);
        strcat(formatBuf, fmat);
        fmtdesc.index++;
    }
    logger_info(LOGGER("camera support format:\n%s"), formatBuf);
    //摄像头配置信息格式
    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; //数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
    //读取当前驱动的频捕获格式
    if (ioctl(fd, VIDIOC_G_FMT, &format) == -1) {
        logger_error(LOGGER("get camera format error!"));
        perror("");
        return false;
    }
    //设置宽高
    format.fmt.pix.width = option.width; //宽
    format.fmt.pix.height = option.height; //高
    format.fmt.pix.field = V4L2_FIELD_ANY; //逐行扫描/隔行扫描设置,V4L2_FIELD_ANY指由硬件自行选择
    char format_name[5];
    camera_format_id_to_name(format.fmt.pix.pixelformat, format_name);
    logger_info(LOGGER("default pixel format is:%s"), format_name);
    //设置视频捕获格式MJPEG
    if (strstr(formatBuf, "Motion-JPEG") != NULL) {
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;//mjpeg
        logger_info(LOGGER("set camera picture format MJPEG"));
    } else {
        logger_error(LOGGER("camera not support format MJPEG"));
        return false;
    }

    //设置当前驱动的格式
    if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
        logger_error(LOGGER("camera set format error!"));
        perror("");
        return false;
    }
    //获取实际设置的格式（因为可能会设置失败）
    if (ioctl(fd, VIDIOC_G_FMT, &format) == -1) {
        logger_error(LOGGER("get camera format error!"));
        perror("");
        return false;
    }
    camera_format_id_to_name(format.fmt.pix.pixelformat, format_name);
    logger_info(LOGGER("actual pixel format is:%s"), format_name);

    struct v4l2_streamparm parm;
    memset(&parm, 0, sizeof(parm));
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (option.fps != 0) {
        parm.parm.capture.timeperframe.numerator = 1;
        //设置帧率
        parm.parm.capture.timeperframe.denominator = option.fps;
        if (ioctl(fd, VIDIOC_S_PARM, &parm) == -1) {
            logger_error(LOGGER("camera set stream parm error!"));
            perror("");
            return false;
        }
    }
    //获取实际的帧率
    if (ioctl(fd, VIDIOC_G_PARM, &parm) == -1) {
        logger_error(LOGGER("camera get stream parm error!"));
        perror("");
        return false;
    }
    camera->option.width = format.fmt.pix.width;
    camera->option.height = format.fmt.pix.height;
    camera->option.fps = parm.parm.capture.timeperframe.denominator;

    camera_control(camera);
    return true;
}

//申请缓存
bool camera_buffer_allocate(camera_t *camera) {
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.count = CAMERA_BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(camera->fd, VIDIOC_REQBUFS, &req) == -1) {
        logger_error(LOGGER("camera: allocate buffer fail"));
        perror("");
        return false;
    }
    camera->buffer_count = req.count;
    camera->buffer = calloc(req.count, sizeof(camera_buffer_t));
    size_t max_buffer_len = 0;
    for (int i = 0; i < camera->buffer_count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (ioctl(camera->fd, VIDIOC_QUERYBUF, &buf) == -1) {
            logger_error(LOGGER("camera: query buffer fail"));
            perror("");
            return false;
        }
        if (buf.length > max_buffer_len) {
            max_buffer_len = buf.length;
        }
        camera->buffer[i].length = buf.length;
        camera->buffer[i].start =
                mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                     camera->fd, buf.m.offset);
        if (camera->buffer[i].start == MAP_FAILED) {
            logger_error(LOGGER("camera: mmap buffer fail"));
            perror("");
            return false;
        }
    }
    camera->curr.start = calloc(max_buffer_len, sizeof(uint8_t));
    return true;
}

//释放缓存
void camera_buffer_release(camera_t *camera) {
    for (size_t i = 0; i < camera->buffer_count; i++) {
        camera_buffer_t buf = camera->buffer[i];
        if (buf.start == NULL) {
            continue;
        }
        munmap(buf.start, buf.length);
    }
    free(camera->buffer);
    camera->buffer_count = 0;
    camera->buffer = NULL;
    free(camera->curr.start);
    camera->curr.start = NULL;
    camera->curr.length = 0;
}

//开始采集
bool camera_start(camera_t *camera) {
    if (camera->buffer_count == 0) {
        //申请缓存空间
        if (!camera_buffer_allocate(camera)) {
            logger_error(LOGGER("camera allocate buffer fail"));
            camera_buffer_release(camera);
            return false;
        }
    }
    //已经在采集中
    if (camera->capturing) {
        logger_debug(LOGGER("camera: already capturing"));
        return true;
    }
    logger_debug(LOGGER("allocate buffer count: %ld"), camera->buffer_count);
    for (size_t i = 0; i < camera->buffer_count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (ioctl(camera->fd, VIDIOC_QBUF, &buf) == -1) {
            logger_error(LOGGER("camera: QBuf fail"));
            perror("");
            return false;
        }
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(camera->fd, VIDIOC_STREAMON, &type) == -1) {
        logger_error(LOGGER("camera: stream on fail"));
        perror("");
        return false;
    }
    //刚开始采集时，前几张图片会有错误，先处理掉
    int times = 5;
    while (times--) {
//        camera_capture(camera);
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        //出队
        if (ioctl(camera->fd, VIDIOC_DQBUF, &buf) == -1) {
            break;
        }
        //入队
        if (ioctl(camera->fd, VIDIOC_QBUF, &buf) == -1) {
            break;
        }
    }
    camera->capturing = true;
    return true;
}
//停止采集
bool camera_stop(camera_t *camera) {
    //未开始采集
    if (!camera->capturing) {
        logger_debug(LOGGER("camera: do not start capture"));
        return true;
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(camera->fd, VIDIOC_STREAMOFF, &type) == -1) {
        logger_error(LOGGER("camera: stream off fail"));
        perror("");
        return false;
    }

    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    for (size_t i = 0; i < camera->buffer_count; i++) {
        ioctl(camera->fd, VIDIOC_DQBUF, &buf);
    }
    camera_buffer_release(camera);
    camera->capturing = false;
    return true;
}

//断开摄像头连接
void camera_detach(camera_t *camera) {
    if (camera != NULL) {
        if (camera->curr.start != NULL) {
            //停止采集
            camera_stop(camera);
        }
        close(camera->fd);
        free(camera);
    }
}

//捕获一张图片
bool camera_capture(camera_t *camera) {
    if (camera->curr.start == NULL) {
        logger_warn(LOGGER("camera curr start is NULL!"));
        return false;
    }

    fd_set fds;
    struct timeval timeout;
    FD_ZERO(&fds);
    FD_SET(camera->fd, &fds);
    timeout.tv_sec = 2; //2秒超时
    timeout.tv_usec = 0;
    int ret = select(camera->fd + 1, &fds, NULL, NULL, &timeout);
    if (ret == -1) {
        logger_error(LOGGER("camera:capture picture fail"));
        return false;
    } else if (ret == 0) {
        logger_error(LOGGER("camera:capture picture, timeout!"));
        return false;
    }
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    //出队
    if (ioctl(camera->fd, VIDIOC_DQBUF, &buf) == -1) {
        logger_error(LOGGER("camera: dequeue buffer fail\n"));
        return false;
    }
    //拷贝捕获的数据
    memcpy(camera->curr.start, camera->buffer[buf.index].start, buf.bytesused);
    camera->curr.length = buf.bytesused;
    //入队
    if (ioctl(camera->fd, VIDIOC_QBUF, &buf) == -1) {
        logger_error(LOGGER("camera: queue buffer fail"));
        perror("");
        return false;
    }
    if (camera->curr.length <= 0) {
        return false;
    }
    return true;
}
