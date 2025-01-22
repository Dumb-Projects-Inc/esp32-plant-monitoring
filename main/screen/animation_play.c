#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "animation_play.h"
#include "display_text.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/semphr.h>
#include "sensor_data.h"

extern volatile bool display_update_required;
extern volatile sensor_data_t sensorData;
extern volatile int screen_number;
extern volatile const uint8_t (*current_animation)[128];

void animation_play(void *param)
{

    leaf_animation_params_t *params = (leaf_animation_params_t *)param;

    ssd1306_handle_t handle = params->handle;
    // const uint8_t(*frames)[128] = params->frames;
    size_t frame_count = params->frame_count;

    int frame = 0;

    static char previous_soil_humidity_str[16] = "0";
    static char previous_env_humidity_str[16] = "0";
    static char previous_soil_temp_str[16] = "0";
    static char previous_env_temp_str[16] = "0";
    static char previous_light_temp_str[16] = "0";
    int min_raw = 200;
    int max_raw = 2000;
    float soil_humidity_percentage = 0;

    while (1)
    {
        for (int y = 0; y < 48; y++)
        {
            ssd1306_draw_line(handle, 0, y, 32, y, 0);
        }
        ssd1306_draw_bitmap(handle, 5, 16, current_animation[frame], 32, 32);
        ssd1306_refresh_gram(handle);
        if (frame == 27)
        {
            frame = 0;
        }
        else
        {
            frame++;
        }

        xSemaphoreTake(sensorData.mutex, portMAX_DELAY);
        if (screen_number == 0)
        {

            if (display_update_required)
            {
                char soil_humidity_str[16];
                soil_humidity_percentage = ((float)(sensorData.soil.humidity - min_raw) / (max_raw - min_raw)) * 100;
                snprintf(soil_humidity_str, sizeof(soil_humidity_str), "%.2f", soil_humidity_percentage);

                char env_humidity_str[16];
                snprintf(env_humidity_str, sizeof(env_humidity_str), "%.2f", sensorData.humidity);

                if (strcmp(soil_humidity_str, previous_soil_humidity_str) != 0)
                {
                    display_text(handle, soil_humidity_str, 80, 12, 96, 18, false);
                    strcpy(previous_soil_humidity_str, soil_humidity_str);
                }

                if (strcmp(env_humidity_str, previous_env_humidity_str) != 0)
                {
                    display_text(handle, env_humidity_str, 0, 12, 96, 33, false);
                    strcpy(previous_env_humidity_str, env_humidity_str);
                }

                display_text(handle, "Humidity", 80, 16, 0, 0, true);
                display_text(handle, "Soil :", 80, 12, 59, 18, false);
                display_text(handle, "Env  :", 0, 12, 59, 33, false);

                display_update_required = false;
            }
        }
        else if (screen_number == 1)
        {
            if (display_update_required)
            {
                char soil_temp_str[16];
                snprintf(soil_temp_str, sizeof(soil_temp_str), "%.2f", sensorData.soil.temperature);

                char env_temp_str[16];
                snprintf(env_temp_str, sizeof(env_temp_str), "%.2f", sensorData.humidity);

                if (strcmp(soil_temp_str, previous_soil_temp_str) != 0)
                {
                    display_text(handle, soil_temp_str, 80, 12, 96, 18, false);
                    strcpy(previous_soil_temp_str, soil_temp_str);
                }

                if (strcmp(env_temp_str, previous_env_temp_str) != 0)
                {
                    display_text(handle, env_temp_str, 0, 12, 96, 33, false);
                    strcpy(previous_env_temp_str, env_temp_str);
                }

                display_text(handle, "Temp", 80, 16, 0, 0, true);
                display_text(handle, "Soil :", 80, 12, 59, 18, false);
                display_text(handle, "Env  :", 0, 12, 59, 33, false);

                display_update_required = false;
            }
        }
        else if (screen_number == 2)
        {
            if (display_update_required)
            {
                char light_str[16];
                snprintf(light_str, sizeof(light_str), "%d", sensorData.light);

                if (strcmp(light_str, previous_light_temp_str) != 0)
                {
                    display_text(handle, light_str, 80, 12, 96, 25, false);
                    strcpy(previous_light_temp_str, light_str);
                }

                display_text(handle, "Light", 80, 16, 0, 0, true);
                display_text(handle, "Soil :", 80, 12, 59, 25, false);

                display_update_required = false;
            }
        }

        xSemaphoreGive(sensorData.mutex);

        vTaskDelay(pdMS_TO_TICKS(42));
    }
}
