#include "button.h"

static TaskHandle_t taskHandle = NULL;

static void IRAM_ATTR button_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(taskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void button_loop(void *Pbargs) {
    PButtonLoopArgs args = Pbargs;
    int last_state = gpio_get_level(args->button_gpio);
    int current_state;
    int gpio_num;

    while (1) {
        ulTaskNotifyTake(0, portMAX_DELAY);
        current_state = gpio_get_level(args->button_gpio);
        if(current_state!= last_state) {
            if (current_state == 0) {
                args->callback_button_down();
            } else {
                args->callback_button_up();
            }
            last_state = current_state;
        }
    }
}

void init_button(int button_gpio, void (*callback_button_down)(), void (*callback_button_up)()) {
    gpio_config_t io_config = {
        .pin_bit_mask = 1ULL << button_gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&io_config);
    
    // This isnt freed as is needed during whole program execution
    PButtonLoopArgs args = malloc(sizeof(buttonLoopArgs));
    if (args == NULL) {
        ESP_LOGE(tag, "Failed to alloc memory to PButton");
        return;
    }
    args->button_gpio = button_gpio;
    args->callback_button_down = callback_button_down;
    args->callback_button_up = callback_button_up;


    xTaskCreate(button_loop, "button_loop", 1024*2, args, 1, &taskHandle);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(button_gpio, button_isr_handler, (void*)&taskHandle);
}