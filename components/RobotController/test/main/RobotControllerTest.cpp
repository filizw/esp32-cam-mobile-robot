#include "RobotController.h"

#include "driver/gpio.h"
#include "esp_log.h"

using Key = RobotController::Key;
using KeyEvent = RobotController::KeyEvent;

auto printLog1 = []() -> void {
    ESP_LOGI("Arrow up", "pressed");
};

auto printLog2 = []() -> void {
    ESP_LOGI("Arrow up", "released");
};

auto toggleLed = []() -> void {
    static auto ledStatus = 0;

    ledStatus = !ledStatus;
    gpio_set_level(GPIO_NUM_4, ledStatus);
};

extern "C" void app_main(void)
{
    RobotController controller("ssid", "password");

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::DOWN, printLog1);
    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::UP, printLog2);
    controller.registerKeyEventHandler(Key::L, KeyEvent::DOWN, toggleLed);
}
