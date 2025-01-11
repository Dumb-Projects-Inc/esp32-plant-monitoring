#pragma once
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#define BUZZER_TAG "Buzzer"

//PWM library for buzzer, adapted from ESP32-C3 Components Demo on DTU Learn Line 56-62 in example_main.c
#define BUZZ_TIMER              LEDC_TIMER_1
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_CHANNEL            LEDC_CHANNEL_4
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz



// Different Notes for songs
//C4 = 244Hz
#define NOTE_C4 244


typedef struct {
    int gpio_num;

    ledc_timer_config_t ledc_timer_buzz;
    ledc_channel_config_t ledc_channel_buzz;
} buzz_config_t;

esp_err_t init_buzzer(buzz_config_t *config);
void all(buzz_config_t *config);