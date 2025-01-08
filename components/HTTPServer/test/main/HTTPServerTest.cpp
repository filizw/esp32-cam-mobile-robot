#include "WiFiStation.h"
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
    "<button onclick=\"LED_ON()\" onmouseup=\"LED_OFF()\">LED ON</button>"
    "<script>"
    "function LED_ON() {"
    "  fetch('/led-on').then(response => response.text()).then(data => {"
    "    console.log(data);"
    "  });"
    "}"
    "function LED_OFF() {"
    "  fetch('/led-off').then(response => response.text()).then(data => {"
    "    console.log(data);"
    "  });"
    "}"
    "let isLedOn = false;"
    "document.addEventListener('keydown', (event) => {"
    "    if (event.key === 'ArrowUp' && !isLedOn) {"
    "        isLedOn = true;"
    "        LED_ON();"
    "    }"
    "});"
    "document.addEventListener('keyup', (event) => {"
    "    if (event.key === 'ArrowUp' && isLedOn) {"
    "        isLedOn = false;"
    "        LED_OFF();"
    "    }"
    "});"
    "</script>"
    "</body>"
    "</html>";

constexpr auto LED_PIN{GPIO_NUM_4};

void ledOn()
{
    gpio_set_level(LED_PIN, 1);
}

void ledOff()
{
    gpio_set_level(LED_PIN, 0);
}

auto indexPageHandler = [](httpd_req_t *req) -> esp_err_t {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, HTML_PAGE, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

auto ledOnHandler = [](httpd_req_t *req) -> esp_err_t {
    ledOn();

    httpd_resp_send(req, "LED toggled", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

auto ledOffHandler = [](httpd_req_t *req) -> esp_err_t {
    ledOff();

    httpd_resp_send(req, "LED toggled", HTTPD_RESP_USE_STRLEN);
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

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    WiFiStation station("ssid", "password");
    station.init();

    HTTPServer serv;
    serv.start();
    serv.registerURIHandler("/", HTTP_GET, indexPageHandler);
    serv.registerURIHandler("/led-on", HTTP_GET, ledOnHandler);
    serv.registerURIHandler("/led-off", HTTP_GET, ledOffHandler);
}
