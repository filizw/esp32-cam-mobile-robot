#include "RobotController.h"

#include "nvs_flash.h"

std::string RobotController::indexPage =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>Test Page</title>"
    "<style>"
    "</style>"
    "</head>"
    "<body>"
    "<button>Test Button</button>"
    "</body>"
    "</html>";

RobotController::RobotController(const std::string &ssid, const std::string &password) : station(WiFiStation(ssid, password))
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Initialize Wi-Fi
    station.setTag("Robot Controller Wi-Fi");
    ESP_ERROR_CHECK(station.init());

    // Start HTTP server
    server.setTag("Robot Controller HTTP server");
    ESP_ERROR_CHECK(server.start());

    // Create HTML index page handler
    auto indexPageHandler = [](httpd_req_t *req) -> esp_err_t {
        httpd_resp_set_type(req, "text/html");
        httpd_resp_send(req, indexPage.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    };

    // Register HTML index page handler
    ESP_ERROR_CHECK(server.registerURIHandler("/", HTTP_GET, indexPageHandler));
}
