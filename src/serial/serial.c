#include "serial.h"
#include "../logger/logger.h"

//打开串口
serial_t serial_open(serial_option_t option) {
    int fd = open(option.name, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        logger_error(LOGGER("open serial:%s error!"), option.name);
        perror("");
        return -1;
    }
    if (fcntl(fd, F_SETFL, 0) < 0) {
        logger_error(LOGGER("serial:%s fcntl error!"), option.name);
        perror("");
        return -1;
    }
    struct termios opt;
    if (tcgetattr(fd, &opt) != 0) {
        logger_error(LOGGER("serial:%s setup error!"), option.name);
        perror("");
        return -1;
    }
    //设置波特率
    cfsetispeed(&opt, option.baud);
    cfsetospeed(&opt, option.baud);

    opt.c_cflag &= ~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_cflag &= ~CSIZE; //先清除数据位,再配置
    opt.c_cflag |= CS8; //设置数据位,CS5,CS6,CS7,CS8
    opt.c_cflag &= ~CSTOPB;//一个停止位
    opt.c_cflag &= ~CRTSCTS; //设置数据流控制, 不使用流控制
    opt.c_cflag &= ~PARENB; //禁止使用奇偶校验

    opt.c_iflag &= ~(ICRNL | INLCR);//修改输入模式
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);//修改输入模式

    opt.c_oflag &= ~(ONLCR | OCRNL); //修改输出模式
    opt.c_oflag &= ~OPOST; //修改输出模式，数据不经处理输出, 原始数据输出
    //ICANON 使用标准输入模式，ECHO回显输入字符，
    // ~(ECHOE|ISIG)在进行write写输 出的时候，不必以回车的键入作为输出的结束，直接发送输入值。
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_cc[VTIME] = 1; //设置等待时间, 读取一个字符等待0*(0/10)s
    opt.c_cc[VMIN] = 36; //设置最小接收字符,读取字符的最少个数为0
    tcflush(fd, TCIFLUSH); //如果发生数据溢出，接收数据，但是不再读取,刷新收到的数据但是不读

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd, TCSANOW, &opt) != 0) {
        logger_error(LOGGER("serial:%s set configure error!"), option.name);
        perror("");
        return -1;
    }
    return fd;
}

//从串口中读数据
size_t serial_read(serial_t serial, byte_t *dst, size_t size) {
    if (serial == -1) {
        return 0;
    }
    ssize_t len = read(serial, dst, size);
    if (len <= 0) {
        return 0;
    }
    return len;
}

//向串口写数据
size_t serial_write(serial_t serial, const byte_t *data, int offset, size_t size) {
    if (serial == -1) {
        return 0;
    }
    byte_t *buffer = malloc(sizeof(byte_t) * (size));
    unsigned long end = offset + size;
    for (size_t index = 0, i = offset; i < end; ++i, ++index) {
        buffer[index] = data[i];
    }
    ssize_t len = write(serial, buffer, size);
    free(buffer);
    if (len < size) {
        return 0;
    }
    return len;
}

//关闭串口
void serial_close(serial_t serial) {
    if (serial == -1) {
        return;
    }
    close(serial);
}