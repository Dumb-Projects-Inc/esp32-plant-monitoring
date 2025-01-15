#pragma once

#include "Adafruit_Stemma_soil_sensor.h"
#include "esp_log.h"

#include "i2c.h"

#define soil_tag "SOIL_SENSOR"

uint16_t get_soil_moisture()
{
    uint16_t moist = 0;
    adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moist);

    return moist;
}

float get_soil_temperature()
{
    float temp = 0;
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temp));
    return temp;
}

