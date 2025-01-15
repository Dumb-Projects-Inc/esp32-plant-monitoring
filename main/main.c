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
#include "am2320.h"

#include "peripherals/button.h"

// Animation
#include "screen/leaf_animation.h"
#include "screen/settings_animation.h"
#include "screen/animations.h"
#include <string.h>

// Layouts
#include "layouts/humidity_screen.h"


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

#define I2C_MASTER_SCL_IO 3
#define I2C_MASTER_SDA_IO 2
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 50000
#define OLED_ADDR 0x3C

QueueHandle_t sensorQueue; // Shared sensor data
ssd1306_handle_t ssd1306_dev = NULL;

void sensorLoop(void *pvParameters)
{
    
    sensor_data_t data;

    init_light_sensor(ADC1_CHANNEL_0);
    i2c_dev_t dev = {0};
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&dev, I2C_NUM));

    while (1)
    {
        data.light = get_light_value();
        data.soil_sensor.humidity = get_soil_moisture();
        data.soil_sensor.temperature = get_soil_temperature();

        float temperature, humidity;
        esp_err_t res = am2320_get_rht(&dev, &temperature, &humidity);
        if (res == ESP_OK)
            data.temperature = temperature;
            data.humidity = humidity;
            ESP_LOGI(tag, "Temperature: %.1f°C, Humidity: %.1f%%", temperature, humidity);
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
                //play_song(doom);  
            }
            else
            {
                rgb_set_color(0, 255, 0);
            }
            ESP_LOGI(tag, "Light: %d, Soil Temp: %.2f, Soil Humidity: %d", data.light, data.soil_sensor.temperature, data.soil_sensor.humidity);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // wait 5 second
    }
}


void app_main(void)
{
    //buzz_config_t b_config = {.gpio_num = GPIO_BUZZ_PIN};
    //rgb_led_config_t rgb_config = {
    //    .r_io_num = GPIO_RGB_R_PIN,
    //    .g_io_num = GPIO_RGB_G_PIN,
    //    .b_io_num = GPIO_RGB_B_PIN,
    //};
     i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    animations_init(&ssd1306_dev);

    humidityScreen(ssd1306_dev, 66, 1);

    //
    //init_buzzer(&b_config);
    //init_rgb_led(&rgb_config);
    //init_i2c();


    // Initialize a shared data struct
    // sensor_data_t *data = {0};

    //sensorQueue = xQueueCreate(5, sizeof(sensor_data_t));

    void *sensorParams = {0}; // Initialized sensors might go here
    xTaskCreate(sensorLoop, "sensorLoop", 4096, sensorParams, 1, NULL);
    xTaskCreate(controlLoop, "controlLoop", 4096, NULL, 1, NULL);
}