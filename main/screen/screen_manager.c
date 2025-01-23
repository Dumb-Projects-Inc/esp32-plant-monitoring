#include "screen_manager.h"

extern volatile sensor_data_t sensorData;

static screen_t screens[] = {
    {.id = 0, .name = "Humidity Screen", .update = update_humidity_screen, .animation = heart_rate_animation, .animation_frames = 28},
    {.id = 1, .name = "Temperature Screen", .update = update_temperature_screen, .animation = temperature_animation, .animation_frames = 28},
    {.id = 2, .name = "Light Screen", .update = update_light_screen, .animation = leaf_animation, .animation_frames = 28},
};

static const size_t num_screens = sizeof(screens) / sizeof(screens[0]);
static volatile int current_screen_index = 0;
SemaphoreHandle_t screen_mutex = NULL;

void screen_manager_init()
{
    screen_mutex = xSemaphoreCreateMutex();
    if (screen_mutex == NULL)
    {
        printf("Failed to create screen mutex.\n");
        return;
    }
}

void next_screen() // increments the screen index and resets if it reaches the end of the index
{
    xSemaphoreTake(screen_mutex, portMAX_DELAY);
    current_screen_index = (current_screen_index + 1) % num_screens;
    xSemaphoreGive(screen_mutex);
}

void previous_screen() // same thing as the next_screen but it decrements the index instead
{
    xSemaphoreTake(screen_mutex, portMAX_DELAY);
    current_screen_index = (current_screen_index == 0) ? num_screens - 1 : current_screen_index - 1;
    xSemaphoreGive(screen_mutex);
}

const screen_t *get_current_screen() // fetches the screen and return it as a pointer to the screen_t struct
{
    xSemaphoreTake(screen_mutex, portMAX_DELAY);
    const screen_t *screen = &screens[current_screen_index];
    xSemaphoreGive(screen_mutex);
    return screen;
}

void set_screen_animation(int screen_id, const uint8_t (*new_animation)[128]) // sets the animation for the screen
{
    if (screen_id >= 0 && screen_id < num_screens)
    {
        screens[screen_id].animation = new_animation;
    }
}

void update_humidity_screen(ssd1306_handle_t handle) // updates the values of the screen
{
    ssd1306_clear_screen(handle, 0);
    char soil_humidity_str[16];
    char env_humidity_str[16];

    snprintf(soil_humidity_str, sizeof(soil_humidity_str), "%d", sensorData.soil.humidity);
    snprintf(env_humidity_str, sizeof(env_humidity_str), "%.2f", sensorData.humidity);

    ssd1306_draw_string(handle, 0, 0, (const uint8_t *)"Humidity", 16, true);
    ssd1306_draw_string(handle, 96, 20, (const uint8_t *)soil_humidity_str, 12, true);
    ssd1306_draw_string(handle, 64, 20, (const uint8_t *)"Soil: ", 12, true);
    ssd1306_draw_string(handle, 96, 36, (const uint8_t *)env_humidity_str, 12, true);
    ssd1306_draw_string(handle, 64, 36, (const uint8_t *)"Env: ", 12, true);
}

void update_temperature_screen(ssd1306_handle_t handle) // updates the values of the screen
{
    ssd1306_clear_screen(handle, 0);
    char soil_temp_str[16];
    char env_temp_str[16];

    snprintf(soil_temp_str, sizeof(soil_temp_str), "%.2f", sensorData.soil.temperature);
    snprintf(env_temp_str, sizeof(env_temp_str), "%.2f", sensorData.temperature);

    ssd1306_draw_string(handle, 0, 0, (const uint8_t *)"Temperature", 16, true);
    ssd1306_draw_string(handle, 96, 20, (const uint8_t *)soil_temp_str, 12, true);
    ssd1306_draw_string(handle, 64, 20, (const uint8_t *)"Soil: ", 12, true);
    ssd1306_draw_string(handle, 96, 36, (const uint8_t *)env_temp_str, 12, true);
    ssd1306_draw_string(handle, 64, 36, (const uint8_t *)"Env : ", 12, true);
}

void update_light_screen(ssd1306_handle_t handle) // updates the values of the screen
{
    ssd1306_clear_screen(handle, 0);
    char light_str[16];
    snprintf(light_str, sizeof(light_str), "%d", sensorData.light);

    ssd1306_draw_string(handle, 0, 0, (const uint8_t *)"Light", 16, true);
    ssd1306_draw_string(handle, 96, 24, (const uint8_t *)light_str, 12, true);
    ssd1306_draw_string(handle, 64, 24, (const uint8_t *)"Light: ", 12, true);
}
