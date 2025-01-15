#include "rgb.h"

#include "esp_log.h"
esp_err_t init_rgb_led(rgb_led_config_t *config){
    
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 1 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    config->ledc_timer = ledc_timer;
    ESP_ERROR_CHECK(ledc_timer_config(&config->ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_red = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_RED,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = config->r_io_num,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    
    config->ledc_channel_red = ledc_channel_red;
    ESP_ERROR_CHECK(ledc_channel_config(&config->ledc_channel_red));

    ledc_channel_config_t ledc_channel_green = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_GREEN,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = config->g_io_num,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    config->ledc_channel_green = ledc_channel_green;
    ESP_ERROR_CHECK(ledc_channel_config(&config->ledc_channel_green));
    
    ledc_channel_config_t ledc_channel_blue = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_BLUE,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = config->b_io_num,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    
    config->ledc_channel_blue = ledc_channel_blue;
    ESP_ERROR_CHECK(ledc_channel_config(&config->ledc_channel_blue));
    return 0;
}

void rgb_set_color(rgb_led_config_t *config, uint8_t red, uint8_t green, uint8_t blue) {

    uint32_t duty_red = LEDC_DUTY - red*(LEDC_DUTY / 255);
    uint32_t duty_green = LEDC_DUTY - green*(LEDC_DUTY / 255);
    uint32_t duty_blue = LEDC_DUTY - blue*(LEDC_DUTY / 255);

    ESP_LOGI("RGB", "Setting color to: %lu %lu %lu", duty_red, duty_green, duty_blue);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, duty_red);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, duty_green);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, duty_blue);
    

    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);


}