// Code modified and inspired from example_main.c available on DTU learn
//

#pragma once

#include "driver/ledc.h"


//Line 43-54 in example_main.c
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL_RED        LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN      LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE       LEDC_CHANNEL_2
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (8192) // 100% Duty
#define LEDC_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz


typedef struct {
    int r_io_num;
    int g_io_num;
    int b_io_num;

    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel_red;
    ledc_channel_config_t ledc_channel_green;
    ledc_channel_config_t ledc_channel_blue;
} rgb_led_config_t;

esp_err_t init_rgb_led(rgb_led_config_t *config);
void rgb_set_color(rgb_led_config_t *config, uint8_t red, uint8_t green, uint8_t blue);