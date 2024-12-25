#pragma once

#include "Vehicle4WD.hpp"

class Vehicle4WDSteering : public Vehicle4WD {
private:
    int m_turnAngle;    // -40° - 40°

public:
    Vehicle4WDSteering(int direction, int speed, int turn = 0) : Vehicle4WD(direction, direction, speed), m_turnAngle(turn) {}

    int getTurnAngle() const {
        return m_turnAngle;
    }

    void turnLeft(int turn) override {
        if (turn >= 0 && turn <= 40) {
            m_turnAngle = -turn;

            int duty = ((1.0+((90.0+m_turnAngle)/180.0))/20.0)*getPwmResolution();
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
    }

    void turnRight(int turn) override {
        if (turn >= 0 && turn <= 40) {
            m_turnAngle = turn;

            int duty = ((1.0+((90.0+m_turnAngle)/180.0))/20.0)*getPwmResolution();
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
    }
};