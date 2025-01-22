#pragma once

#include "ssd1306.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"

typedef struct {
    ssd1306_handle_t handle;    
    const uint8_t (*frames)[128]; 
    size_t frame_count;          
} leaf_animation_params_t;

void animation_play(void *param); 
