#include "animation_play.h"
#define I2C_MASTER_NUM I2C_NUM_0
#define OLED_ADDR 0x3C

extern volatile sensor_data_t sensorData;
extern volatile rule_limits_t limits;

void animation_play(ssd1306_handle_t handle)
{
    static size_t frame_index = 0;

    while (1)
    {
        const screen_t *screen = get_current_screen(); // fetches the pointer to the current screen
        if (screen == NULL)
        {
            printf("No screen found\n");
            continue;
        }
        screen->update(handle);

        if (sensorData.soil.humidity < limits.soil_humidity && sensorData.soil.humidity > 0)
        {
            set_screen_animation(0, alarm_animation);
            set_screen_number(0);
        }
        else
        {
            set_screen_animation(0, heart_rate_animation);
        }

        if (screen->animation != NULL && screen->animation_frames > 0) // if there is an animation set to play, then play it!
        {
            for (int y = 22; y < 48; y++)
            {
                ssd1306_draw_line(handle, 0, y, 32, y, 0);
            }

            ssd1306_draw_bitmap(handle, 5, 22, screen->animation[frame_index], 32, 32);
            frame_index = (frame_index + 1) % screen->animation_frames; // increments the index of frames and resets ALL the way back to frame 1 when it reaches the end of index
        }
        ssd1306_refresh_gram(handle);  // refreshes the screen. This function is the reason why the screen is smooth and not flickering
        vTaskDelay(pdMS_TO_TICKS(16)); // waits
    }
}

void animations_init(ssd1306_handle_t *dev)
{
    *dev = ssd1306_create(I2C_MASTER_NUM, OLED_ADDR);
    ssd1306_clear_screen(*dev, 0x00);
    ssd1306_refresh_gram(*dev);
}
