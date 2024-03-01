#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define CMD_SELECT   (char)0x01
#define CMD_WRITE    (char)0x02
#define CMD_READ     (char)0x03

uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;

#define NB_RETRY     10
#define ERR_DELAY    1000 * 100

int i2c_writebuf(char *buf, size_t n) {
    int r = 0, retries = NB_RETRY;

    do {
        if (r) {
            fprintf(stderr, "i2c_writebuf failed: %d, retries: %d\n", r, retries);
            usleep(ERR_DELAY);
        }

        r = bcm2835_i2c_write(buf, n);

    } while (!(r == BCM2835_I2C_REASON_OK || --retries == 0));

    return r == BCM2835_I2C_REASON_OK;
}

int i2c_readbuf(char *buf, size_t n) {
    int r = 0, retries = NB_RETRY;

    do {
        if (r) {
            fprintf(stderr, "i2c_writebuf failed: %d, retries: %d\n", r, retries);
            usleep(ERR_DELAY);
        }

        r = bcm2835_i2c_read(buf, n);

    } while (!(r == BCM2835_I2C_REASON_OK || --retries == 0));

    return r == BCM2835_I2C_REASON_OK;
}

int select_register(char reg) {
    char buf[2];

    buf[0] = CMD_SELECT;
    buf[1] = reg;

    if (!i2c_writebuf(buf, sizeof(buf)))
        fprintf(stderr, "Failed CMD_SELECT\n");
}

void write_register(int fd, char val) {
    char buf[2];

    buf[0] = CMD_WRITE;
    buf[1] = val;

    if (!i2c_writebuf(buf, sizeof(buf)))
        fprintf(stderr, "Failed CMD_WRITE\n");
}

char read_register() {
    char buf[1];

    buf[0] = CMD_READ;

    if (!i2c_writebuf(buf, sizeof(buf)))
        fprintf(stderr, "Failed CMD_READ\n");

    if (!i2c_readbuf(buf, sizeof(buf)) < 0)
        fprintf(stderr, "Failed read\n");

    return buf[0];
}

int main() {
    if (!bcm2835_init()) {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }

    if (!bcm2835_i2c_begin()) {
      printf("bcm2835_i2c_begin failed. Are you running as root??\n");
      return 1;
    }

    bcm2835_i2c_setSlaveAddress(0x08);
    bcm2835_i2c_setClockDivider(clk_div);

    for (int i = 0; i < 10; i++) {
        select_register(i);
        fprintf(stderr, "REG %d, value: %d\n", i, read_register());
    }

    bcm2835_i2c_end();
    bcm2835_close();
}

