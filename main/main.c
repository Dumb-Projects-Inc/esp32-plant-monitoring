#include <stdio.h>


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#include <driver/gpio.h>
#include "esp_log.h"
#include "components/buzz.h"
#include "components/rgb.h"

#include "sensor_data.h"

#include "components/sensors/lightSensor.h"

// OUTPUT PINS
#define GPIO_BUZZ_PIN (9)
#define GPIO_REDLED_PIN (1)

#define GPIO_RGB_R_PIN (4)
#define GPIO_RGB_G_PIN (5)
#define GPIO_RGB_B_PIN (6)


// I2C Pins
#define GPIO_SDA_PIN (2)
#define GPIO_SCL_PIN (3)

//Input Pins
#define GPIO_BTN1_PIN (18)
#define GPIO_BTN2_PIN (19)
#define GPIO_LIGHTSENSOR_PIN (0)

#define tag "PROGRAM"

void sensorLoop(void *pvParameters ) {
    sensor_data_t data;
    
    init_light_sensor(ADC1_CHANNEL_0);

    while(1) {
        data.light = get_light_value();
        vTaskDelay(pdMS_TO_TICKS(5000)); // wait 5 seconds
    }
}

QueueHandle_t sensorQueue; //Shared sensor data

void app_main(void)
{
    buzz_config_t b_config = {};
    b_config.gpio_num = GPIO_BUZZ_PIN;
    init_buzzer(&b_config);
    all(&b_config);

    //Initialize a shared data struct
    //sensor_data_t *data = {0};
    sensorQueue = xQueueCreate(5, sizeof(sensor_data_t));

    void *sensorParams = {0}; //Initialized sensors might go here
    xTaskCreate(sensorLoop, "sensorLoop", 2048, sensorParams, 1, NULL);

}