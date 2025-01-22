#include "animations.h"

//#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define OLED_ADDR 0x3C

void animations_init(ssd1306_handle_t *dev) {
    *dev = ssd1306_create(I2C_MASTER_NUM, OLED_ADDR);
    ssd1306_clear_screen(*dev, 0x00);
    ssd1306_refresh_gram(*dev);
}

