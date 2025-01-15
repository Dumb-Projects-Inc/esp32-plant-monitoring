#include "animations.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include "leaf_animation.h"
#include "settings_animation.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define OLED_ADDR 0x3C

void animations_init(ssd1306_handle_t *dev) {
    *dev = ssd1306_create(I2C_MASTER_NUM, OLED_ADDR);
    ssd1306_clear_screen(*dev, 0x00);
    ssd1306_refresh_gram(*dev);
}

void play_leaf_animation(ssd1306_handle_t dev) {
    leaf_animation_play(dev);
}

// Delay is plus minus 1 second
void play_settings_animation(ssd1306_handle_t dev, int delay_ms) {
    settings_animation_play(dev, delay_ms);
}