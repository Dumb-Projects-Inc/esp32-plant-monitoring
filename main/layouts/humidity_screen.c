#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "leaf_animation.h"
#include "display_text.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void humidityScreen(ssd1306_handle_t dev, uint16_t env_humidity, uint16_t soil_humidity)
{
    ssd1306_clear_screen(dev, 0x00);

    char soil_humidity_str[16];
    if (soil_humidity == 100)
    {
        strcpy(soil_humidity_str, "100% :");
    }
    else if (soil_humidity >= 10)
    {
        snprintf(soil_humidity_str, sizeof(soil_humidity_str), " %d%%", soil_humidity);
    }
    else
    {
        snprintf(soil_humidity_str, sizeof(soil_humidity_str), "  %d%%", soil_humidity);
    }

    char env_humidity_str[16];
    if (env_humidity == 100)
    {
        strcpy(env_humidity_str, "100% :");
    }
    else if (env_humidity >= 10)
    {
        snprintf(env_humidity_str, sizeof(env_humidity_str), " %d%%", env_humidity);
    }
    else
    {
        snprintf(env_humidity_str, sizeof(env_humidity_str), "  %d%%", env_humidity);
    }

    display_text(dev, "", 80, 16, 0, 48, true);
    display_text(dev, "Humidity", 80, 16, 0, 0, true);
    display_text(dev, "Soil :", 80, 12, 59, 18, false);
    display_text(dev, soil_humidity_str, 80, 12, 96, 18, false);
    display_text(dev, "Env  :", 0, 12, 59, 33, false);
    display_text(dev, env_humidity_str, 0, 12, 96, 33, false);
    xTaskCreate(leaf_animation_play, "HumidityScreenTask", 2048, (void *)dev, 5, NULL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("Humidity Screen\n");
}