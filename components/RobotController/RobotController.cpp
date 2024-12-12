#include "RobotController.h"

#include "nvs_flash.h"

WiFiStation RobotController::station("", "");
HTTPServer RobotController::server;
std::unordered_map<std::string, RobotController::KeyEventHandlerContext> RobotController::contexts;

std::string RobotController::indexPage =
    "<!DOCTYPE html>"
    "<html>"
    // head
    "<head>"
    "<title>Test Page</title>" // Page title
    // style
    "<style>"
    "body { font-family: Arial, sans-serif; margin: 20px; display: flex; flex-direction: column; align-items: center; }"
    ".key-container { display: grid; grid-template-areas: \". . . . ArrowUp .\" \"L . . ArrowLeft ArrowDown ArrowRight\"; gap: 20px; margin-top: 20px; }"
    ".key { width: 50px; height: 50px; border: 2px solid black; display: flex; justify-content: center; align-items: center; font-size: 10px; }"
    ".pressed { background-color: black; color: white; }"
    ".released { background-color: white; color: black; }"
    "</style>"
    "</head>"
    // body
    "<body>"
    "<div class='key-container'>"
    "<div style='grid-area: L;' id='L' class='key released'>L</div>"
    "<div style='grid-area: ArrowUp;' id='ArrowUp' class='key released'>Up</div>"
    "<div style='grid-area: ArrowLeft;' id='ArrowLeft' class='key released'>Left</div>"
    "<div style='grid-area: ArrowDown;' id='ArrowDown' class='key released'>Down</div>"
    "<div style='grid-area: ArrowRight;' id='ArrowRight' class='key released'>Right</div>"
    "</div>"
    
    // script
    "<script>"
    "let isArrowUpKeyPressed = false;"
    "let isArrowDownKeyPressed = false;"
    "let isArrowLeftKeyPressed = false;"
    "let isArrowRightKeyPressed = false;"
    "let isLKeyPressed = false;"

    "function updateKeyVisual(key, isPressed) {"
    "const keyElement = document.getElementById(key);"
    "if (isPressed) {"
    "keyElement.classList.add('pressed');"
    "keyElement.classList.remove('released');"
    "} else {"
    "keyElement.classList.add('released');"
    "keyElement.classList.remove('pressed');"
    "}"
    "}"

    "function keyEvent(key, event) {"
    "fetch(`/${key}-${event}`).then(response => response.text()).then(data => {"
    "console.log(data);"
    "});"
    "}"

    "document.addEventListener('keydown', (event) => {"
    "switch (event.key) {"
    // ArrowUp
    "case 'ArrowUp':"
    "if(!isArrowUpKeyPressed) {"
    "isArrowUpKeyPressed = true;"
    "keyEvent('ArrowUp', 'down');"
    "updateKeyVisual('ArrowUp', true);"
    "}"
    "break;"
    // ArrowDown
    "case 'ArrowDown':"
    "if(!isArrowDownKeyPressed) {"
    "isArrowDownKeyPressed = true;"
    "keyEvent('ArrowDown', 'down');"
    "updateKeyVisual('ArrowDown', true);"
    "}"
    "break;"
    // ArrowLeft
    "case 'ArrowLeft':"
    "if(!isArrowLeftKeyPressed) {"
    "isArrowLeftKeyPressed = true;"
    "keyEvent('ArrowLeft', 'down');"
    "updateKeyVisual('ArrowLeft', true);"
    "}"
    "break;"
    // ArrowRight
    "case 'ArrowRight':"
    "if(!isArrowRightKeyPressed) {"
    "isArrowRightKeyPressed = true;"
    "keyEvent('ArrowRight', 'down');"
    "updateKeyVisual('ArrowRight', true);"
    "}"
    "break;"
    // l
    "case 'l':"
    "if(!isLKeyPressed) {"
    "isLKeyPressed = true;"
    "keyEvent('l', 'down');"
    "updateKeyVisual('L', true);"
    "}"
    "break;"
    "}"
    "});"

    "document.addEventListener('keyup', (event) => {"
    "switch (event.key) {"
    // ArrowUp
    "case 'ArrowUp':"
    "if(isArrowUpKeyPressed) {"
    "isArrowUpKeyPressed = false;"
    "keyEvent('ArrowUp', 'up');"
    "updateKeyVisual('ArrowUp', false);"
    "}"
    "break;"
    // ArrowDown
    "case 'ArrowDown':"
    "if(isArrowDownKeyPressed) {"
    "isArrowDownKeyPressed = false;"
    "keyEvent('ArrowDown', 'up');"
    "updateKeyVisual('ArrowDown', false);"
    "}"
    "break;"
    // ArrowLeft
    "case 'ArrowLeft':"
    "if(isArrowLeftKeyPressed) {"
    "isArrowLeftKeyPressed = false;"
    "keyEvent('ArrowLeft', 'up');"
    "updateKeyVisual('ArrowLeft', false);"
    "}"
    "break;"
    // ArrowRight
    "case 'ArrowRight':"
    "if(isArrowRightKeyPressed) {"
    "isArrowRightKeyPressed = false;"
    "keyEvent('ArrowRight', 'up');"
    "updateKeyVisual('ArrowRight', false);"
    "}"
    "break;"
    // l
    "case 'l':"
    "if(isLKeyPressed) {"
    "isLKeyPressed = false;"
    "keyEvent('l', 'up');"
    "updateKeyVisual('L', false);"
    "}"
    "break;"
    "}"
    "});"

    "</script>"
    "</body>"
    "</html>";

RobotController::RobotController(const std::string &ssid, const std::string &password)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Initialize Wi-Fi
    station.setCredentials(ssid, password);
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

void RobotController::registerKeyEventHandler(const Key &key, const KeyEvent &event, KeyEventHandler handler)
{
    std::string uri;

    switch (key)
    {
    case Key::ARROW_UP:
        uri = "/ArrowUp-";
        break;
    case Key::ARROW_DOWN:
        uri = "/ArrowDown-";
        break;
    case Key::ARROW_LEFT:
        uri = "/ArrowLeft-";
        break;
    case Key::ARROW_RIGHT:
        uri = "/ArrowRight-";
        break;
    case Key::L:
        uri = "/l-";
        break;
    }

    switch (event)
    {
    case KeyEvent::DOWN:
        uri += "down";
        break;
    case KeyEvent::UP:
        if(key != Key::L)
            uri += "up";
        break;
    }

    contexts[uri] = {handler};

    auto uriHandler = [](httpd_req_t *req) -> esp_err_t {
        KeyEventHandlerContext *context = (KeyEventHandlerContext *)req->user_ctx;

        if(context && context->handler)
        {
            context->handler();

            httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }

        return ESP_FAIL;
    };

    ESP_ERROR_CHECK(server.registerURIHandler(uri, HTTP_GET, uriHandler, &contexts[uri]));
}
