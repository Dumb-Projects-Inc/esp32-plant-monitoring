#pragma once

#include "am2320.h"
#include "esp_log.h"

#include "i2c.h"

#define AM_tag "AM2320_SENSOR"

uint16_t get_soil_moisture()
{
    return 1;
}

float get_soil_temperature()
{
    float temp = 0;

    return temp;
}