#include "CameraServer.h"
#include "WiFiStation.h"
#include "nvs_flash.h"

const char *INDEX_PAGE =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>Test</title>"
    "<style>"
    "img { display: block; margin: 0 auto; max-width: 100%; height: auto; }"
    "</style>"
    "</head>"
    "<body>"
    "<img src='/stream'>"
    "</body>"
    "</html>";

auto indexPageHandler = [](httpd_req_t *req) -> esp_err_t {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, INDEX_PAGE, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

extern "C" void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    WiFiStation station("ssid", "password");
    station.init();

    CameraServer camServ;
    camServ.start();
    camServ.registerURIHandler("/", HTTP_GET, indexPageHandler);
}
