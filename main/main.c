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
#include "Adafruit_Stemma_soil_sensor.h"

// OUTPUT PINS
#define GPIO_BUZZ_PIN (9)
#define GPIO_REDLED_PIN (1)

#define GPIO_RGB_R_PIN (4)
#define GPIO_RGB_G_PIN (5)
#define GPIO_RGB_B_PIN (6)


// I2C Pins
#define GPIO_SDA_PIN (2)
#define GPIO_SCL_PIN (3)

#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0


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
        data.soil_sensor.humidity = 10;
        data.soil_sensor.temperature = 25;
        vTaskDelay(pdMS_TO_TICKS(5000)); // wait 5 seconds
    }
}

QueueHandle_t sensorQueue; //Shared sensor data

void stemma_soil_demo(){
    int ret = ESP_OK;
    uint16_t moisture_value = 0;
    float temperature_value = 0;

    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());
    //ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_init(I2C_NUM, I2C_MASTER_SDA_GPIO, I2C_MASTER_SCL_GPIO));

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
        
        //500 ms delay
        vTaskDelay((500) / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    buzz_config_t b_config = {};
    b_config.gpio_num = GPIO_BUZZ_PIN;
    init_buzzer(&b_config);
    
    //all(&b_config);


    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_GPIO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    i2c_param_config(I2C_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));

    stemma_soil_demo();

    //Initialize a shared data struct
    //sensor_data_t *data = {0};
    sensorQueue = xQueueCreate(5, sizeof(sensor_data_t));

    void *sensorParams = {0}; //Initialized sensors might go here
    xTaskCreate(sensorLoop, "sensorLoop", 2048, sensorParams, 1, NULL);

}