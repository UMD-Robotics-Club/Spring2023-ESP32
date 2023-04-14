#include "SwerveDrive.h"

SwerveDrive::SwerveDrive(SpeedMotor* driveMotor, PositionMotor* steerMotor, float wheelBase) :
    Drive(driveMotor, steerMotor), driveMotor(driveMotor), steerMotor(steerMotor), wheelBase(wheelBase){}

void SwerveDrive::update(IMU * imu){
    this->motor1->update(); // motor 1 is the drive motor
    this->motor2->update(); // motor 2 is the steer motor
}

void SwerveDrive::setDriveTarget(float velocity, float angle){
    this->targetVelocity = velocity;
    this->targetAngle = angle;
    this->driveMotor->setTarget(velocity);
    this->steerMotor->setTarget(angle);
}

void SwerveDrive::setPID(float kp, float ki, float kd){
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}