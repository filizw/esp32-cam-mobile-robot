#include "RobotController.h"

#include "esp_log.h"

using Key = RobotController::Key;
using KeyEvent = RobotController::KeyEvent;

extern "C" void app_main(void)
{
    RobotController controller("ssid", "password");

    auto printLog1 = []() -> void {
        ESP_LOGI("Arrow up", "pressed");
    };

    auto printLog2 = []() -> void {
        ESP_LOGI("Arrow up", "released");
    };

    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::DOWN, printLog1);
    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::UP, printLog2);
}
