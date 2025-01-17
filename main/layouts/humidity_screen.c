#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "display_text.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_data.h"  
#include <freertos/semphr.h>

extern volatile bool display_update_required;
extern volatile sensor_data_t sensorData;

void humidityScreen(ssd1306_handle_t dev) {
    xSemaphoreTake(sensorData.mutex, portMAX_DELAY);
    display_update_required = true;  
    xSemaphoreGive(sensorData.mutex);
}
