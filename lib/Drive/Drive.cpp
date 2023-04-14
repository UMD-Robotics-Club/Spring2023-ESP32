#include "Drive.h"

Drive::Drive(Motor* motor1, Motor* motor2) :
    motor1(motor1),motor2(motor2){}

void Drive::begin(){
    motor1->begin();
    motor2->begin();
}

void Drive::update(){
    this->motor1->update();
    this->motor2->update();
}

void Drive::setDriveTarget(float motor1Target, float motor2Target){
    this->motor1->setTargetVelocity(motor1Target);
    this->motor2->setTargetVelocity(motor2Target);
}

xyzData Drive::getDriveTarget(){
    return xyzData(this->motor1->getTargetVelocity(), this->motor2->getTargetVelocity(), 0);
}
