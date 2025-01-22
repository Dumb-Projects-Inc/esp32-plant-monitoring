#include "button.h"

static void IRAM_ATTR button_isr_handler(void* arg) {
    TaskHandle_t taskHandle = (TaskHandle_t)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(taskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void button_loop(void *Pbargs) {
    PButtonLoopArgs args = Pbargs;
    int last_state = gpio_get_level(args->button_gpio);
    int current_state;

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        current_state = gpio_get_level(args->button_gpio);
        if(current_state != last_state) {
            if (current_state == 0) {
                if (args->callback_button_down != NULL) {
                    args->callback_button_down();
                }
            } else {
                if (args ->callback_button_up != NULL) {
                    args->callback_button_up();
                }
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
    
    PButtonLoopArgs args = malloc(sizeof(buttonLoopArgs));
    if (args == NULL) {
        ESP_LOGE(tag, "Failed to alloc memory to PButton");
        return;
    }
    args->button_gpio = button_gpio;
    args->callback_button_down = callback_button_down;
    args->callback_button_up = callback_button_up;

    char taskName[20];
    snprintf(taskName, 20, "button_loop_%d", button_gpio);
    TaskHandle_t taskHandle;
    xTaskCreate(button_loop, taskName, 1024*2, args, 1, &taskHandle);

    static bool isrInstalled = false;
    if (!isrInstalled) {
        esp_err_t err = gpio_install_isr_service(0);
        if (err != ESP_OK) {
            ESP_LOGI("button_loop", "ISR service install error: %d", err);
            exit(1);
        }
        isrInstalled = true;
    }
    gpio_isr_handler_add(button_gpio, button_isr_handler, (void*)taskHandle);
}