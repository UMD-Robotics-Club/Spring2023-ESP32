#pragma once
#include <Wire.h>
#include <SPI.h>
#include "JY901.h"
#include "Arduino.h"
#include "xyzData.h"

class IMU{
    public:

        /**
         * @brief Construct a new IMU object
        */
        IMU() = default;

        /**
         * @brief Initialize the IMU
         * @param None
        */
        void begin();


        /**
         * @brief Get the acceleration data
         * @param None
         * @return xyzData
        */
        xyzData getAccel();

        /**
         * @brief Get the acceleration data
         * @param None
         * @return xyzData
        */
        xyzData getGyro();

        /**
         * @brief Get the acceleration data
         * @param None
         * @return xyzData
        */
        xyzData getMag();

        /**
         * @brief get the composite angle
         * @param None
         * @return xyzData
        */
       xyzData getAngle();

        /**
         * @brief Get the acceleration data
         * @param None
         * @return xyzData
        */
        void calibrate();
        
        /**
         * @brief Print the IMU data
         * @param None
         * @return None
        */
        void print();

    private:
        xyzData accelOffset;
        xyzData gyroOffset;
        xyzData magOffset;

        bool isInitialized = false;
        bool isCalibrated = false;

        static constexpr float deg_to_rad = PI / 180;
        static constexpr float TAU = 2 * PI;

        /**
         * @brief Wrap the angle between -pi and pi
         * @param angle
         * @return float contrained between -pi and pi
        */
        float wrap_angle(float angle);
};