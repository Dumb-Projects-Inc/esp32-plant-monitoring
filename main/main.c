#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <driver/gpio.h>
#include "esp_log.h"
#include "esp_random.h"
#include "peripherals/buzz.h"
#include "peripherals/rgb.h"

#include "sensor_data.h"
#include "i2c.h"

#include "sensors/lightSensor.h"
#include "sensors/soilSensor.h"
#include "am2320.h"

#include "peripherals/button.h"

// screen
#include "screen/animation_play.h"
#include <string.h>
#include "screen/bitmap.h"
#include "screen/screen_manager.h"

#include "wifi.h"

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

// Sample rates and waits
#define SENSOR_SAMPLE_RATE_MS (1000)
#define NUM_SAMPLE 1
#define TIME_BETWEEN_SAMPLES 20

#define CONTROL_DELAY_MS (5000)

#define EXPERIMENT_LOGGING 0 // 0 = on , 1 = off
#define LOGGING_WIFI 0       // 0 = on , 1 = off

#define tag "PROGRAM"

#define OLED_ADDR 0x3C

volatile sensor_data_t sensorData = {
    .soil.humidity = 0,
    .soil.temperature = 0,
    .temperature = 0,
    .humidity = 0,
    .light = 0,
    .mutex = NULL};

volatile rule_limits_t limits = {
    .light = 0,
    .temperature = 0,
    .humidity = 0,
    .soil_temperature = 0,
    .soil_humidity = 600,
};

volatile int screen_number = 0;

ssd1306_handle_t ssd1306_dev = NULL;

void sensorLoop(void *pvParameters)
{
    init_light_sensor(ADC1_CHANNEL_0);

    i2c_dev_t dev = {0};
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));

    while (1)
    {
        sensor_data_t sensorData_sample = {
            .soil.humidity = 0,
            .soil.temperature = 0,
            .temperature = 0,
            .humidity = 0,
            .light = 0,
        };

        // Take the average over multiple samples.
        for (size_t i = 0; i < NUM_SAMPLE; i++)
        {
            sensorData_sample.light += get_light_value();
            sensorData_sample.soil.humidity += get_soil_moisture();
            sensorData_sample.soil.temperature += get_soil_temperature();

            float temperature, humidity;
            esp_err_t res = am2320_get_rht(&dev, &temperature, &humidity);
            if (res == ESP_OK)
            {
                sensorData_sample.temperature = temperature;
                sensorData_sample.humidity = humidity;
            }
            vTaskDelay(pdMS_TO_TICKS(TIME_BETWEEN_SAMPLES));
        }
        xSemaphoreTake(sensorData.mutex, portMAX_DELAY);
        sensorData.light = sensorData_sample.light / NUM_SAMPLE;
        sensorData.soil.humidity = sensorData_sample.soil.humidity / NUM_SAMPLE;
        sensorData.soil.temperature = sensorData_sample.soil.temperature / NUM_SAMPLE;
        sensorData.temperature = sensorData_sample.temperature / NUM_SAMPLE;
        sensorData.humidity = sensorData_sample.humidity / NUM_SAMPLE;

        xSemaphoreGive(sensorData.mutex);
        vTaskDelay(pdMS_TO_TICKS(SENSOR_SAMPLE_RATE_MS - (TIME_BETWEEN_SAMPLES * NUM_SAMPLE))); // wait
    }
}

void log_data_serial()
{
#if LOGGING_WIFI == 0
    post(sensorData);
#else
    ESP_LOGI("SENSOR_VALS", "%d, %.2f, %.2f, %.2f, %d", sensorData.light, sensorData.temperature, sensorData.humidity, sensorData.soil.temperature, sensorData.soil.humidity);
#endif
}

void controlLoop(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(sensorData.mutex, portMAX_DELAY);
        if (sensorData.soil.humidity < limits.soil_humidity && sensorData.light < limits.light 
            && sensorData.temperature < limits.temperature && sensorData.humidity < limits.humidity
            && sensorData.soil.temperature < limits.soil_temperature && sensorData.soil.humidity > 0)
        {
            xSemaphoreGive(sensorData.mutex);

            rgb_set_color(255, 0, 0);
            //Only start the song task if it is not already running
            if (xTaskGetHandle("song_task") == NULL) {
                int song = esp_random() % 3;
                xTaskCreate(song_task, "song_task", 4096, (void*)&song, 1, NULL);
            }
            
        }
        else
        {
            xSemaphoreGive(sensorData.mutex);

            rgb_set_color(0, 255, 0);
        }
#if EXPERIMENT_LOGGING == 0
        log_data_serial();
#endif
        vTaskDelay(pdMS_TO_TICKS(CONTROL_DELAY_MS)); // wait
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
    animations_init(&ssd1306_dev);
    screen_manager_init();
    init_button(GPIO_BTN1_PIN, next_screen, NULL);
    init_button(GPIO_BTN2_PIN, previous_screen, NULL);
#if LOGGING_WIFI == 0
    init_wifi();
#endif

    sensorData.mutex = xSemaphoreCreateMutex();

    xTaskCreate(animation_play, "Screen", 4096 * 2, ssd1306_dev, 1, NULL);
    xTaskCreate(sensorLoop, "sensorLoop", 4096, NULL, 1, NULL);
    xTaskCreate(controlLoop, "controlLoop", 4096 * 2, NULL, 1, NULL);
}