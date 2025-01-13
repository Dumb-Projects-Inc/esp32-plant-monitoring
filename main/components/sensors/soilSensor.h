#pragma once

#include "Adafruit_Stemma_soil_sensor.h"
#include "esp_log.h"

#include "i2c.h"

#define tag "SOIL_SENSOR"


void stemma_soil_demo()
{
    int ret = ESP_OK;
    uint16_t moisture_value = 0;
    float temperature_value = 0;

    // Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());

    for (int i = 0; i < 10; i++)
    {
        ret = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture_value);

        if (ret == ESP_OK)
        {
            ESP_LOGI(tag, "Adafruit Stemma sensor value: =%u", moisture_value);
        }

        ret = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperature_value);

        if (ret == ESP_OK)
        {
            ESP_LOGI(tag, "Adafruit Stemma sensor value: =%f", temperature_value);
        }

        // 500 ms delay
        vTaskDelay((500) / portTICK_PERIOD_MS);
    }
}

uint16_t get_soil_moisture()
{
    uint16_t moist = 0;
    adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moist);

    return moist;
}

