#pragma once

#include "Motor.h"
#include "IMU.h"

class SwerveDrive{
    public:
        SwerveDrive(Motor* steerMotor, Motor* driveMotor, float wheelBase);

        /**
         * @brief Initialize the swerve drive
        */
        void begin();

        /**
         * @brief Update the swerve drive
        */
        void update(IMU * imu);

        /**
         * @brief Set the velocity of the swerve drive
         * @param velocity
         * @param angle
        */
        void setVelocity(float velocity, float angle);

        /**
         * @brief Set the PID values for the swerve drive
         * @param kp
         * @param ki
         * @param kd
        */
        void setPID(float kp, float ki, float kd);

        /**
         * @brief Get the velocity of the swerve drive
         * @param None
         * @return xyzData
        */
        xyzData getVelocity();

    private:
        Motor* steerMotor;
        Motor* driveMotor;
        float kp = 1;
        float ki = 0;
        float kd = 0;
        float wheelBase;
        float targetAngle = 0;
        float targetVelocity = 0;

};