#pragma once

#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"

void settings_animation_play(ssd1306_handle_t handle, int delay_ms);