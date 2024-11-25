#pragma once

#include <iostream>

class Vehicle4WD {
private:
    //0 == stop, 1 == forward, 2 == backward
    int m_leftWheels;
    int m_rightWheels;

public:
    Vehicle4WD(int left, int right) : m_leftWheels(left), m_rightWheels(right) {}

    void driveForward() {
        m_leftWheels = 1;
        m_rightWheels = 1;
    }

    void driveBackward() {
        m_leftWheels = 2;
        m_rightWheels = 2;
    }
    //car1.turnLeft(angle/percentage)
    virtual void turnLeft() {
        m_leftWheels = 2;
        m_rightWheels = 1;
    }

    virtual void turnRight() {
        m_leftWheels = 1;
        m_rightWheels = 2;
    }
};