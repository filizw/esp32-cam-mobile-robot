#pragma once

#include "Vehicle4WD.hpp"

namespace ab {
    /**
     * This class is derived from Vehicle4WD so the turning mechanism using servos
     * can be implemented
     */
    class Vehicle4WDSteering : public Vehicle4WD {
    private:
        int m_turnAngle;    // -40° - 40°
        int m_duty;

    public:
        /**
         * Default contructor
         */
        Vehicle4WDSteering();
        /**
         * Parameterized constructor that takes direction of driving, speed and turn angle
         */
        Vehicle4WDSteering(int direction, int speed, int turn);
        //~Vehicle4WDSteering() {}

        int getTurnAngle() const;

        /**
         * Overrided function to turn vehicle
         * it takes angle in degrees, the set range is 0-40 left and right, so overall -40 to 40 degrees
         * but it can be changed in code for different vehicles
         * this range works best for our robot
         */
        void turnLeft(int turn) override;
        /**
         * The same as function turnLeft() but right
         */
        void turnRight(int turn) override;
    };
}