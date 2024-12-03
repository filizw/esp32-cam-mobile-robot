#include "WiFiSta.h"
#include "HTTPServer.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

const char *HTML_PAGE =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>Test Page</title>"
    "<style>"
    "body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; }"
    "h1 { color: #333; }"
    "button { padding: 10px 20px; font-size: 16px; margin-top: 20px; cursor: pointer; }"
    "</style>"
    "</head>"
    "<body>"
    "<button onclick=\"toggleLED()\">Toggle LED</button>"
    "<script>"
    "function toggleLED() {"
    "  fetch('/toggle-led').then(response => response.text()).then(data => {"
    "    console.log(data);"
    "  });"
    "}"
    "</script>"
    "</body>"
    "</html>";

static bool ledLevel{};
constexpr auto LED_PIN{GPIO_NUM_4};

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

    WiFiSta sta("ssid", "password");
    sta.init();

    auto indexPageHandler = [](httpd_req_t *req) -> esp_err_t {
        httpd_resp_set_type(req, "text/html");
        httpd_resp_send(req, HTML_PAGE, HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    };

    auto toggleLedHandler = [](httpd_req_t *req) -> esp_err_t {
        ledLevel = !ledLevel;
        gpio_set_level(LED_PIN, ledLevel);

        httpd_resp_send(req, "LED toggled", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    };

    HTTPServer serv;
    serv.start();
    serv.registerURIHandler("/", HTTP_GET, indexPageHandler);
    serv.registerURIHandler("/toggle-led", HTTP_GET, toggleLedHandler);
}
