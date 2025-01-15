#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <driver/gpio.h>
#include "esp_log.h"
#include "peripherals/buzz.h"
#include "peripherals/rgb.h"

#include "sensor_data.h"
#include "i2c.h"

#include "sensors/lightSensor.h"
#include "sensors/soilSensor.h"

#include "peripherals/button.h"

// OUTPUT PINS
#define GPIO_BUZZ_PIN (9)
#define GPIO_REDLED_PIN (1)

#define GPIO_RGB_R_PIN (4)
#define GPIO_RGB_G_PIN (5)
#define GPIO_RGB_B_PIN (6)



// Input Pins
#define GPIO_BTN1_PIN (18)
#define GPIO_BTN2_PIN (19)
#define GPIO_LIGHTSENSOR_PIN (0)

#define tag "PROGRAM"

QueueHandle_t sensorQueue; // Shared sensor data



void sensorLoop(void *pvParameters)
{
    sensor_data_t data;

    init_light_sensor(ADC1_CHANNEL_0);

    while (1)
    {
        data.light = get_light_value();
        data.soil_sensor.humidity = get_soil_moisture();
        data.soil_sensor.temperature = get_soil_temperature();
        xQueueSend(sensorQueue, &data, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000)); // wait 5 seconds
    }
}

void controlLoop(void *pvParameters)
{
    sensor_data_t data;
    while (1)
    {
        if (xQueueReceive(sensorQueue, &data, portMAX_DELAY))
        {
            if(data.soil_sensor.humidity < 600)
            {
                rgb_set_color(255, 0, 0);
                play_song(doom, 300);
            }
            else
            {
                rgb_set_color(0, 255, 0);
            }
            ESP_LOGI(tag, "Soil Moisture: %u, Soil Temperature, %.2f, Light: %d", data.soil_sensor.humidity, data.soil_sensor.temperature, data.light);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // wait 5 second
    }
}


void app_main(void)
{
    buzz_config_t b_config = {.gpio_num = GPIO_BUZZ_PIN};
    rgb_led_config_t rgb_config = {
        .r_io_num = GPIO_RGB_R_PIN,
        .g_io_num = GPIO_RGB_G_PIN,
        .b_io_num = GPIO_RGB_B_PIN,
    };
    
    init_buzzer(&b_config);
    init_rgb_led(&rgb_config);
    init_i2c();


    // Initialize a shared data struct
    // sensor_data_t *data = {0};
    sensorQueue = xQueueCreate(5, sizeof(sensor_data_t));

    void *sensorParams = {0}; // Initialized sensors might go here
    xTaskCreate(sensorLoop, "sensorLoop", 2048, sensorParams, 1, NULL);
    xTaskCreate(controlLoop, "controlLoop", 2048, NULL, 1, NULL);
}