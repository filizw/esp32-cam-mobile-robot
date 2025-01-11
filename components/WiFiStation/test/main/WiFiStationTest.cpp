#include "WiFiStation.h"
#include "nvs_flash.h"

extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Input WiFi SSID and password
    std::string ssid{"ssid"};
    std::string password{"password"};

    WiFiStation station(ssid, password);
    station.setAddress("192.168.0.100", "192.168.0.1", "255.255.255.0");
    station.setTag("Wi-Fi");
    station.init();
}
