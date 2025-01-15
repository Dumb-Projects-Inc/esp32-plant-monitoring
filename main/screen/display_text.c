
#include "display_text.h"

void display_text(ssd1306_handle_t dev, const char *text, int delay_ms, int fontSize, uint8_t x, uint8_t y, bool center)
{
    int text_len = strlen(text);
    int screen_width = 128; 
    int char_width = fontSize / 2;
    int max_chars = screen_width / char_width;
    char buffer[max_chars + 1];

    if (center) {
        int text_pixel_width = text_len * char_width;
        x = (screen_width - text_pixel_width) / 2;
    }

    if (text_len <= max_chars) {
        ssd1306_draw_string(dev, x, y, (const uint8_t *)text, fontSize, true);
        ssd1306_refresh_gram(dev);
    } else {
        for (int i = 0; i < text_len - max_chars + 1; i++) {
            strncpy(buffer, text + i, max_chars);
            buffer[max_chars] = '\0';
            ssd1306_clear_screen(dev, 0x00);
            ssd1306_draw_string(dev, 0, y, (const uint8_t *)buffer, fontSize, true);
            ssd1306_refresh_gram(dev);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
    }
}
