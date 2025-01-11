// Code modified and inspired from example_main.c available on DTU learn
//

#pragma once

#include "driver/ledc.h"


//Line 43-54 in example_main.c
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO_RED      (4) // Define the output GPIO for red
#define LEDC_OUTPUT_IO_GREEN    (5) // Define the output GPIO for green
#define LEDC_OUTPUT_IO_BLUE     (6) // Define the output GPIO for blue
#define LEDC_CHANNEL_RED        LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN      LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE       LEDC_CHANNEL_2
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
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