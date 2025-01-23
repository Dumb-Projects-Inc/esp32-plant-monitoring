#pragma once

#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include <stdio.h>
#include <string.h>
#include "animation_play.h"
#include <freertos/semphr.h>
#include "screen_manager.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define OLED_ADDR 0x3C

void animation_play(ssd1306_handle_t handle); 
void animations_init(ssd1306_handle_t *handle); 
