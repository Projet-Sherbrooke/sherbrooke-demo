#include <ssd1306_i2c.h>
#include <signal.h>
#include "bmp280.h"

#define WIDTH  128
#define HEIGHT 32

void trap_sig(int signo) { /* Nothing to do. */ }

int main(int argc, char **argv) {
    const char *fname = "/dev/i2c-1";
    ssd1306_i2c_t *oled;
    ssd1306_framebuffer_t *fb;
    struct bmp280_device bmp280 = {
        .i2c_adapter = 1,
        .i2c_addr = BMP280_I2C_ADDR_PRIM,
        .mode = BMP280_FORCE,
        .temp_os = BMP280_OS_1X,
        .pres_os = BMP280_OS_1X,
    };    

    oled = ssd1306_i2c_open(fname, 0x3c, 128, 32, NULL);

    bmp280_init(&bmp280);
    bmp280_oneshot_read(&bmp280);

    if (ssd1306_i2c_display_initialize(oled) < 0) {
        fprintf(stderr, "Failed to initialize display\n");
        ssd1306_i2c_close(oled);
        exit(1);
    }

    ssd1306_i2c_display_clear(oled);

    printf("Initial temp value: %f\n", bmp280.temperature_val);

    int base_y = 20;
    int base_x = 1;
    int y = base_y;
    int x = base_x;
    //int min_temp = -37, max_temp = 85;

    fb = ssd1306_framebuffer_create(oled->width, oled->height, oled->err);

    while (true) {
        bmp280_oneshot_read(&bmp280);

        printf("Temp: %f C\n", bmp280.temperature_val);

        usleep(1000 * 500);

        x = x + 1;
        if (x == 128) {
            ssd1306_framebuffer_clear(fb);
            x = 1;
        }
        y = abs(32 - (int)bmp280.temperature_val);

        ssd1306_framebuffer_put_pixel_rotation(fb, x, y, 1, 0);

        ssd1306_i2c_display_update(oled, fb);
    }
}
