#include "leaf_animation.h"
#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "leaf_animation.h"
#include "display_text.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/semphr.h>
#include "sensor_data.h"

extern volatile bool display_update_required;
extern volatile sensor_data_t sensorData;

static const uint8_t frames[][128] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 30, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 3, 81, 26, 0, 2, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 9, 12, 0, 0, 137, 12, 0, 0, 217, 14, 0, 0, 81, 26, 0, 0, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 9, 12, 0, 0, 9, 12, 0, 0, 25, 14, 0, 0, 17, 26, 0, 0, 17, 154, 124, 0, 17, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 1, 12, 0, 0, 1, 12, 0, 0, 1, 14, 0, 0, 1, 26, 0, 0, 1, 154, 124, 0, 1, 147, 192, 0, 0, 145, 128, 0, 0, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1, 12, 0, 0, 1, 12, 0, 0, 1, 14, 0, 0, 1, 26, 0, 0, 1, 154, 124, 0, 1, 147, 192, 0, 0, 145, 128, 0, 0, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 0, 14, 0, 0, 0, 26, 0, 0, 0, 26, 124, 0, 0, 19, 192, 0, 0, 17, 128, 0, 0, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 0, 14, 0, 0, 0, 26, 0, 0, 0, 26, 124, 0, 0, 19, 192, 0, 0, 17, 128, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 124, 0, 0, 3, 192, 0, 0, 1, 128, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 1, 128, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 1, 128, 0, 0, 1, 216, 0, 0, 2, 80, 0, 0, 62, 112, 0, 0, 0, 112, 0, 0, 0, 48, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 0, 8, 0, 0, 0, 136, 0, 0, 1, 136, 0, 0, 1, 216, 0, 0, 2, 80, 0, 0, 62, 112, 0, 0, 0, 112, 0, 0, 0, 48, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 0, 0, 1, 137, 0, 0, 1, 217, 0, 0, 2, 81, 0, 0, 62, 113, 128, 0, 0, 113, 128, 0, 0, 48, 128, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 0, 0, 1, 137, 0, 0, 1, 217, 0, 0, 2, 81, 0, 0, 62, 113, 128, 0, 0, 113, 128, 0, 0, 48, 128, 0, 0, 32, 128, 0, 0, 0, 128, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 0, 0, 1, 137, 8, 0, 1, 217, 8, 0, 2, 81, 24, 0, 62, 113, 144, 0, 0, 113, 144, 0, 0, 48, 144, 0, 0, 32, 176, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 0, 0, 113, 147, 0, 0, 48, 144, 0, 0, 32, 176, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 64, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 120, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 11, 0, 0, 0, 137, 12, 0, 1, 137, 12, 0, 1, 217, 14, 0, 2, 81, 26, 0, 62, 113, 154, 124, 0, 113, 147, 192, 0, 48, 145, 128, 0, 32, 177, 0, 0, 0, 160, 0, 0, 0, 224, 0, 0, 0, 224, 0, 0, 0, 192, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

