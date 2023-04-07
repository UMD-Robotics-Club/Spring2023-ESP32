#include "Motor.h"

Motor::Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t pwmPin, uint8_t pwmChannel) : 
forwardPin(forwardPin), backwardPin(backwardPin), pwmPin(pwmPin), pwmChannel(pwmChannel){}

Motor::Motor(uint8_t pwmPin, uint8_t pwmChannel) : forwardPin(-1), backwardPin(-1), pwmPin(pwmPin), pwmChannel(pwmChannel){}

float Motor::getVelocity(){
    return this->currentVelocity;
}

void Motor::begin(){  
    ledcAttachPin(pwmPin, pwmChannel);
    ledcSetup(pwmChannel, 24000, 8);
    if(forwardPin != -1 && backwardPin != -1){
        pinMode(forwardPin, OUTPUT);
        pinMode(backwardPin, OUTPUT);
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, LOW);
    }
    Serial.print("Motor::begin() pwmChannel: ");
    Serial.print(pwmChannel);
    Serial.print(" pwmPin: ");
    Serial.print(pwmPin);
    if(forwardPin != -1 && backwardPin != -1){
        Serial.print(" forwardPin: ");
        Serial.print(forwardPin);
        Serial.print(" backwardPin: ");
        Serial.println(backwardPin);
    }
}

float Motor::update(){
    return currentVelocity;
}

void Motor::setVelocity(int velocity){
    // make sure the velocity is in the range of -255 to 255
    if(velocity > 255){
        velocity = 255;
    } else if(velocity < -255){
        velocity = -255;
    }

    currentVelocity = (float)velocity;

    if(velocity > 0){
        digitalWrite(forwardPin, HIGH);
        digitalWrite(backwardPin, LOW);
    } else if(velocity < 0){
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, HIGH);
    } else {
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, LOW);
    }
    ledcWrite(pwmChannel, abs(velocity));
}
