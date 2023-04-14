#pragma once

#include "Drive.h"
#include "IMU.h"

class SwerveDrive : public Drive{
    public:
        SwerveDrive(Motor* steerMotor, Motor* driveMotor, float wheelBase);

        /**
         * @brief Update the swerve drive
        */
        void update(IMU * imu);

        void update() override{
            this->update(nullptr);
        };

        /**
         * @brief Set the velocity of the swerve drive
         * @param velocity
         * @param angle
        */
        void setDriveTarget(float velocity, float angle) override;

        /**
         * @brief Set the PID values for the swerve drive
         * @param kp
         * @param ki
         * @param kd
        */
        void setPID(float kp, float ki, float kd);

    private:
        float kp = 1;
        float ki = 0;
        float kd = 0;
        float wheelBase;
        float targetAngle = 0;
        float targetVelocity = 0;

};