#pragma once
#ifndef MILLING_REGULATOR_H
#define MILLING_REGULATOR_H
#include <avr/io.h>

void Regulate() {
    
}



#endif

#include <avr/io.h>
#include <stdio.h>


class LiniarMotorHandler
{
private:
    uint8_t &directionPin;
    uint8_t &pulsePin;
    float resolutionmmPerStep;
public:
    LiniarMotorHandler(uint8_t &dPin, uint8_t &pPin): directionPin(dPin), pulsePin(pPin) {}
    ~LiniarMotorHandler() {}
    void driveMotor(float distancemm, float speed);
};



void LiniarMotorHandler::driveMotor(float distancemm, float speed) {
    int pulses = (distancemm/resolutionmmPerStep);
    int dir = 0; // forward
    if(pulses < 0) {
        dir = 1; // backward
    }

    

}