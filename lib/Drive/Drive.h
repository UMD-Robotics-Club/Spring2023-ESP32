#pragma once

#include "Motor.h"
#include "IMU.h"

class Drive{
    public:
        Drive(Motor* motor1, Motor* motor2);

        /**
         * @brief Initialize the swerve drive
        */
        virtual void begin();

        /**
         * @brief Update the swerve drive
        */
        virtual void update();

        /**
         * @brief Set the velocity of the swerve drive
         * @param motor1Target
         * @param motor2Target
        */
        virtual void setDriveTarget(float motor1Target, float motor2Target);

        /**
         * @brief Get the velocity of the swerve drive
         * @param None
         * @return xyzData
        */
        xyzData getDriveTarget();

    protected:
        Motor* motor1;
        Motor* motor2;
};