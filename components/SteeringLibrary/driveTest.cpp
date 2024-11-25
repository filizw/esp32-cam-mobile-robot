#include <iostream>
#include "Vehicle4WD.hpp"
#include "Vehicle4WDSteering.hpp"

//#include "driver/gpio.h"

#define MOTOR_IN1_PIN 32
#define MOTOR_IN2_PIN 33
#define MOTOR_IN3_PIN 25
#define MOTOR_IN4_PIN 26
#define MOTOR_ENA_PIN 27
#define MOTOR_ENB_PIN 14

#define SERVO_STEERING_PIN 12

extern "C" void app_main() {
    Vehicle4WD car(0, 0);
}