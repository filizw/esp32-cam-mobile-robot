#include "nvs_flash.h"
#include "WiFiSta.h"

extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Input your WiFi SSID and password
    std::string ssid{"ssid"};
    std::string password{"password"};

    WiFiSta wifiStation(ssid, password);
    wifiStation.init();
}
