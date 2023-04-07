#include "ServoMotor.h"

ServoMotor::ServoMotor(uint8_t pwmPin, uint8_t pwmChannel, volatile int * encoderCount, uint16_t minAngle, uint16_t maxAngle) : 
Motor(pwmPin, pwmChannel), incriment(encoderCount), minAngle(minAngle), maxAngle(maxAngle){}

ServoMotor::ServoMotor(uint8_t forwardPin, 
                        uint8_t backwardPin, 
                        uint8_t pwmPin, 
                        uint8_t pwmChannel, 
                        volatile int * encoderCount, 
                        uint16_t minAngle, 
                        uint16_t maxAngle) :
Motor(forwardPin, backwardPin, pwmPin, pwmChannel), incriment(encoderCount), minAngle(minAngle), maxAngle(maxAngle){}

void ServoMotor::setUnitPerPulse(float unitPerPulse){
    this->unitPerPulse = unitPerPulse;
}

float ServoMotor::getUnitPerPulse(){
    return unitPerPulse;
}

void ServoMotor::setTargetPosition(float targetPosition){
    this->targetPosition = targetPosition;
}

float ServoMotor::getTargetPosition(){
    return targetPosition;
}

float ServoMotor::getCurrentPosition(){
    return float(encoderCount)*unitPerPulse;
}

void ServoMotor::setPID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float ServoMotor::update(){
    // calculate dt
    unsigned long now = millis();
    int time_diff = int(now - lastTime);
    if(time_diff < 1 || *incriment == 0) return 0.0;
    lastTime = now;
    float dt = float(time_diff)*0.001;

    // update encoder count
    lastEncoderCount = encoderCount;
    encoderCount += *incriment;
    // reset incriment to 0
    *incriment = 0;

    // calculate the error
    float error = targetPosition - float(encoderCount)*unitPerPulse;
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
