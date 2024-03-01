#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CMD_SELECT   (char)0x01
#define CMD_WRITE    (char)0x02
#define CMD_READ     (char)0x03

int open_i2c_device(int nb, int addr) {
    char buf[20];
    int fd;

    snprintf(buf, sizeof(buf) - 1, "/dev/i2c-%d", nb);

    if ((fd = open(buf, O_RDWR)) < 0) {
        perror("open");
        exit(1);
    }

    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        perror("ioctl");
        exit(1);
    }

    return fd;
}

int select_register(int fd, char reg) {
    char buf[2];
    int n = 0;

    buf[0] = CMD_SELECT;
    buf[1] = reg;

    if ((n = write(fd, buf, 2)) < 0)
        fprintf(stderr, "Failed CMD_SELECT\n");
}

void write_register(int fd, char val) {
    char buf[1];
    int n = 0;

    buf[0] = CMD_WRITE;
    buf[1] = val;

    if ((n = write(fd, buf, 2)) < 0)
        fprintf(stderr, "Failed CMD_WRITE\n");
}

char read_register(int fd) {
    char buf[1];
    int n = 0;

    buf[0] = CMD_READ;

    if ((n = write(fd, buf, 1)) < 0)
        fprintf(stderr, "Failed CMD_READ\n");

    if ((n = read(fd, buf, sizeof(buf))) < 0)
        fprintf(stderr, "Failed read\n");

    return buf[0];
}

int main() {
    int fd = open_i2c_device(1, 0x08);

    for (int i = 0; i < 10; i++) {
        select_register(fd, i);
        fprintf(stderr, "REG %d, value: %d\n", i, read_register(fd));
    }

    close(fd);
}

