#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "ssd1306.h"

void animations_init(ssd1306_handle_t *dev);
void play_leaf_animation(ssd1306_handle_t dev);
void play_settings_animation(ssd1306_handle_t dev, int delay_ms);

#endif