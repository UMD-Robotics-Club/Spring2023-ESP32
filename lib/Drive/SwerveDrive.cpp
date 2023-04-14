#include "SwerveDrive.h"

SwerveDrive::SwerveDrive(Motor* driveMotor, Motor* steerMotor, float wheelBase) :
    Drive(driveMotor, steerMotor), wheelBase(wheelBase){}

void SwerveDrive::update(IMU * imu){
    this->motor1->update(); // motor 1 is the drive motor
    this->motor2->update(); // motor 2 is the steer motor
}

void SwerveDrive::setDriveTarget(float velocity, float angle){
    this->targetVelocity = velocity;
    this->targetAngle = angle;
    this->motor1->setVelocity(velocity);
    this->motor2->setVelocity(angle);
}

void SwerveDrive::setPID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}