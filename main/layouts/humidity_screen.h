
#ifndef HUMIDITY_SCREEN_H
#define HUMIDITY_SCREEN_H

#include "ssd1306.h"

void humidityScreen(ssd1306_handle_t dev, uint16_t env_humidity, uint16_t soil_humidity);

#endif