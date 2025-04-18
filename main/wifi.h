#if LOGGING_WIFI == 0
#include "esp_wifi.h"
#include "esp_wpa2.h" // For WPA2 APIs
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "sensor_data.h"

#include "wifi_secrets.h"

#include "esp_http_client.h"

#define MAX_HTTP_OUTPUT_BUFFER 2048


static const char *WIFI_TAG = "WIFI";

extern volatile sensor_data_t sensorData;
extern volatile rule_limits_t limits;

void init_wifi() {
        esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default Wi-Fi station
    esp_netif_create_default_wifi_sta();

    // Initialize the Wi-Fi driver
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set the Wi-Fi to station mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Configure Wi-Fi credentials for WPA2 Enterprise
    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, "DTUsecure");

    // Set WPA2 Enterprise username and password Deprecated but it works
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)wifi_username, strlen(wifi_username)));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_username((uint8_t *)wifi_username, strlen(wifi_username)));
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_set_password((uint8_t *)wifi_password, strlen(wifi_password)));

    // Enable WPA2 Enterprise
    ESP_ERROR_CHECK(esp_wifi_sta_wpa2_ent_enable());

    // Apply the Wi-Fi configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));

    // Start the Wi-Fi driver
    ESP_ERROR_CHECK(esp_wifi_start());

    // Connect to the Wi-Fi network
    ESP_ERROR_CHECK(esp_wifi_connect());
}


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{

    switch(evt->event_id) {
        // Read the data sent from server
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(WIFI_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // Print the data as a string
            char *data = malloc(evt->data_len + 1);
            memcpy(data, evt->data, evt->data_len);
            data[evt->data_len] = '\0'; // Make sure the string is null terminated

            ESP_LOGI(WIFI_TAG, "HTTP_EVENT_ON_DATA, data=%s", data);
            
            //Parse the data
            char *token = strtok(data, ",");
            
            limits.light = atoi(token);
            limits.temperature = atof(strtok(NULL, ","));
            limits.humidity = atof(strtok(NULL, ","));
            limits.soil_temperature = atof(strtok(NULL, ","));
            limits.soil_humidity = atoi(strtok(NULL, ","));

            ESP_LOGI(WIFI_TAG, "Limits: %d, %.2f, %.2f, %.2f, %d", limits.light, limits.temperature, limits.humidity, limits.soil_temperature, limits.soil_humidity);

            free(data);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void http_post_task(void *pvParameters) {
    // Create a buffer in the stack.
    char post_data[150] = {0};
    xSemaphoreTake(sensorData.mutex, portMAX_DELAY);
    int out = snprintf(post_data, sizeof(post_data), "{\"light\": %d, \"temperature\": %.2f, \"humidity\": %.2f, \"soil_temperature\": %.2f, \"soil_humidity\": %d }", 
    sensorData.light, sensorData.temperature, sensorData.humidity, sensorData.soil.temperature, sensorData.soil.humidity);
    xSemaphoreGive(sensorData.mutex);
    if(out < 0) {
        ESP_LOGE(WIFI_TAG, "Failed to create post data");
        vTaskDelete(NULL);
    }

    esp_http_client_config_t config = {
        .url = "http://172.232.139.206:1234/data",

        .event_handler = _http_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_url(client, config.url);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(WIFI_TAG, "HTTP POST Status = %d, Content length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(WIFI_TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}

void post() {
    xTaskCreate(&http_post_task, "http_post_task", 4096, NULL, 5, NULL);
}

#endif