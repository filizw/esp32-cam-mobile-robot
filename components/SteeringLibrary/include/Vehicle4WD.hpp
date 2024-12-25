#pragma once

#include <cmath>
#include <vector>

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_gpio.h"
#include "soc/gpio_sig_map.h"

#define LED_PIN 2

#define MOTOR_IN1_PIN 26
#define MOTOR_IN2_PIN 25
#define MOTOR_IN3_PIN 33
#define MOTOR_IN4_PIN 32
#define MOTOR_ENA_PIN 14
#define MOTOR_ENB_PIN 27

#define SERVO_STEERING_PIN 12

class Vehicle4WD {
private:
    int m_leftWheels;   //0 == stop, 1 == forward, 2 == backward
    int m_rightWheels;

    int m_speed;    //working range 40 - 100

    int m_pwmResolution;

public:
    Vehicle4WD(int left = 0, int right = 0, int speed = 50) : m_leftWheels(left), m_rightWheels(right), m_speed(speed) {
        gpio_set_direction((gpio_num_t)LED_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_IN1_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_IN2_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_IN3_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_IN4_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_ENA_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)MOTOR_ENB_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)SERVO_STEERING_PIN, GPIO_MODE_OUTPUT);

        ledc_timer_config_t ledc_timer_0 = {};
        ledc_timer_0.speed_mode = LEDC_LOW_SPEED_MODE;  // Tryb niskiej prędkości (wolniejsze czasy odpowiedzi)
        ledc_timer_0.timer_num = LEDC_TIMER_0;         // Wybór jednego z dostępnych timerów (LEDC_TIMER_0 do LEDC_TIMER_3)
        ledc_timer_0.duty_resolution = LEDC_TIMER_13_BIT; // Rozdzielczość PWM (2^13 = 8192 poziomów)
        ledc_timer_0.freq_hz = 50;                   // Częstotliwość sygnału PWM w Hz
        ledc_timer_0.clk_cfg = LEDC_AUTO_CLK;

        ledc_channel_config_t ledc_channel_0 = {};
        ledc_channel_0.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel_0.channel = LEDC_CHANNEL_0;
        ledc_channel_0.timer_sel = LEDC_TIMER_0;
        ledc_channel_0.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_0.gpio_num = MOTOR_ENA_PIN;
        ledc_channel_0.duty = 0;
        ledc_channel_0.hpoint = 0;

        ledc_channel_config_t ledc_channel_1 = {};
        ledc_channel_1.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel_1.channel = LEDC_CHANNEL_1;
        ledc_channel_1.timer_sel = LEDC_TIMER_0;
        ledc_channel_1.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_1.gpio_num = SERVO_STEERING_PIN;
        ledc_channel_1.duty = 0;
        ledc_channel_1.hpoint = 0;

        m_pwmResolution = pow(2, ledc_timer_0.duty_resolution);

        ledc_timer_config(&ledc_timer_0);
        ledc_channel_config(&ledc_channel_0);
        ledc_channel_config(&ledc_channel_1);

        esp_rom_gpio_connect_out_signal(MOTOR_ENB_PIN, LEDC_LS_SIG_OUT0_IDX, false, false);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, m_speed*m_pwmResolution/100);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 615);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    }

    void setSpeed(int speed) {
        if (speed >= 0 && speed <= 100) {
            m_speed = speed;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, m_speed*m_pwmResolution/100);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
    }

    int getPwmResolution() const {
        return m_pwmResolution;
    }

    int getSpeed() const {
        return m_speed;
    }

    std::vector<int> getDirection() const {
        std::vector<int> dirVector = {m_leftWheels, m_rightWheels};
        return dirVector;
    }

    //car1.driveForward(duration) 0 == infinite drive
    void driveForward(int duration) {
        if (duration > 0) {
            m_leftWheels = 1;
            m_rightWheels = 1;
            
            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);

            vTaskDelay(duration/portTICK_PERIOD_MS);

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
        } else {
            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
        }
    }

    void driveBackward(int duration) {
        if (duration > 0) {
            m_leftWheels = 2;
            m_rightWheels = 2;

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 1);

            vTaskDelay(duration/portTICK_PERIOD_MS);

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
        } else {
            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 1);            
        }
    }

    void stop() {
        m_leftWheels = 0;
        m_rightWheels = 0;

        gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
        gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
        gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
        gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
    }
    //car1.turnLeft(angle/percentage) 0 == infinite turn
    virtual void turnLeft(int turn) {
        if (turn >= 0) {
            m_leftWheels = 2;
            m_rightWheels = 1;

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 1);

            vTaskDelay(turn/portTICK_PERIOD_MS);

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
        }
    }

    virtual void turnRight(int turn) {
        if (turn >= 0) {
            m_leftWheels = 1;
            m_rightWheels = 2;

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 1);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);

            vTaskDelay(turn/portTICK_PERIOD_MS);

            gpio_set_level((gpio_num_t)MOTOR_IN1_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN2_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN3_PIN, 0);
            gpio_set_level((gpio_num_t)MOTOR_IN4_PIN, 0);
        }
    }
};