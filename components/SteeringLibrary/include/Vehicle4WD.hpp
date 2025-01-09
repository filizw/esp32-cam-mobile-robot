#pragma once

#include <cmath>
#include <vector>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_gpio.h"
#include "soc/gpio_sig_map.h"

/**
 * define pins used for LED, motors and servos
 * MOTOR_IN1 and MOTOR_IN2 control right wheels
 * MOTOR_IN3 and MOTOR_IN4 control left wheels
 * one GPIO pin controls 2 servos
 */
constexpr gpio_num_t LED_PIN = GPIO_NUM_2;

constexpr gpio_num_t MOTOR_IN1_PIN = GPIO_NUM_26;
constexpr gpio_num_t MOTOR_IN2_PIN = GPIO_NUM_25;
constexpr gpio_num_t MOTOR_IN3_PIN = GPIO_NUM_33;
constexpr gpio_num_t MOTOR_IN4_PIN = GPIO_NUM_32;
constexpr gpio_num_t MOTOR_ENA_PIN = GPIO_NUM_14;
constexpr gpio_num_t MOTOR_ENB_PIN = GPIO_NUM_27;

constexpr gpio_num_t SERVO_STEERING_PIN = GPIO_NUM_12;

namespace ab {
    class Vehicle4WD {
    private:
        int m_leftWheels;   //0 == stop, 1 == forward, 2 == backward
        int m_rightWheels;

        int m_speed;    //working range 40 - 100

        int m_pwmResolution;

        void leftWheelsDirection(int dir);
        void rightWheelsDirection(int dir);

    public:
        /**
         * Default constructor
         */
        Vehicle4WD();
        /**
         * Constructor that takes direction of left and right
         * wheels and speed
         * 0 - stop, 1 - forward, 2 - backward
         */
        Vehicle4WD(int left, int right, int speed);

        /**
         * The destructor is virtual so it is destroyed correctly
         */
        virtual ~Vehicle4WD();

        /**
         * Setting speed of vehicle
         * the range is 0-100 but recommended range is 40-100
         */
        void setSpeed(int speed);

        /**
         * Get the resolution of PWM signal that is running motors and servos
         */
        int getPwmResolution() const;

        /**
         * Get current set speed of the vehicle
         */
        int getSpeed() const;

        /**
         * Get vector of 2 elements that are the rotating directions of left and right wheels
         */
        std::vector<int> getDirection() const;

        /**
         * Set the duration of driving forward in milliseconds
         * when 0 is passed the drive is infinite
         */
        void driveForward(int duration);

        /**
         * The same as driveForward(), but backwards
         */
        void driveBackwards(int duration);

        /**
         * Stop the vehicle
         */
        void stop();

        /**
         * Set the duration of turning left (tank like) in milliseconds
         * when 0 is passed the turn is infinite
         * 
         * These turn functions are virtual so the turning can be implemented in more traditional way
         * in the Vehicle4WDSteering class
         */
        virtual void turnLeft(int turn);

        /**
         * The same as turnLeft() but right
         */
        virtual void turnRight(int turn);

        /**
         * Makes square figure 0 - left, 1 - right
         * duration means how long the side will be
         */
        void makeSquare(int lr, int duration);
    };
}