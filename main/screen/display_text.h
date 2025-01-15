#ifndef DISPLAY_TEXT_H
#define DISPLAY_TEXT_H

#include <string.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "display_text.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void display_text(ssd1306_handle_t dev, const char *text, int delay_ms, int fontSize, uint8_t x, uint8_t y, bool center);

#endif