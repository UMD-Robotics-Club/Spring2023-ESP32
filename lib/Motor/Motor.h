#pragma once
#include <Arduino.h>

class Motor{
    public:
        /**
         * @brief Construct a new Encoded Motor object
         * @param forwardPin The pin to control the forward direction of the motor
         * @param backwardPin The pin to control the backward direction of the motor
         * @param pwmPin The pin to control the speed of the motor
         * @param incriment The number of steps to incriment the encoder count by. (positive or negative)
        */
        Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t pwmPin, uint8_t pwmChannel);
        Motor(uint8_t pwmPin, uint8_t pwmChannel);
        ~Motor() = default;

        /**
         * @brief Get the current linear velocity of the motor in mm/s
         * @return int The velocity of the motor
         */
        virtual float getVelocity();

        /**
         * @brief run any updates which need to be done continuously
        */
        virtual float update();

        /**
         * @brief Setup the motor
         * @return None.
         */
        virtual void begin();

        /**
         * @brief Set the velocity of the motor
         * @param velocity The velocity to set the motor to. (Range: -255 to 255)
         * @return None.
         */
        virtual void setVelocity(int velocity);
    
    protected:
        // pins
        uint8_t forwardPin;
        uint8_t backwardPin;
        uint8_t pwmPin;
        uint8_t pwmChannel;

        float currentVelocity = 0;
};