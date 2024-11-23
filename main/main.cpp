#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "WiFiSta.h"

#define LED_PIN GPIO_NUM_4

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"

extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    WiFiSta wifi(WIFI_SSID, WIFI_PASSWORD);
    wifi.init();

    int level = 0;

    while(1)
    {
        level = !level;
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
