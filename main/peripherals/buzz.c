#include "buzz.h"

volatile buzz_config_t b_config = {};
esp_err_t init_buzzer(buzz_config_t *config) {

    // Prepare and then apply the LEDC PWM timer configuration (we use it for the buzzer)
    ledc_timer_config_t ledc_timer_buzz = {
        .speed_mode       = BUZZ_MODE,
        .duty_resolution  = BUZZ_DUTY_RES,
        .timer_num        = BUZZ_TIMER,
        .freq_hz          = BUZZ_FREQUENCY,  // Set output frequency at 1 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    config->ledc_timer_buzz = ledc_timer_buzz;
    ESP_ERROR_CHECK(ledc_timer_config(&config->ledc_timer_buzz));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_buzz = {
        .speed_mode     = BUZZ_MODE,
        .channel        = BUZZ_CHANNEL,
        .timer_sel      = BUZZ_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = config->gpio_num,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    
    config->ledc_channel_buzz = ledc_channel_buzz;
    ESP_ERROR_CHECK(ledc_channel_config(&config->ledc_channel_buzz));
    
    //Make config available to all functions in this file
    b_config = *config;
    return 0;
}




//This function should be removed down the line, when we can play music
void all() {
    
    

    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 4096)); //50% duty
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
    //1000 ms delay
    ESP_LOGI(BUZZER_TAG, "Playing 1000 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, 400)); //50% duty
    ESP_LOGI(BUZZER_TAG, "Playing 600 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);
    
    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, 800)); //50% duty
    ESP_LOGI(BUZZER_TAG, "Playing 800 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, 1000)); //50% duty
    ESP_LOGI(BUZZER_TAG, "Playing 1000 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, 400)); //50% duty
    ESP_LOGI(BUZZER_TAG, "Playing 600 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);
    
    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, 800)); //50% duty
    ESP_LOGI(BUZZER_TAG, "Playing 800 Hz.");
    vTaskDelay((600) / portTICK_PERIOD_MS);

    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 0)); //0% duty
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
    ESP_LOGI(BUZZER_TAG, "Buzzer off.");
}

void play_song(const int *notes, int length) {
    for (int i = 0; i < length; i += 3) {
        play_note(notes[i], notes[i+1]);
        pause_note(notes[i+2]);
    }
    pause_note(1);
}

void play_note(int hz, int period) {
    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 6000)); //50% duty
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
    ESP_ERROR_CHECK(ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, hz)); //50% duty
    vTaskDelay((period) / portTICK_PERIOD_MS);
    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 0)); //0% duty
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
}

void pause_note(int period) {
    // Set duty
    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 0)); //0% duty
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
    vTaskDelay((period) / portTICK_PERIOD_MS);
}