#include "serial.h"
#include "../util.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

const int SERIAL_BUFFER_SIZE = 512;

//打开串口
SERIAL serial_open(SERIAL_OPTION option) {
    int fd = open(option.name, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open serial error!\n");
        return -1;
    }
    if (fcntl(fd, F_SETFL, 0) < 0) {
        perror("fcntl error!\n");
        return -1;
    }
    struct termios opt;
    if (tcgetattr(fd, &opt) != 0) {
        perror("setup serial error!\n");
        return -1;
    }
    //设置波特率
    cfsetispeed(&opt, option.baud);
    cfsetospeed(&opt, option.baud);

    opt.c_cflag |= CLOCAL; //修改控制模式，CLOCAL含义为忽略所有调制解调器的状态行, 保证程序不会占用串口
    opt.c_cflag |= CREAD; //修改控制模式，CREAD代表启用字符接收器, 使得能够从串口中读取输入数据
    opt.c_oflag &= ~(ONLCR | OCRNL); //修改输出模式
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);//修改输入模式
    opt.c_iflag &= ~(ICRNL | INLCR);//修改输入模式
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);//修改输入模式
    opt.c_cflag &= ~CRTSCTS; //设置数据流控制, 不使用流控制
    opt.c_cflag &= ~CSIZE; //先清除数据位,再配置
    opt.c_cflag |= CS8; //设置数据位,CS5,CS6,CS7,CS8
    opt.c_cflag &= ~PARENB; //禁止使用奇偶校验
    opt.c_iflag &= ~INPCK; //禁止输入奇偶校验

    opt.c_cflag &= ~CSTOPB; //设置停止位, 一位停止位
    opt.c_oflag &= ~OPOST; //修改输出模式，数据不经处理输出, 原始数据输出
    //ICANON 使用标准输入模式，ECHO回显输入字符，
    // ~(ECHOE|ISIG)在进行write写输 出的时候，不必以回车的键入作为输出的结束，直接发送输入值。
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE |
                     ISIG);
    opt.c_cc[VTIME] = 1; //设置等待时间, 读取一个字符等待0*(0/10)s
    opt.c_cc[VMIN] = 1; //设置最小接收字符,读取字符的最少个数为0
    tcflush(fd, TCIFLUSH); //如果发生数据溢出，接收数据，但是不再读取,刷新收到的数据但是不读

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd, TCSANOW, &opt) != 0) {
        perror("serial_Set function->com set Error");
        return -1;
    }
    return fd;
}

//从串口中读数据
size_t serial_read(SERIAL serial, char *dst, size_t size) {
    if (serial == -1) {
        return 0;
    }
    size_t len = read(serial, dst, size);
    if (len <= 0) {
        return 0;
    }
    return len;
}

//向串口写数据
size_t serial_write(SERIAL serial, char *data, int offset, size_t size) {
    if (serial == -1) {
        return 0;
    }
    char *buffer = malloc(sizeof(char) * (size));
    unsigned long end = offset + size;
    data_copy(buffer, data, offset, (int) end);
    size_t len = write(serial, buffer, size);
    free(buffer);
    if (len < size) {
        return 0;
    }
    return len;
}

//关闭串口
void serial_close(SERIAL serial) {
    if (serial == -1) {
        return;
    }
    close(serial);
}