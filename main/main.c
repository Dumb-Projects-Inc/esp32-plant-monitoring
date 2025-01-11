#include <stdio.h>

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include "esp_log.h"
#include "components/buzz.h"
#include "components/rgb.h"

// OUTPUT PINS
#define GPIO_BUZZ_PIN (9)
#define GPIO_REDLED_PIN (8)

#define GPIO_RGB_R_PIN (4)
#define GPIO_RGB_G_PIN (5)
#define GPIO_RGB_B_PIN (6)



#define GPIO_PIEZO_IO (1ULL << GPIO_BUZZ_PIN)

#define tag "PROGRAM"

void app_main(void)
{
    buzz_config_t b_config = {};
    b_config.gpio_num = GPIO_BUZZ_PIN;
    init_buzzer(&b_config);
    all(&b_config);
}