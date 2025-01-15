#pragma once

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


#define tag "PROGRAM"

typedef struct {
    int button_gpio;
    void (*callback_button_down)();
    void (*callback_button_up)();
} buttonLoopArgs, *PButtonLoopArgs;

void init_button(int button_gpio, void (*callback_button_down)(), void (*callback_button_up)());