#pragma once

#include <iostream>
#include "Vehicle4WD.hpp"

class Vehicle4WDSteering : public Vehicle4WD {
private:
    int m_turnAngle;

public:
    Vehicle4WDSteering(int direction, int turn) : Vehicle4WD(direction, direction), m_turnAngle(turn) {}

    void turnLeft() override {
        ;;
    }

    void turnRight() override {
        ;;
    }
};