#include "driver/gpio.h"
#include "esp_log.h"
#define tag "PROGRAM"

typedef struct _buttonLoopArgs {
    int button_gpio;
    void (*callback_button_down)();
    void (*callback_button_up)();
} buttonLoopArgs, *PButtonLoopArgs;

void button_loop(void *Pbargs) {
    PButtonLoopArgs args = Pbargs;
    int button_status = gpio_get_level(args->button_gpio);
    int button_now;
    while (1) {
        button_now = gpio_get_level(args->button_gpio);
        if(button_status != button_now) {
            if (button_now == 0) {
                args->callback_button_down();
            } else {
                args->callback_button_up();
            }
            button_status = button_now;
        }

        vTaskDelay(50/portTICK_PERIOD_MS);
    }
    
}

void init_button(int button_gpio, void (*callback_button_down)(), void (*callback_button_up)()) {
    gpio_config_t io_config = {
        .pin_bit_mask = 1ULL << button_gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_config);
    PButtonLoopArgs args = malloc(sizeof(buttonLoopArgs));
    if (args == NULL) {
        return;
    }
    args->button_gpio = button_gpio;
    args->callback_button_down = callback_button_down;
    args->callback_button_up = callback_button_up;
    xTaskCreate(button_loop, "button_loop", 1024*2, args, 1, NULL);

}