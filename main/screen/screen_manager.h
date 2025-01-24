#pragma once

#include "ssd1306.h"
#include "animation_play.h"
#include "sensor_data.h"
#include "bitmap.h"

typedef struct {
    int id;
    const char *name;
    void (*update)(ssd1306_handle_t handle);
    const uint8_t (*animation)[128]; 
    size_t animation_frames;
} screen_t;


void screen_manager_init();
void next_screen();
void previous_screen();
void set_screen_number(int screen_number);
const screen_t *get_current_screen();
void set_screen_animation(int screen_id, const uint8_t (*new_animation)[128]);

void update_humidity_screen(ssd1306_handle_t handle); 
void update_temperature_screen(ssd1306_handle_t handle); 
void update_light_screen(ssd1306_handle_t handle); 


