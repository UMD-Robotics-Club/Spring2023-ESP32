#include "SwerveDrive.h"

SwerveDrive::SwerveDrive(Motor* steerMotor, Motor* driveMotor, float wheelBase) :
    steerMotor(steerMotor),driveMotor(driveMotor), wheelBase(wheelBase){}


void SwerveDrive::begin(){
    steerMotor->begin();
    driveMotor->begin();
}

void SwerveDrive::update(IMU * imu){
    this->steerMotor->update();
    this->driveMotor->update();
}

void SwerveDrive::setVelocity(float velocity, float angle){
    this->targetVelocity = velocity;
    this->targetAngle = angle;
}

void SwerveDrive::setPID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

xyzData SwerveDrive::getVelocity(){
    return this->driveMotor->getVelocity();
}