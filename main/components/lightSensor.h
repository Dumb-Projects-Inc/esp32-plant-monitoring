#pragma once

#include <driver/adc.h>
#include "esp_log.h"

int init_light_sensor(int ADC_CHANNEL);
float get_light_value();