//static const uint8_t leaf_animation[][128] = {
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 60, 32, 0, 3, 224, 48, 0, 15, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 192, 0, 8, 1, 128, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 16, 8, 6, 0, 32, 8, 2, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 60, 48, 0, 1, 224, 16, 0, 7, 0, 16, 0, 24, 0, 24, 0, 96, 0, 24, 0, 192, 0, 8, 1, 128, 1, 8, 3, 0, 3, 8, 2, 0, 2, 8, 6, 0, 6, 12, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 6, 0, 48, 8, 2, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 193, 0, 16, 0, 118, 0, 48, 0, 60, 0, 32, 0, 28, 0, 96, 0, 54, 0, 192, 0, 195, 1, 128, 63, 1, 255, 0, 24, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 224, 0, 0, 3, 160, 0, 0, 30, 48, 0, 0, 248, 16, 0, 3, 128, 16, 0, 28, 0, 24, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 8, 1, 0, 1, 8, 3, 0, 3, 12, 2, 0, 2, 12, 6, 0, 6, 12, 6, 0, 4, 12, 4, 0, 8, 12, 4, 0, 24, 12, 6, 0, 48, 12, 2, 0, 32, 8, 3, 0, 64, 8, 1, 128, 128, 24, 0, 193, 128, 16, 0, 115, 0, 16, 0, 62, 0, 32, 0, 28, 0, 96, 0, 54, 0, 64, 0, 227, 1, 128, 63, 129, 199, 0, 24, 0, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 96, 0, 0, 1, 224, 0, 0, 15, 48, 0, 0, 124, 16, 0, 3, 192, 16, 0, 14, 0, 24, 0, 48, 0, 8, 0, 96, 0, 8, 0, 192, 0, 8, 1, 128, 1, 8, 1, 0, 1, 12, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 12, 4, 6, 0, 8, 12, 6, 0, 24, 12, 2, 0, 48, 12, 3, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 24, 0, 99, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 54, 0, 64, 0, 227, 0, 192, 63, 129, 131, 0, 16, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 144, 0, 0, 30, 16, 0, 1, 240, 24, 0, 7, 0, 8, 0, 28, 0, 8, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 140, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 4, 4, 6, 0, 12, 4, 2, 0, 24, 4, 2, 0, 48, 12, 1, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 0, 227, 0, 192, 63, 131, 129, 128, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 112, 0, 0, 1, 144, 0, 0, 14, 16, 0, 0, 240, 24, 0, 3, 128, 8, 0, 12, 0, 8, 0, 48, 0, 8, 0, 96, 0, 12, 0, 192, 0, 140, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 24, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 1, 226, 0, 192, 63, 3, 129, 192, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 112, 0, 0, 1, 144, 0, 0, 15, 16, 0, 0, 248, 24, 0, 7, 128, 8, 0, 28, 0, 8, 0, 48, 0, 12, 0, 96, 0, 12, 0, 192, 0, 132, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 8, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 192, 8, 0, 97, 0, 24, 0, 62, 0, 16, 0, 12, 0, 48, 0, 54, 0, 32, 0, 226, 0, 64, 63, 3, 129, 192, 16, 1, 255, 0, 0, 0, 60, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 144, 0, 0, 30, 16, 0, 0, 240, 24, 0, 3, 128, 8, 0, 12, 0, 8, 0, 48, 0, 8, 0, 96, 0, 12, 0, 192, 0, 140, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 24, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 192, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 1, 226, 0, 192, 63, 3, 129, 192, 16, 0, 255, 0, 0, 0, 8, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 48, 0, 0, 60, 16, 0, 1, 224, 16, 0, 7, 0, 24, 0, 28, 0, 8, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 140, 1, 0, 1, 4, 3, 0, 1, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 4, 4, 6, 0, 12, 4, 2, 0, 24, 4, 2, 0, 48, 12, 1, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 0, 227, 0, 192, 63, 131, 129, 128, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 224, 0, 0, 1, 160, 0, 0, 6, 48, 0, 0, 120, 16, 0, 3, 192, 16, 0, 14, 0, 24, 0, 56, 0, 8, 0, 96, 0, 8, 0, 192, 0, 8, 1, 128, 1, 8, 1, 0, 1, 12, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 12, 4, 6, 0, 8, 12, 2, 0, 24, 12, 2, 0, 48, 12, 3, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 24, 0, 99, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 118, 0, 96, 1, 195, 0, 192, 63, 1, 131, 128, 0, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 224, 0, 0, 3, 160, 0, 0, 14, 32, 0, 0, 240, 48, 0, 3, 128, 16, 0, 12, 0, 16, 0, 48, 0, 24, 0, 96, 0, 8, 0, 192, 1, 8, 1, 128, 1, 8, 3, 0, 3, 8, 2, 0, 2, 12, 2, 0, 6, 12, 6, 0, 4, 12, 4, 0, 8, 12, 4, 0, 24, 12, 6, 0, 16, 12, 2, 0, 32, 8, 3, 0, 96, 8, 1, 128, 192, 24, 0, 193, 128, 24, 0, 115, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 54, 0, 64, 0, 227, 0, 128, 63, 129, 227, 0, 0, 0, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 1, 192, 0, 0, 7, 32, 0, 0, 60, 32, 0, 1, 224, 48, 0, 7, 128, 16, 0, 28, 0, 16, 0, 48, 0, 16, 0, 192, 0, 24, 1, 128, 1, 8, 1, 0, 3, 8, 2, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 16, 8, 6, 0, 48, 8, 2, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 24, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 96, 0, 118, 0, 192, 1, 195, 1, 128, 63, 1, 255, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 128, 0, 0, 0, 192, 0, 0, 3, 64, 0, 0, 14, 96, 0, 0, 120, 32, 0, 3, 192, 32, 0, 15, 0, 48, 0, 56, 0, 16, 0, 96, 0, 16, 0, 192, 1, 24, 1, 128, 3, 24, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 96, 0, 102, 0, 192, 1, 195, 3, 128, 63, 1, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 3, 64, 0, 0, 28, 64, 0, 0, 112, 32, 0, 3, 192, 32, 0, 14, 0, 48, 0, 56, 0, 16, 0, 96, 0, 16, 0, 192, 2, 16, 1, 0, 2, 24, 3, 0, 2, 8, 2, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 98, 0, 128, 1, 195, 131, 0, 63, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 7, 64, 0, 0, 28, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 192, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 6, 24, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 24, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 0, 128, 1, 195, 131, 0, 63, 0, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 7, 64, 0, 0, 28, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 224, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 24, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 114, 0, 128, 1, 195, 131, 0, 63, 0, 252, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 1, 192, 0, 0, 3, 64, 0, 0, 30, 96, 0, 0, 248, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 96, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 2, 8, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 128, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 1, 128, 1, 195, 131, 0, 63, 0, 252, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 3, 192, 0, 0, 30, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 12, 0, 48, 0, 48, 0, 16, 0, 96, 0, 16, 0, 128, 0, 24, 1, 0, 2, 24, 3, 0, 2, 8, 6, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 128, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 1, 128, 0, 195, 135, 0, 63, 0, 252, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 3, 224, 0, 0, 14, 32, 0, 0, 248, 32, 0, 3, 192, 48, 0, 14, 0, 16, 0, 48, 0, 16, 0, 96, 0, 16, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 6, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 114, 0, 128, 0, 195, 3, 0, 63, 128, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 1, 224, 0, 0, 14, 32, 0, 0, 120, 32, 0, 3, 192, 48, 0, 14, 0, 16, 0, 56, 0, 16, 0, 224, 0, 16, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 118, 0, 128, 0, 195, 3, 0, 63, 129, 254, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 6, 32, 0, 0, 120, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 224, 0, 24, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 128, 0, 195, 3, 0, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void leaf_animation_play(void *param)
{

    ssd1306_handle_t handle = *(ssd1306_handle_t *)param;
    int frame = 0;

    static char previous_soil_humidity_str[16] = "0";
    static char previous_env_humidity_str[16] = "0";
    int min_raw = 200;
    int max_raw = 2000;
    float soil_humidity_percentage = 0;

    while (1)
    {
        for (int y = 0; y < 48; y++)
        {
            ssd1306_draw_line(handle, 0, y, 32, y, 0);
        }
        ssd1306_draw_bitmap(handle, 5, 16, frames[frame], 32, 32);
        ssd1306_refresh_gram(handle);
        frame = (frame + 1) % (sizeof(frames) / sizeof(frames[0]));

        xSemaphoreTake(sensorData.mutex, portMAX_DELAY);

        if (display_update_required)
        {
            char soil_humidity_str[16];
            soil_humidity_percentage = ((float)(sensorData.soil.humidity - min_raw) / (max_raw - min_raw)) * 100;
            snprintf(soil_humidity_str, sizeof(soil_humidity_str), "%.2f", soil_humidity_percentage);

            char env_humidity_str[16];
            snprintf(env_humidity_str, sizeof(env_humidity_str), "%.2f", sensorData.humidity);

            if (strcmp(soil_humidity_str, previous_soil_humidity_str) != 0)
            {
                display_text(handle, soil_humidity_str, 80, 12, 96, 18, false);
                strcpy(previous_soil_humidity_str, soil_humidity_str);
            }

            if (strcmp(env_humidity_str, previous_env_humidity_str) != 0)
            {
                display_text(handle, env_humidity_str, 0, 12, 96, 33, false);
                strcpy(previous_env_humidity_str, env_humidity_str);
            }

            display_text(handle, "Humidity", 80, 16, 0, 0, true);
            display_text(handle, "Soil :", 80, 12, 59, 18, false);
            display_text(handle, "Env  :", 0, 12, 59, 33, false);

            display_update_required = false;
        }

        xSemaphoreGive(sensorData.mutex);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
