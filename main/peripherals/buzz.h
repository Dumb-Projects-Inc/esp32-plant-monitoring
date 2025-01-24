#pragma once
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

static const char *BUZZER_TAG = "Buzzer";

//PWM library for buzzer, adapted from ESP32-C3 Components Demo on DTU Learn Line 56-62 in example_main.c
#define BUZZ_TIMER              LEDC_TIMER_1
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_CHANNEL            LEDC_CHANNEL_4
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 1 kHz


typedef struct {
    int gpio_num;

    ledc_timer_config_t ledc_timer_buzz;
    ledc_channel_config_t ledc_channel_buzz;
} buzz_config_t;

esp_err_t init_buzzer(buzz_config_t *config);
void all();
void play_note(int hz, int period);
void pause_note(int period);
void play_song(const int *notes);

void song_task(void *pvParameters);

static const int rondo[] = {1262, 100, 0, 428, 493, 107, 20, 440, 107, 20, 415, 107, 20, 440, 108, 20, 339, 107, 127, 293, 107, 127, 369, 107, 20, 523, 107, 20, 349, 108, 20, 523, 107, 20, 380, 107, 127, 293, 107, 127, 391, 107, 20, 659, 108, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 466, 108, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 479, 107, 127, 293, 107, 128, 423, 107, 127, 448, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 73, 479, 108, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 74, 479, 107, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 399, 107, 127, 484, 107, 128, 329, 107, 341, 493, 107, 20, 440, 107, 20, 415, 108, 20, 440, 107, 20, 339, 107, 127, 293, 107, 127, 369, 107, 20, 523, 108, 20, 349, 107, 20, 523, 107, 20, 380, 107, 127, 293, 107, 127, 391, 108, 20, 659, 107, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 108, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 479, 107, 127, 293, 108, 127, 423, 107, 127, 448, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 74, 479, 107, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 452, 107, 127, 479, 107, 128, 359, 53, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 399, 107, 127, 484, 108, 127, 329, 107, 341, 587, 107, 128, 640, 107, 127, 407, 107, 127, 513, 107, 127, 380, 108, 20, 783, 107, 20, 479, 107, 20, 659, 107, 20, 384, 107, 127, 391, 107, 128, 587, 107, 127, 640, 107, 127, 407, 107, 127, 513, 108, 127, 380, 107, 20, 783, 107, 20, 479, 107, 20, 659, 107, 20, 384, 107, 342, 479, 107, 127, 538, 107, 127, 336, 108, 127, 423, 107, 127, 302, 107, 20, 659, 107, 20, 391, 107, 20, 523, 108, 20, 323, 107, 127, 329, 107, 127, 479, 107, 127, 538, 108, 127, 336, 107, 127, 423, 107, 127, 302, 107, 20, 659, 108, 20, 391, 107, 20, 523, 107, 20, 323, 107, 341, 493, 108, 20, 440, 107, 20, 415, 107, 20, 440, 107, 20, 339, 107, 127, 293, 107, 128, 369, 107, 20, 523, 107, 20, 349, 107, 20, 523, 107, 20, 380, 107, 127, 293, 108, 127, 391, 107, 20, 659, 107, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 108, 20, 415, 107, 20, 880, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 427, 108, 127, 261, 107, 127, 391, 107, 127, 407, 107, 128, 415, 214, 234, 359, 107, 127, 329, 108, 127, 339, 107, 127, 288, 107, 127, 320, 107, 128, 293, 107, 127, 266, 107, 127, 190, 107, 127, 256, 54, 20, 261, 54, 20, 493, 53, 74, 246, 53, 20, 256, 54, 127, 219, 54, 20, 155, 53, 342, 622, 107, 127, 698, 107, 127, 407, 27, 20, 439, 27, 20, 466, 27, 20, 513, 26, 127, 220, 108, 127, 439, 107, 127, 475, 107, 127, 407, 27, 20, 439, 27, 20, 466, 27, 20, 513, 26, 128, 475, 107, 127, 439, 107, 127, 423, 107, 127, 271, 27, 20, 293, 27, 20, 311, 27, 20, 342, 27, 127, 369, 107, 127, 311, 27, 20, 329, 27, 20, 349, 26, 20, 391, 27, 127, 423, 107, 128, 305, 26, 20, 329, 27, 20, 349, 27, 20, 384, 27, 127, 329, 107, 127, 399, 107, 127, 431, 108, 127, 407, 27, 20, 439, 26, 20, 466, 27, 20, 513, 27, 127, 220, 107, 127, 439, 107, 128, 475, 107, 127, 407, 27, 20, 439, 26, 20, 466, 27, 20, 513, 27, 127, 475, 107, 127, 439, 108, 127, 423, 107, 127, 271, 27, 20, 293, 27, 20, 311, 26, 20, 342, 27, 127, 415, 107, 128, 305, 26, 20, 329, 27, 20, 349, 27, 20, 384, 27, 127, 329, 107, 127, 349, 107, 342, 622, 107, 127, 698, 107, 127, 407, 27, 20, 439, 27, 20, 466, 27, 20, 513, 26, 128, 220, 107, 127, 439, 107, 127, 475, 107, 127, 407, 27, 20, 439, 27, 20, 466, 27, 20, 513, 27, 127, 475, 107, 127, 439, 107, 127, 423, 107, 128, 271, 26, 20, 293, 27, 20, 311, 27, 20, 342, 27, 127, 369, 107, 127, 311, 27, 20, 329, 27, 20, 349, 27, 20, 391, 26, 127, 423, 108, 127, 305, 27, 20, 329, 26, 20, 349, 27, 20, 384, 27, 127, 329, 107, 127, 399, 107, 128, 431, 107, 127, 407, 27, 20, 439, 26, 20, 466, 27, 20, 513, 27, 127, 220, 107, 127, 439, 108, 127, 475, 107, 127, 407, 27, 20, 439, 27, 20, 466, 26, 20, 513, 27, 127, 475, 107, 128, 439, 107, 127, 423, 107, 127, 271, 27, 20, 293, 27, 20, 311, 27, 20, 342, 26, 127, 415, 108, 127, 305, 27, 20, 329, 26, 20, 349, 27, 20, 384, 27, 127, 329, 107, 127, 349, 107, 342, 1108, 107, 20, 1174, 107, 20, 1108, 107, 20, 987, 107, 20, 403, 108, 20, 987, 107, 20, 377, 107, 20, 830, 107, 20, 356, 107, 20, 880, 107, 20, 369, 107, 20, 739, 108, 20, 380, 107, 20, 739, 107, 20, 384, 107, 20, 698, 107, 20, 336, 107, 20, 622, 107, 20, 362, 108, 20, 554, 107, 20, 369, 107, 20, 698, 107, 20, 356, 107, 20, 830, 107, 20, 377, 107, 20, 830, 108, 20, 377, 107, 20, 987, 107, 20, 440, 107, 20, 1046, 107, 20, 399, 107, 20, 1046, 107, 20, 399, 108, 20, 1174, 107, 20, 399, 107, 20, 987, 107, 20, 403, 107, 20, 987, 107, 20, 377, 107, 20, 830, 108, 20, 356, 107, 20, 880, 107, 20, 369, 107, 20, 739, 107, 20, 369, 107, 20, 739, 107, 20, 423, 108, 20, 659, 107, 20, 369, 107, 20, 622, 107, 20, 391, 107, 20, 554, 107, 20, 359, 107, 20, 659, 108, 20, 439, 107, 20, 622, 107, 20, 391, 107, 20, 554, 107, 20, 415, 107, 20, 523, 107, 20, 369, 108, 341, 1108, 107, 20, 1174, 107, 20, 1108, 107, 20, 987, 108, 20, 403, 107, 20, 987, 107, 20, 377, 107, 20, 830, 107, 20, 356, 107, 20, 880, 107, 20, 369, 108, 20, 739, 107, 20, 380, 107, 20, 739, 107, 20, 384, 107, 20, 698, 107, 20, 336, 107, 20, 622, 108, 20, 362, 107, 20, 554, 107, 20, 369, 107, 20, 698, 107, 20, 356, 107, 20, 830, 107, 20, 377, 108, 20, 830, 107, 20, 377, 107, 20, 987, 107, 20, 440, 107, 20, 1046, 107, 20, 399, 107, 20, 1046, 108, 20, 399, 107, 20, 1174, 107, 20, 399, 107, 20, 987, 107, 20, 403, 107, 20, 987, 107, 20, 377, 108, 20, 830, 107, 20, 356, 107, 20, 880, 107, 20, 369, 107, 20, 739, 107, 20, 369, 107, 20, 739, 108, 20, 423, 107, 20, 659, 107, 20, 369, 107, 20, 622, 107, 20, 391, 107, 20, 554, 107, 20, 359, 108, 20, 659, 107, 20, 439, 107, 20, 622, 107, 20, 391, 107, 20, 554, 107, 20, 415, 107, 20, 523, 108, 20, 369, 107, 341, 659, 107, 20, 587, 107, 20, 554, 108, 20, 493, 107, 20, 311, 107, 20, 493, 107, 20, 369, 107, 20, 587, 107, 20, 391, 107, 20, 739, 108, 20, 423, 107, 20, 880, 107, 20, 466, 107, 20, 830, 107, 20, 415, 107, 20, 659, 107, 20, 369, 108, 20, 587, 107, 20, 377, 107, 20, 493, 107, 20, 311, 107, 20, 493, 107, 20, 369, 107, 20, 587, 108, 20, 391, 107, 20, 739, 107, 20, 423, 107, 20, 880, 107, 20, 391, 215, 234, 342, 107, 20, 587, 107, 20, 323, 107, 20, 493, 107, 20, 311, 107, 20, 493, 108, 20, 369, 107, 20, 587, 107, 20, 391, 107, 20, 739, 107, 20, 423, 107, 20, 880, 107, 20, 466, 108, 20, 830, 107, 20, 415, 107, 20, 659, 107, 20, 369, 107, 20, 587, 107, 20, 377, 107, 20, 493, 108, 20, 349, 107, 20, 659, 107, 20, 285, 107, 20, 554, 107, 20, 269, 107, 20, 587, 107, 20, 261, 108, 20, 493, 107, 20, 220, 214, 234, 1108, 107, 20, 1174, 108, 20, 1108, 107, 20, 987, 107, 20, 403, 107, 20, 987, 107, 20, 377, 107, 20, 830, 107, 20, 356, 108, 20, 880, 107, 20, 369, 107, 20, 739, 107, 20, 380, 107, 20, 739, 107, 20, 384, 107, 20, 698, 108, 20, 336, 107, 20, 622, 107, 20, 362, 107, 20, 554, 107, 20, 369, 107, 20, 698, 107, 20, 356, 108, 20, 830, 107, 20, 377, 107, 20, 830, 107, 20, 377, 107, 20, 987, 107, 20, 391, 107, 20, 1046, 108, 20, 399, 107, 20, 1046, 107, 20, 391, 107, 20, 1046, 107, 20, 384, 107, 20, 932, 107, 20, 380, 108, 20, 1108, 107, 20, 377, 107, 20, 1108, 107, 20, 377, 107, 20, 1108, 107, 20, 377, 107, 20, 1108, 108, 20, 380, 107, 20, 1108, 107, 20, 369, 107, 20, 880, 107, 20, 349, 107, 20, 880, 107, 20, 369, 108, 20, 830, 107, 20, 349, 107, 20, 987, 107, 20, 356, 107, 20, 739, 107, 20, 311, 107, 20, 739, 108, 20, 349, 107, 20, 698, 107, 20, 311, 107, 341, 659, 108, 20, 587, 107, 20, 554, 107, 20, 493, 107, 20, 311, 107, 20, 493, 107, 20, 369, 107, 20, 587, 108, 20, 391, 107, 20, 739, 107, 20, 423, 107, 20, 880, 107, 20, 466, 107, 20, 830, 107, 20, 415, 108, 20, 659, 107, 20, 369, 107, 20, 587, 107, 20, 377, 107, 20, 493, 107, 20, 311, 107, 20, 493, 108, 20, 369, 107, 20, 587, 107, 20, 391, 107, 20, 739, 107, 20, 423, 107, 20, 880, 107, 20, 391, 215, 234, 342, 107, 20, 587, 107, 20, 323, 107, 20, 493, 108, 20, 311, 107, 20, 493, 107, 20, 369, 107, 20, 587, 107, 20, 391, 107, 20, 739, 107, 20, 423, 108, 20, 880, 107, 20, 466, 107, 20, 830, 107, 20, 415, 107, 20, 659, 107, 20, 369, 107, 20, 587, 108, 20, 377, 107, 20, 493, 107, 20, 349, 107, 20, 659, 107, 20, 285, 107, 20, 554, 107, 20, 269, 108, 20, 587, 107, 20, 261, 107, 20, 493, 107, 20, 220, 214, 235, 1108, 107, 20, 1174, 107, 20, 1108, 107, 20, 987, 107, 20, 403, 107, 20, 987, 107, 20, 377, 108, 20, 830, 107, 20, 356, 107, 20, 880, 107, 20, 369, 107, 20, 739, 107, 20, 380, 107, 20, 739, 108, 20, 384, 107, 20, 698, 107, 20, 336, 107, 20, 622, 107, 20, 362, 107, 20, 554, 107, 20, 369, 108, 20, 698, 107, 20, 356, 107, 20, 830, 107, 20, 377, 107, 20, 830, 107, 20, 377, 107, 20, 987, 108, 20, 391, 107, 20, 1046, 107, 20, 399, 107, 20, 1046, 107, 20, 391, 107, 20, 1046, 107, 20, 384, 108, 20, 932, 107, 20, 380, 107, 20, 1108, 107, 20, 377, 107, 20, 1108, 107, 20, 377, 107, 20, 1108, 108, 20, 377, 107, 20, 1108, 107, 20, 380, 107, 20, 1108, 107, 20, 369, 107, 20, 880, 107, 20, 349, 108, 20, 880, 107, 20, 369, 107, 20, 830, 107, 20, 349, 107, 20, 987, 107, 20, 356, 107, 20, 739, 108, 20, 311, 107, 20, 739, 107, 20, 349, 107, 20, 698, 107, 20, 311, 107, 342, 622, 107, 127, 698, 107, 127, 407, 27, 20, 311, 27, 20, 273, 27, 20, 264, 26, 20, 171, 27, 20, 190, 27, 20, 220, 134, 154, 439, 107, 127, 475, 107, 127, 407, 27, 20, 311, 27, 20, 273, 27, 20, 264, 27, 20, 171, 26, 20, 190, 27, 20, 220, 27, 47, 475, 107, 127, 439, 107, 127, 423, 107, 128, 271, 26, 20, 207, 27, 20, 182, 27, 20, 176, 27, 20, 114, 26, 20, 127, 27, 20, 146, 27, 47, 369, 107, 127, 311, 27, 20, 229, 27, 20, 198, 26, 20, 190, 27, 20, 116, 27, 20, 130, 27, 20, 155, 27, 46, 423, 108, 127, 305, 26, 20, 233, 27, 20, 205, 27, 20, 197, 27, 20, 128, 27, 20, 142, 26, 20, 164, 27, 47, 329, 107, 127, 399, 107, 128, 431, 107, 127, 407, 27, 20, 311, 26, 20, 273, 27, 20, 264, 27, 20, 171, 27, 20, 190, 27, 20, 220, 133, 154, 439, 108, 127, 475, 107, 127, 407, 27, 20, 311, 27, 20, 273, 26, 20, 264, 27, 20, 171, 27, 20, 190, 27, 20, 220, 26, 47, 475, 107, 128, 439, 107, 127, 423, 107, 127, 271, 27, 20, 207, 27, 20, 182, 26, 20, 176, 27, 20, 114, 27, 20, 127, 27, 20, 146, 27, 46, 415, 108, 127, 305, 26, 20, 233, 27, 20, 205, 27, 20, 197, 27, 20, 128, 27, 20, 142, 26, 20, 164, 27, 47, 329, 107, 127, 349, 107, 342, 622, 107, 127, 698, 107, 127, 407, 27, 20, 311, 27, 20, 273, 27, 20, 264, 27, 20, 171, 26, 20, 190, 27, 20, 220, 134, 154, 439, 107, 127, 475, 107, 128, 407, 26, 20, 311, 27, 20, 273, 27, 20, 264, 27, 20, 171, 26, 20, 190, 27, 20, 220, 27, 47, 475, 107, 127, 439, 107, 127, 423, 108, 127, 271, 26, 20, 207, 27, 20, 182, 27, 20, 176, 27, 20, 114, 27, 20, 127, 26, 20, 146, 27, 47, 369, 107, 127, 311, 27, 20, 229, 27, 20, 198, 27, 20, 190, 26, 20, 116, 27, 20, 130, 27, 20, 155, 27, 47, 423, 107, 127, 305, 27, 20, 233, 26, 20, 205, 27, 20, 197, 27, 20, 128, 27, 20, 142, 27, 20, 164, 26, 47, 329, 107, 127, 399, 108, 127, 431, 107, 127, 407, 27, 20, 311, 27, 20, 273, 26, 20, 264, 27, 20, 171, 27, 20, 190, 27, 20, 220, 133, 155, 439, 107, 127, 475, 107, 127, 407, 27, 20, 311, 27, 20, 273, 26, 20, 264, 27, 20, 171, 27, 20, 190, 27, 20, 220, 27, 46, 475, 108, 127, 439, 107, 127, 423, 107, 127, 271, 27, 20, 207, 27, 20, 182, 27, 20, 176, 26, 20, 114, 27, 20, 127, 27, 20, 146, 27, 47, 415, 107, 127, 305, 27, 20, 233, 26, 20, 205, 27, 20, 197, 27, 20, 128, 27, 20, 142, 27, 20, 164, 26, 47, 329, 107, 127, 349, 108, 341, 493, 107, 20, 440, 107, 20, 415, 107, 20, 440, 108, 20, 339, 107, 127, 293, 107, 127, 369, 107, 20, 523, 107, 20, 349, 108, 20, 523, 107, 20, 380, 107, 127, 293, 107, 127, 391, 107, 20, 659, 108, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 466, 108, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 479, 107, 127, 293, 107, 128, 423, 107, 127, 448, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 73, 479, 108, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 74, 479, 107, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 399, 107, 127, 484, 107, 128, 329, 107, 341, 493, 107, 20, 440, 107, 20, 415, 108, 20, 440, 107, 20, 339, 107, 127, 293, 107, 127, 369, 107, 20, 523, 108, 20, 349, 107, 20, 523, 107, 20, 380, 107, 127, 293, 107, 127, 391, 108, 20, 659, 107, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 108, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 479, 107, 127, 293, 108, 127, 423, 107, 127, 448, 107, 127, 359, 54, 20, 380, 53, 20, 987, 54, 74, 479, 107, 127, 452, 107, 127, 479, 107, 127, 359, 54, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 452, 107, 127, 479, 107, 128, 359, 53, 20, 380, 54, 20, 987, 53, 74, 479, 107, 127, 399, 107, 127, 484, 108, 127, 329, 107, 341, 587, 107, 128, 640, 107, 127, 407, 107, 127, 513, 107, 127, 380, 108, 20, 783, 107, 20, 479, 107, 20, 659, 107, 20, 384, 107, 127, 391, 107, 128, 587, 107, 127, 640, 107, 127, 407, 107, 127, 513, 108, 127, 380, 107, 20, 783, 107, 20, 479, 107, 20, 659, 107, 20, 384, 107, 342, 479, 107, 127, 538, 107, 127, 336, 108, 127, 423, 107, 127, 302, 107, 20, 659, 107, 20, 391, 107, 20, 523, 108, 20, 323, 107, 127, 329, 107, 127, 479, 107, 127, 538, 108, 127, 336, 107, 127, 423, 107, 127, 302, 107, 20, 659, 108, 20, 391, 107, 20, 523, 107, 20, 323, 107, 341, 493, 108, 20, 440, 107, 20, 415, 107, 20, 440, 107, 20, 339, 107, 127, 293, 107, 128, 369, 107, 20, 523, 107, 20, 349, 107, 20, 523, 107, 20, 380, 107, 127, 293, 108, 127, 391, 107, 20, 659, 107, 20, 377, 107, 20, 659, 107, 20, 466, 107, 20, 880, 108, 20, 415, 107, 20, 880, 107, 20, 466, 107, 20, 880, 107, 20, 415, 107, 20, 880, 107, 20, 427, 108, 127, 261, 107, 127, 391, 107, 127, 407, 107, 128, 415, 214, 234, 359, 107, 127, 329, 108, 127, 339, 107, 127, 288, 107, 127, 320, 107, 128, 293, 107, 127, 266, 107, 127, 190, 107, 127, 256, 54, 20, 261, 54, 20, 493, 53, 74, 246, 53, 20, 256, 54, 127, 219, 54, 20, 155, 53, 342, 440, 80, 20, 622, 27, 20, 880, 80, 47, 493, 80, 20, 698, 27, 20, 329, 27, 20, 987, 53, 47, 246, 27, 20, 203, 27, 20, 193, 26, 20, 264, 27, 20, 273, 27, 20, 342, 27, 20, 493, 27, 46, 220, 108, 127, 311, 80, 20, 439, 27, 20, 880, 80, 47, 329, 80, 20, 475, 27, 20, 987, 80, 47, 246, 27, 20, 277, 27, 20, 233, 27, 20, 314, 26, 20, 273, 27, 20, 342, 27, 20, 493, 27, 47, 329, 80, 20, 475, 27, 20, 987, 80, 47, 311, 80, 20, 439, 27, 20, 880, 80, 47, 302, 80, 20, 423, 27, 20, 830, 81, 46, 164, 27, 20, 135, 27, 20, 128, 27, 20, 176, 27, 20, 182, 26, 20, 228, 27, 20, 329, 27, 47, 246, 80, 20, 369, 27, 20, 830, 80, 47, 184, 27, 20, 146, 27, 20, 136, 26, 20, 190, 27, 20, 193, 27, 20, 246, 27, 20, 369, 26, 47, 277, 81, 20, 423, 26, 20, 987, 81, 47, 184, 26, 20, 152, 27, 20, 144, 27, 20, 197, 27, 20, 204, 26, 20, 256, 27, 20, 369, 27, 47, 233, 80, 20, 329, 27, 20, 659, 80, 47, 269, 80, 20, 399, 27, 20, 880, 81, 46, 285, 81, 20, 431, 27, 20, 987, 80, 47, 246, 26, 20, 203, 27, 20, 193, 27, 20, 264, 27, 20, 273, 27, 20, 342, 26, 20, 493, 27, 47, 220, 107, 127, 311, 81, 20, 439, 26, 20, 880, 81, 47, 329, 80, 20, 475, 27, 20, 987, 80, 47, 246, 27, 20, 203, 26, 20, 193, 27, 20, 264, 27, 20, 273, 27, 20, 342, 27, 20, 493, 26, 47, 329, 80, 20, 475, 27, 20, 987, 81, 46, 311, 81, 20, 439, 27, 20, 880, 80, 47, 302, 53, 20, 423, 54, 20, 830, 53, 74, 164, 27, 20, 135, 27, 20, 128, 26, 20, 176, 27, 20, 182, 27, 20, 228, 27, 20, 329, 26, 47, 269, 81, 20, 415, 26, 20, 987, 81, 47, 184, 26, 20, 152, 27, 20, 144, 27, 20, 197, 27, 20, 204, 26, 20, 256, 27, 20, 369, 27, 47, 233, 80, 20, 329, 27, 20, 659, 80, 47, 220, 80, 20, 349, 27, 20, 880, 81, 261, 440, 80, 20, 622, 27, 20, 880, 80, 47, 493, 80, 20, 698, 27, 20, 987, 80, 47, 246, 27, 20, 203, 27, 20, 193, 27, 20, 264, 26, 20, 273, 27, 20, 342, 27, 20, 493, 27, 47, 220, 107, 127, 311, 80, 20, 439, 27, 20, 880, 80, 47, 329, 80, 20, 475, 27, 20, 987, 81, 46, 246, 27, 20, 203, 27, 20, 193, 27, 20, 264, 27, 20, 273, 26, 20, 342, 27, 20, 493, 27, 47, 329, 80, 20, 475, 27, 20, 987, 80, 47, 311, 80, 20, 439, 27, 20, 880, 80, 47, 302, 81, 20, 423, 26, 20, 830, 81, 47, 164, 26, 20, 135, 27, 20, 128, 27, 20, 176, 27, 20, 182, 26, 20, 228, 27, 20, 329, 27, 47, 246, 80, 20, 369, 27, 20, 830, 80, 47, 184, 27, 20, 146, 27, 20, 136, 26, 20, 190, 27, 20, 193, 27, 20, 246, 27, 20, 369, 27, 46, 277, 81, 20, 423, 27, 20, 987, 80, 47, 184, 26, 20, 152, 27, 20, 144, 27, 20, 197, 27, 20, 204, 27, 20, 256, 26, 20, 369, 27, 47, 233, 80, 20, 329, 27, 20, 659, 80, 47, 269, 81, 20, 399, 26, 20, 880, 81, 47, 285, 80, 20, 431, 27, 20, 987, 80, 47, 246, 27, 20, 203, 26, 20, 193, 27, 20, 264, 27, 20, 273, 27, 20, 342, 27, 20, 493, 26, 47, 220, 107, 127, 311, 81, 20, 439, 27, 20, 880, 80, 47, 329, 80, 20, 475, 27, 20, 987, 80, 47, 246, 27, 20, 203, 27, 20, 193, 26, 20, 264, 27, 20, 273, 27, 20, 342, 27, 20, 493, 26, 47, 329, 81, 20, 475, 26, 20, 987, 81, 47, 311, 80, 20, 439, 27, 20, 880, 80, 47, 302, 80, 20, 423, 27, 20, 830, 80, 47, 164, 27, 20, 135, 27, 20, 128, 26, 20, 176, 27, 20, 182, 27, 20, 228, 27, 20, 329, 27, 46, 269, 81, 20, 415, 27, 20, 987, 80, 47, 184, 26, 20, 152, 27, 20, 144, 27, 20, 197, 27, 20, 204, 27, 20, 256, 26, 20, 369, 27, 47, 233, 80, 20, 329, 27, 20, 659, 80, 47, 349, 27, 20, 277, 27, 20, 249, 27, 20, 244, 26, 20, 171, 27, 20, 190, 27, 20, 220, 134, 154, 513, 107, 127, 220, 54, 20, 598, 53, 20, 987, 54, 73, 523, 27, 20, 419, 27, 20, 366, 27, 20, 344, 27, 20, 171, 26, 20, 190, 27, 20, 220, 348, 369, 601, 107, 20};
static const int doom[] = {300, 82, 270, 20, 164, 130, 20, 82, 280, 20, 146, 130, 20, 82, 280, 20, 130, 130, 20, 82, 270, 20, 116, 140, 20, 97, 140, 20, 82, 130, 20, 100, 140, 20, 130, 140, 20, 82, 270, 20, 164, 140, 20, 82, 270, 20, 146, 130, 20, 82, 280, 20, 130, 130, 20, 82, 280, 20, 97, 410, 20, 116, 130, 160, 82, 270, 20, 164, 140, 20, 82, 270, 20, 146, 140, 20, 82, 270, 20, 130, 140, 20, 82, 270, 20, 116, 130, 20, 82, 280, 20, 100, 130, 20, 130, 140, 20, 82, 270, 20, 164, 140, 20, 82, 270, 20, 146, 140, 20, 82, 270, 20, 130, 140, 20, 82, 270, 20, 97, 410, 20, 116, 140, 150, 58, 140, 20, 82, 130, 20, 164, 140, 20, 82, 270, 20, 146, 140, 20, 82, 270, 20, 130, 140, 20, 82, 270, 20, 116, 140, 20, 97, 130, 20, 82, 140, 20, 100, 140, 20, 130, 130, 20, 82, 280, 20, 164, 130, 20, 82, 280, 20, 146, 130, 20, 82, 270, 20, 130, 140, 20, 82, 270, 20, 97, 410, 20, 116, 140, 160, 82, 270, 20, 164, 130, 20, 82, 280, 20, 146, 130, 20, 82, 280, 20, 130, 130, 20, 82, 280, 20, 116, 130, 20, 97, 140, 20, 82, 130, 20, 100, 140, 20, 130, 140, 20, 82, 270, 20, 164, 140, 20, 82, 270, 20, 146, 140, 20, 82, 270, 20, 369, 70, 20, 329, 60, 20, 311, 70, 20, 369, 70, 20, 440, 70, 20, 391, 70, 20, 369, 70, 20, 311, 60, 20, 369, 70, 20, 391, 70, 20, 440, 70, 20, 493, 70, 20, 440, 60, 20, 391, 70, 20, 369, 70, 20, 311, 70, 20, 82, 270, 20, 164, 140, 20, 82, 270, 20, 146, 140, 20};
static const int mario[] = {300, 587, 490, 150, 523, 120, 30, 659, 120, 150, 622, 240, 280, 311, 240, 280, 523, 240, 150, 391, 250, 150, 329, 240, 150, 440, 250, 20, 493, 250, 20, 466, 120, 30, 440, 120, 150, 391, 160, 20, 659, 170, 20, 783, 160, 30, 880, 240, 30, 698, 120, 20, 783, 120, 150, 659, 250, 20, 523, 120, 30, 587, 120, 20, 493, 120, 280, 523, 240, 150, 391, 250, 150, 329, 240, 150, 440, 250, 20, 493, 250, 20, 466, 120, 30, 440, 120, 150, 391, 160, 20, 659, 170, 20, 783, 160, 30, 880, 240, 30, 698, 120, 20, 783, 120, 150, 659, 250, 20, 523, 120, 30, 587, 120, 20, 493, 120, 280, 261, 120, 150, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150, 587, 120, 150, 415, 120, 30, 440, 120, 20, 523, 120, 150, 440, 120, 30, 523, 120, 20, 587, 120, 30, 261, 120, 150, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150, 587, 120, 150, 830, 360, 290, 391, 120, 150, 261, 120, 150, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150, 587, 120, 150, 415, 120, 30, 440, 120, 20, 523, 120, 150, 440, 120, 30, 523, 120, 20, 587, 120, 280, 508, 120, 270, 479, 120, 280, 415, 120, 270, 391, 240, 160, 261, 240, 280, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150, 587, 120, 150, 415, 120, 30, 440, 120, 20, 523, 120, 150, 440, 120, 30, 523, 120, 20, 587, 120, 30, 261, 120, 150, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150, 587, 120, 150, 830, 360, 290, 391, 120, 150, 261, 120, 150, 783, 120, 20, 739, 120, 30, 698, 120, 20, 622, 120, 150};
static const int megalovania[] = {2448, 293, 272, 20, 587, 137, 156, 440, 136, 293, 415, 136, 157, 391, 136, 156, 349, 137, 156, 293, 137, 20, 349, 136, 20, 391, 136, 20, 261, 273, 20, 587, 136, 157, 440, 136, 293, 415, 136, 157, 391, 136, 156, 349, 137, 156, 293, 136, 20, 349, 137, 20, 391, 136, 20, 246, 273, 20, 587, 136, 157, 440, 136, 293, 415, 136, 156, 391, 137, 156, 349, 136, 157, 293, 136, 20, 349, 137, 20, 391, 136, 20, 233, 273, 20, 587, 136, 156, 440, 137, 292, 415, 137, 156, 391, 137, 156, 349, 136, 157, 293, 136, 20, 349, 136, 20, 391, 137, 20, 207, 136, 20, 293, 273, 156, 254, 137, 20, 146, 136, 156, 246, 137, 156, 239, 136, 157, 226, 136, 156, 207, 137, 20, 226, 136, 20, 391, 137, 20, 184, 136, 20, 261, 136, 20, 277, 137, 156, 239, 136, 20, 130, 137, 156, 233, 136, 157, 226, 136, 157, 213, 136, 156, 195, 137, 20, 213, 136, 20, 391, 136, 20, 174, 137, 20, 246, 136, 20, 269, 136, 157, 233, 136, 20, 123, 137, 156, 226, 136, 157, 220, 136, 156, 207, 137, 156, 190, 136, 20, 207, 137, 20, 391, 136, 20, 164, 137, 20, 233, 136, 20, 261, 136, 157, 226, 136, 20, 116, 136, 157, 233, 136, 156, 226, 137, 156, 213, 137, 156, 195, 136, 20, 213, 137, 20, 391, 136, 20, 233, 136, 20, 415, 137, 20, 466, 136, 156, 305, 137, 20, 73, 136, 20, 146, 137, 20, 293, 136, 156, 282, 137, 20, 146, 136, 20, 261, 136, 20, 73, 137, 20, 233, 136, 20, 329, 136, 20, 554, 137, 20, 207, 136, 20, 369, 137, 20, 448, 136, 156, 293, 137, 20, 65, 136, 20, 130, 136, 20, 282, 137, 156, 271, 136, 20, 130, 137, 20, 251, 136, 20, 65, 137, 20, 224, 136, 20, 317, 136, 20, 554, 137, 20, 195, 136, 20, 349, 136, 20, 439, 137, 156, 288, 136, 20, 61, 137, 20, 123, 136, 20, 277, 137, 156, 266, 136, 20, 123, 137, 20, 246, 136, 20, 61, 136, 20, 219, 137, 20, 311, 136, 20, 554, 136, 20, 184, 137, 20, 329, 136, 20, 431, 137, 156, 282, 136, 20, 58, 137, 20, 116, 136, 20, 282, 136, 157, 271, 136, 20, 130, 136, 20, 251, 137, 20, 65, 136, 20, 224, 137, 20, 317, 136, 20, 554, 136, 20, 233, 137, 20, 415, 136, 20, 466, 136, 157, 305, 136, 20, 73, 136, 20, 146, 137, 20, 293, 136, 157, 282, 136, 20, 146, 136, 20, 261, 137, 20, 73, 136, 20, 233, 136, 20, 329, 137, 20, 554, 136, 20, 207, 136, 20, 369, 137, 20, 448, 136, 157, 293, 136, 20, 65, 136, 20, 130, 137, 20, 282, 136, 156, 271, 137, 20, 130, 136, 20, 251, 136, 20, 65, 137, 20, 224, 136, 20, 317, 137, 20, 554, 136, 20, 195, 136, 20, 349, 137, 20, 439, 136, 156, 288, 137, 20, 61, 136, 20, 123, 136, 20, 277, 137, 156, 266, 137, 20, 123, 136, 20, 246, 136, 20, 61, 137, 20, 219, 136, 20, 311, 136, 20, 554, 137, 20, 184, 136, 20, 329, 136, 20, 431, 137, 156, 282, 137, 20, 58, 136, 20, 116, 136, 20, 282, 137, 156, 271, 136, 20, 130, 137, 20, 251, 136, 20, 65, 136, 20, 224, 137, 20, 317, 136, 20, 554, 137, 20, 226, 136, 156, 320, 137, 20, 698, 136, 20, 73, 136, 20, 226, 137, 20, 146, 136, 20, 226, 136, 157, 207, 136, 20, 146, 137, 20, 207, 136, 20, 73, 273, 20, 146, 136, 156, 213, 137, 156, 302, 136, 20, 698, 137, 20, 65, 136, 20, 226, 137, 20, 130, 136, 20, 233, 136, 157, 226, 136, 20, 302, 136, 20, 195, 137, 20, 213, 136, 20, 226, 136, 20, 130, 137, 156, 207, 137, 156, 293, 136, 20, 698, 137, 20, 61, 136, 20, 219, 136, 20, 123, 137, 20, 226, 136, 156, 233, 137, 20, 123, 136, 20, 254, 137, 20, 61, 136, 20, 233, 136, 20, 123, 137, 156, 261, 136, 157, 369, 136, 156, 261, 137, 20, 226, 136, 20, 369, 137, 20, 261, 136, 156, 65, 137, 20, 130, 136, 20, 65, 136, 20, 302, 137, 20, 65, 136, 20, 130, 136, 157, 356, 136, 20, 73, 137, 20, 448, 136, 20, 783, 136, 20, 89, 137, 20, 356, 136, 20, 146, 136, 20, 261, 137, 156, 239, 136, 20, 146, 137, 20, 233, 136, 20, 73, 273, 20, 113, 136, 20, 97, 137, 20, 342, 136, 20, 65, 136, 20, 329, 137, 20, 783, 136, 20, 84, 136, 20, 342, 137, 20, 130, 136, 20, 236, 137, 156, 250, 136, 20, 130, 137, 20, 257, 136, 20, 65, 136, 20, 223, 137, 20, 261, 136, 20, 97, 136, 20, 415, 137, 20, 384, 136, 20, 311, 137, 20, 761, 136, 20, 246, 136, 20, 305, 137, 20, 349, 136, 20, 226, 136, 20, 1046, 137, 20, 246, 136, 20, 362, 136, 20, 223, 137, 20, 277, 136, 20, 187, 137, 20, 243, 136, 20, 293, 136, 20, 164, 137, 20, 58, 136, 20, 207, 136, 20, 523, 137, 20, 80, 136, 20, 288, 136, 20, 116, 137, 20, 261, 136, 157, 80, 136, 20, 130, 136, 20, 75, 137, 20, 65, 136, 20, 69, 136, 20, 106, 137, 20, 97, 136, 20, 58, 136, 157, 116, 136, 157, 58, 272, 20, 116, 137, 20, 58, 136, 20, 640, 136, 20, 251, 137, 20, 362, 136, 20, 311, 136, 20, 323, 137, 20, 311, 136, 20, 362, 137, 20, 523, 136, 20, 233, 136, 20, 783, 137, 20, 377, 136, 156, 323, 137, 20, 65, 136, 20, 130, 136, 20, 65, 274, 156, 130, 136, 20, 65, 273, 20, 233, 136, 20, 130, 136, 20, 880, 137, 20, 277, 136, 156, 359, 137, 20, 830, 136, 20, 239, 137, 20, 226, 136, 20, 293, 136, 20, 219, 137, 20, 698, 136, 20, 73, 136, 20, 339, 137, 20, 73, 136, 20, 254, 136, 20, 73, 137, 20, 391, 136, 157, 277, 136, 156, 339, 137, 156, 239, 136, 20, 233, 137, 20, 311, 136, 20, 246, 136, 157, 77, 136, 20, 155, 137, 20, 77, 409, 20, 155, 136, 156, 181, 137, 156, 246, 136, 157, 207, 136, 20, 58, 137, 20, 362, 136, 20, 58, 136, 20, 587, 137, 20, 58, 136, 20, 116, 136, 20, 58, 137, 20, 246, 136, 20, 58, 136, 20, 116, 137, 156, 256, 137, 156, 130, 136, 157, 271, 136, 20, 65, 136, 20, 130, 137, 20, 65, 136, 20, 640, 136, 20, 65, 137, 20, 130, 136, 20, 65, 137, 20, 266, 136, 20, 65, 136, 20, 130, 137, 156, 336, 136, 157, 146, 136, 156, 73, 273, 20, 146, 137, 20, 73, 136, 20, 880, 136, 20, 246, 137, 20, 339, 136, 20, 233, 136, 20, 226, 137, 20, 219, 136, 20, 302, 136, 20, 587, 137, 20, 251, 136, 157, 138, 136, 156, 69, 273, 20, 138, 136, 20, 77, 137, 20, 538, 136, 20, 77, 136, 20, 155, 137, 20, 77, 409, 20, 155, 136, 157, 58, 136, 156, 116, 137, 156, 58, 273, 20, 116, 136, 20, 58, 137, 20, 640, 136, 20, 251, 136, 20, 362, 137, 20, 311, 136, 20, 323, 136, 20, 311, 137, 20, 362, 136, 20, 523, 136, 20, 233, 137, 20, 783, 136, 20, 377, 137, 156, 323, 136, 20, 65, 137, 20, 130, 136, 20, 65, 272, 157, 130, 136, 20, 65, 273, 20, 233, 137, 20, 130, 136, 20, 880, 136, 20, 277, 137, 156, 359, 136, 20, 830, 137, 20, 239, 136, 20, 226, 136, 20, 293, 137, 20, 219, 136, 20, 698, 137, 20, 73, 136, 20, 339, 136, 20, 73, 137, 20, 254, 136, 20, 73, 136, 20, 391, 137, 156, 277, 136, 157, 339, 136, 157, 239, 136, 20, 233, 136, 20, 311, 137, 20, 246, 136, 156, 77, 137, 20, 155, 136, 20, 77, 409, 20, 155, 137, 156, 181, 136, 157, 246, 136, 156, 207, 137, 20, 58, 136, 20, 362, 136, 20, 58, 137, 20, 587, 136, 20, 58, 137, 20, 116, 136, 20, 58, 136, 20, 246, 137, 20, 58, 136, 20, 116, 136, 157, 256, 136, 156, 130, 137, 156, 271, 137, 20, 65, 136, 20, 130, 136, 20, 65, 137, 20, 640, 136, 20, 65, 136, 20, 130, 137, 20, 65, 136, 20, 266, 136, 20, 65, 137, 20, 130, 136, 157, 336, 136, 156, 146, 137, 156, 73, 273, 20, 146, 136, 20, 73, 136, 20, 880, 137, 20, 246, 136, 20, 339, 137, 20, 233, 136, 20, 226, 136, 20, 219, 137, 20, 302, 136, 20, 587, 136, 20, 251, 137, 156, 138, 136, 157, 69, 273, 20, 138, 136, 20, 77, 136, 20, 538, 137, 20, 77, 136, 20, 155, 136, 20, 77, 409, 20, 155, 137, 156, 164, 137, 156, 116, 818, 566, 349, 136, 20, 116, 273, 156, 207, 136, 157, 130, 545, 293, 293, 136, 20, 130, 545, 430, 207, 136, 156, 123, 2319, 1792, 164, 137, 156, 116, 818, 566, 349, 136, 20, 116, 273, 156, 207, 137, 156, 130, 544, 294, 293, 136, 20, 130, 547, 428, 207, 136, 157, 146, 545, 293, 293, 136, 20, 201, 137, 20, 261, 136, 20, 190, 136, 20, 233, 137, 20, 179, 136, 20, 174, 136, 20, 195, 137, 20, 164, 136, 20, 174, 137, 20, 155, 272, 20, 146, 954, 703, 125, 136, 20, 73, 136, 20, 130, 137, 156, 113, 136, 20, 116, 137, 156, 110, 137, 156, 106, 136, 157, 100, 136, 20, 349, 136, 20, 92, 137, 20, 100, 136, 20, 97, 136, 20, 141, 137, 20, 65, 136, 20, 138, 137, 156, 119, 136, 20, 130, 137, 156, 116, 136, 20, 293, 137, 20, 113, 136, 156, 106, 137, 156, 97, 137, 20, 106, 136, 20, 97, 136, 20, 138, 137, 20, 61, 136, 20, 134, 136, 157, 116, 136, 20, 123, 136, 157, 113, 136, 157, 110, 136, 156, 103, 137, 156, 95, 136, 20, 103, 137, 20, 97, 136, 20, 87, 136, 20, 190, 137, 20, 233, 136, 20, 587, 137, 20, 219, 136, 20, 123, 136, 20, 698, 137, 20, 113, 136, 20, 587, 136, 20, 184, 137, 156, 168, 136, 157, 152, 136, 20, 168, 137, 20, 226, 136, 20, 116, 136, 20, 58, 137, 20, 130, 136, 156, 113, 137, 20, 116, 136, 156, 110, 137, 156, 106, 137, 156, 100, 136, 20, 349, 137, 20, 92, 136, 20, 100, 136, 20, 97, 137, 20, 141, 136, 20, 65, 136, 20, 138, 137, 156, 119, 137, 20, 130, 136, 156, 116, 137, 20, 293, 136, 20, 113, 136, 157, 106, 136, 156, 97, 137, 20, 106, 136, 20, 97, 137, 20, 146, 136, 156, 246, 137, 156, 219, 136, 20, 146, 137, 156, 199, 136, 157, 211, 136, 157, 195, 136, 156, 169, 137, 20, 178, 136, 20, 226, 136, 20, 146, 137, 156, 246, 136, 157, 219, 136, 20, 146, 137, 156, 199, 136, 157, 211, 136, 156, 195, 137, 156, 169, 136, 20, 178, 137, 20, 226, 136, 20, 58, 137, 156, 116, 136, 157, 58, 272, 20, 116, 137, 20, 58, 273, 156, 116, 136, 20, 58, 409, 20, 116, 137, 156, 65, 136, 157, 130, 136, 156, 65, 273, 20, 130, 137, 20, 65, 273, 156, 130, 136, 20, 65, 409, 20, 130, 136, 157, 73, 136, 157, 146, 136, 156, 73, 273, 20, 146, 136, 20, 69, 273, 156, 138, 137, 20, 69, 409, 20, 138, 136, 157, 65, 136, 156, 130, 137, 156, 65, 273, 20, 130, 136, 20, 61, 273, 156, 123, 137, 20, 61, 409, 20, 123, 136, 156, 58, 137, 156, 116, 137, 156, 58, 273, 20, 116, 136, 20, 58, 272, 157, 116, 136, 20, 58, 410, 20, 116, 136, 156, 65, 137, 156, 130, 136, 157, 65, 272, 20, 130, 137, 20, 65, 272, 157, 130, 136, 20, 65, 409, 20, 130, 137, 156, 73, 136, 157, 146, 136, 157, 73, 272, 20, 146, 137, 20, 73, 273, 156, 146, 136, 20, 73, 409, 20, 146, 137, 156, 73, 136, 157, 146, 136, 156, 73, 273, 20, 146, 136, 20, 73, 274, 156, 146, 136, 20, 73, 409, 20, 146, 136, 157, 58, 136, 156, 116, 137, 156, 58, 273, 20, 116, 136, 20, 58, 273, 156, 116, 137, 20, 58, 409, 20, 116, 136, 157, 65, 136, 156, 130, 137, 156, 65, 273, 20, 130, 136, 20, 65, 272, 157, 130, 137, 20, 65, 409, 20, 130, 136, 156, 73, 137, 156, 146, 136, 157, 73, 273, 20, 146, 136, 20, 69, 272, 157, 138, 136, 20, 69, 409, 20, 138, 137, 156, 65, 137, 156, 130, 136, 157, 65, 272, 20, 130, 137, 20, 61, 273, 156, 123, 136, 20, 61, 409, 20, 123, 137, 156, 58, 136, 157, 116, 136, 156, 58, 273, 20, 116, 137, 20, 58, 273, 156, 116, 136, 20, 58, 409, 20, 116, 136, 157, 65, 136, 157, 130, 136, 156, 65, 273, 20, 130, 136, 20, 65, 273, 156, 130, 137, 20, 65, 409, 20, 130, 136, 157, 103, 136, 20, 146, 136, 20, 207, 137, 156, 127, 136, 20, 73, 137, 20, 146, 136, 20, 123, 137, 156, 119, 136, 20, 146, 137, 20, 113, 136, 20, 73, 136, 20, 103, 137, 20, 160, 136, 20, 195, 136, 20, 103, 137, 20, 146, 136, 20, 207, 137, 156, 127, 136, 20, 73, 137, 20, 146, 136, 20, 123, 136, 157, 119, 136, 20, 146, 136, 20, 113, 137, 20, 73, 136, 20, 103, 137, 20, 160, 136, 20, 195, 136, 20, 233, 273, 20, 587, 136, 157, 440, 136, 293, 415, 136, 157, 391, 136, 156, 349, 137, 156, 293, 136, 20, 349, 137, 20, 391, 136, 20, 261, 273, 20, 587, 136, 157, 440, 136, 293, 415, 136, 156, 391, 137, 156, 349, 136, 157, 293, 136, 20, 349, 137, 20, 391, 136, 20, 293, 273, 20, 587, 136, 156, 440, 137, 292, 415, 137, 156, 391, 137, 156, 349, 136, 157, 293, 136, 20, 349, 136, 20, 391, 137, 20, 293, 272, 20, 587, 137, 156, 440, 137, 292, 415, 137, 156, 391, 136, 157, 349, 136, 156, 293, 137, 20, 349, 136, 20, 391, 137, 20, 233, 272, 20, 587, 137, 156, 440, 136, 293, 415, 136, 157, 391, 136, 157, 349, 136, 156, 293, 137, 20, 349, 136, 20, 391, 136, 20, 261, 273, 20, 587, 136, 157, 440, 136, 293, 415, 136, 157, 391, 136, 156, 349, 137, 156, 293, 136, 20, 349, 137, 20, 391, 136, 20};