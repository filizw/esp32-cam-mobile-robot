#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Vehicle4WD.hpp"
#include "Vehicle4WDSteering.hpp"
#include "driver/ledc.h"

#define LED_PIN 2

#define MOTOR_IN1_PIN 26
#define MOTOR_IN2_PIN 25
#define MOTOR_IN3_PIN 33
#define MOTOR_IN4_PIN 32
#define MOTOR_ENA_PIN 14
#define MOTOR_ENB_PIN 27

#define SERVO_STEERING_PIN 12


extern "C" void app_main() {
    gpio_set_direction((gpio_num_t)LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_IN1_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_IN2_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_IN3_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_IN4_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_ENA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)MOTOR_ENB_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)SERVO_STEERING_PIN, GPIO_MODE_OUTPUT);
/*
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
    ledc_channel_0.gpio_num = SERVO_STEERING_PIN;
    ledc_channel_0.duty = 0;
    ledc_channel_0.hpoint = 0;

    ledc_channel_config_t ledc_channel_1 = {};
    ledc_channel_1.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_1.channel = LEDC_CHANNEL_1;
    ledc_channel_1.timer_sel = LEDC_TIMER_0;
    ledc_channel_1.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_1.gpio_num = MOTOR_ENA_PIN;
    ledc_channel_1.duty = 0;
    ledc_channel_1.hpoint = 0;

    ledc_timer_config(&ledc_timer_0);
    ledc_channel_config(&ledc_channel_0);
    ledc_channel_config(&ledc_channel_1);

    while(1) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 491);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(pdMS_TO_TICKS(2000));

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 737);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 49);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
*/
    Vehicle4WDSteering car1(1, 50, 0);
    //car1.driveForward(2000);
    //car1.turnLeft(40);
    //car1.driveForward(1000);
    //car1.turnRight(20);
    //car1.driveBackward(1500);
    //car1.turnLeft(0);
    /*
    while(1) {
    gpio_set_level((gpio_num_t)LED_PIN, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level((gpio_num_t)LED_PIN, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    */
}