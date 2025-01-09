#include "Vehicle4WDSteering.hpp"

namespace ab {
    Vehicle4WDSteering::Vehicle4WDSteering() {
        m_turnAngle = 0;
        m_duty = getPwmResolution()*(1.5/20);
    }
    Vehicle4WDSteering::Vehicle4WDSteering(int direction, int speed, int turn) : Vehicle4WD(direction, direction, speed), m_turnAngle(turn) {}

    int Vehicle4WDSteering::getTurnAngle() const {
        return m_turnAngle;
    }

    void Vehicle4WDSteering::turnLeft(int turn) {
        if (turn >= 0 && turn <= 40) {
            m_turnAngle = -turn;

            m_duty = ((1.0+((90.0+m_turnAngle)/180.0))/20.0)*getPwmResolution();
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, m_duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
    }
    void Vehicle4WDSteering::turnRight(int turn) {
        if (turn >= 0 && turn <= 40) {
            m_turnAngle = turn;

            m_duty = ((1.0+((90.0+m_turnAngle)/180.0))/20.0)*getPwmResolution();
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, m_duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
    }
}