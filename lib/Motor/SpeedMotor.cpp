#include "SpeedMotor.h"

SpeedMotor::SpeedMotor(uint8_t pwmPin, uint8_t pwmChannel, volatile int * encoderCount, uint16_t minAngle, uint16_t maxAngle) : PositionMotor(pwmPin, pwmChannel, encoderCount, minAngle, maxAngle){}

SpeedMotor::SpeedMotor(uint8_t forwardPin, uint8_t backwardPin, uint8_t pwmPin, uint8_t pwmChannel, volatile int * encoderCount, uint16_t minAngle, uint16_t maxAngle) : PositionMotor(forwardPin, backwardPin, pwmPin, pwmChannel, encoderCount, minAngle, maxAngle){}

// TODO: Test if this ever gets called
float SpeedMotor::getError(){
    return targetPosition - float(encoderCount - lastEncoderCount)*unitPerPulse/dt;
}