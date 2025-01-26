#include "RobotController.h"
#include "Vehicle4WDSteering.hpp"

using Key = RobotController::Key;
using KeyEvent = RobotController::KeyEvent;

extern "C" void app_main(void)
{
    ab::Vehicle4WDSteering* car1 = new ab::Vehicle4WDSteering(0, 50, 0);
    RobotController controller("*****", "*****");

    auto arrowUpPressed = [=]() -> void {
        ESP_LOGI("Arrow up", "pressed");

        car1->driveForward(0);
    };

    auto arrowUpReleased = [=]() -> void {
        ESP_LOGI("Arrow up", "released");

        car1->stop();
    };

    auto arrowDownPressed = [=]() -> void {
        ESP_LOGI("Arrow down", "pressed");

        car1->driveBackwards(0);
    };

    auto arrowDownReleased = [=]() -> void {
        ESP_LOGI("Arrow down", "released");

        car1->stop();
    };

    auto arrowLeftPressed = [=]() -> void {
        ESP_LOGI("Arrow left", "pressed");

        car1->turnLeft(40);
    };

    auto arrowLeftReleased = [=]() -> void {
        ESP_LOGI("Arrow left", "released");

        car1->turnLeft(0);
    };

    auto arrowRightPressed = [=]() -> void {
        ESP_LOGI("Arrow right", "pressed");

        car1->turnRight(40);
    };

    auto arrowRightReleased = [=]() -> void {
        ESP_LOGI("Arrow right", "released");

        car1->turnRight(0);
    };

    auto toggleLed = [=]() -> void {
        static auto speedStatus = 0;
        speedStatus = !speedStatus;

        if (speedStatus) {
            car1->setSpeed(100);
            ESP_LOGI("L", "pressed");
        } else {
            car1->setSpeed(50);
            ESP_LOGI("L", "pressed");
        }
    };

    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::DOWN, arrowUpPressed);
    controller.registerKeyEventHandler(Key::ARROW_UP, KeyEvent::UP, arrowUpReleased);
    controller.registerKeyEventHandler(Key::ARROW_DOWN, KeyEvent::DOWN, arrowDownPressed);
    controller.registerKeyEventHandler(Key::ARROW_DOWN, KeyEvent::UP, arrowDownReleased);
    controller.registerKeyEventHandler(Key::ARROW_LEFT, KeyEvent::DOWN, arrowLeftPressed);
    controller.registerKeyEventHandler(Key::ARROW_LEFT, KeyEvent::UP, arrowLeftReleased);
    controller.registerKeyEventHandler(Key::ARROW_RIGHT, KeyEvent::DOWN, arrowRightPressed);
    controller.registerKeyEventHandler(Key::ARROW_RIGHT, KeyEvent::UP, arrowRightReleased);
    controller.registerKeyEventHandler(Key::L, KeyEvent::DOWN, toggleLed);
    controller.registerKeyEventHandler(Key::L, KeyEvent::UP, toggleLed);
}