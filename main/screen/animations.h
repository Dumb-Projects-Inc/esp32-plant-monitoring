#pragma once

#include "ssd1306.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include "leaf_animation.h"
#include "settings_animation.h"

void animations_init(ssd1306_handle_t *dev);
void play_leaf_animation(ssd1306_handle_t dev);
void play_settings_animation(ssd1306_handle_t dev, int delay_ms);

