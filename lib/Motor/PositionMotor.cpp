#include "PositionMotor.h"

PositionMotor::PositionMotor(uint8_t pwmPin, uint8_t pwmChannel, volatile int * encoderCount, uint16_t minAngle, uint16_t maxAngle) : 
Motor(pwmPin, pwmChannel), incriment(encoderCount), minAngle(minAngle), maxAngle(maxAngle){}

PositionMotor::PositionMotor(uint8_t forwardPin, 
                        uint8_t backwardPin, 
                        uint8_t pwmPin, 
                        uint8_t pwmChannel, 
                        volatile int * encoderCount, 
                        uint16_t minAngle, 
                        uint16_t maxAngle) :
Motor(forwardPin, backwardPin, pwmPin, pwmChannel), incriment(encoderCount), minAngle(minAngle), maxAngle(maxAngle){}

void PositionMotor::setUnitPerPulse(float unitPerPulse){
    this->unitPerPulse = unitPerPulse;
}

float PositionMotor::getUnitPerPulse(){
    return unitPerPulse;
}

void PositionMotor::setTargetPosition(float targetPosition){
    this->targetPosition = targetPosition;
}

float PositionMotor::getTargetPosition(){
    return targetPosition;
}

float PositionMotor::getCurrentPosition(){
    return float(encoderCount)*unitPerPulse;
}

void PositionMotor::setPID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float PositionMotor::update(){
    // calculate dt
    unsigned long now = millis();
    int time_diff = int(now - lastTime);
    if(time_diff < 1 || *incriment == 0) return 0.0;
    lastTime = now;
    this->dt = float(time_diff)*0.001;

    // update encoder count
    lastEncoderCount = encoderCount;
    encoderCount += *incriment;
    // reset incriment to 0
    *incriment = 0;

    // calculate the error
    float error = getError();
    // calculate the integral
    integral += error*dt;
    // calculate the derivative
    float derivative = (error - lastError)/dt;
    lastError = error;

    // calculate the velocity
    float velocity = kp*error + ki*integral + kd*derivative;
    // set the velocity
    setVelocity(velocity);

    // return the current position
    return getCurrentPosition();
}

float PositionMotor::getError(){
    return targetPosition - getCurrentPosition();
}
