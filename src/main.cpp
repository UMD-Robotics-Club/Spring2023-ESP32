#include <Arduino.h>
#include "PINOUT.h"
#include "SerialMessage.h"
#include "SwerveDrive.h"
#include "BluetoothSerialMessage.h"


// Sonar sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN);
//Servo servo;

volatile int leftEncoderCount = 0;
volatile int rightEncoderCount = 0;

// Incriment / Decrement depending on encoder state during an interrupt
void leftEncoderInc(){
  if (digitalRead(LEFT_ENC_A_PIN) == digitalRead(LEFT_ENC_B_PIN)) {
    leftEncoderCount++;
    return;
  }
  leftEncoderCount--;
  
}

// Incriment / Decrement depending on encoder state during an interrupt
void rightEncoderInc(){
  if (digitalRead(RIGHT_ENC_A_PIN) == digitalRead(RIGHT_ENC_B_PIN)) {
    rightEncoderCount--;
  } else {
    rightEncoderCount++;
  }
}

// TODO: Switch these to Position/Velocity motors once they have encoders
// the motor object that handles controlling the steering. This object can be subbed for A Position or Speed motor object
Motor steerMotor(STEER_MOTOR_PWM_PIN, 0);
// the motor object that handles controlling the drive. This object can be subbed for A Position or Speed motor object
Motor driveMotor(DRIVE_MOTOR_PWM_PIN, 1);
// this class takes in two motor objects and the distance between the two wheels to synchronize the motor's control systems.
// It's reocmmended that you use this class instead of the motor objects directly.
// TODO: switch this to swerve drive when we have encoded motors
Drive wheels(&driveMotor, &steerMotor); //151/1.86

// Object to handle IMU communication
IMU imu;

// Object to handle serial communication and decode the commands
SerialMessage ser;
// This is the raw bluetooth serial object
BluetoothSerial bleSerial;
// this is a wrapper around the bluetooth serial object that does the same thing as SerialMessage, but over bluetooth
BluetoothSerialMessage bleSer(&bleSerial);

void setup() {
  Serial.begin(115200);
  bleSer.init();
  imu.begin();

  Serial.println("Starting up...");
  bleSerial.println("Starting up...");

  //TODO: set these things when we switch over to an encoded motor
  // steerMotor.setUnitsPerStep(1);
  // steerMotor.setPID(1, 0, 0);
  // driveMotor.setUnitsPerStep(1);
  // driveMotor.setPID(1, 0, 0);

  wheels.begin(); // initialize the motors and the drive system
  // TODO: enable this when we switch over to an encoded motor
  // wheels.setPID(0.2, 12, 40); // TODO: this currently doesn't do anything behind the scenes. Probably fix that.

  // attach the interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A_PIN), leftEncoderInc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A_PIN), rightEncoderInc, CHANGE);
  
  Serial.println("Started");
  bleSerial.println("Started");
}

/**
  *@brief Take the serial arguments and do the appropriate action
  *@param args The array of arguments from the serial message
  *@param args_length The length of the array of arguments
*/
void doSerialCommand(int * args, int args_length) {
  switch (args[0]) {
    case ERR:{ // Note, the constants like ERR, MOTOR_READ, etc are defined in SerialMessage.h
      Serial.println("!ERR;");
      bleSerial.println("!ERR;");
      break;
    }
    // sends the current pose of the robot over serial
    case MOTOR_READ:{
      Serial.print("!MTR,");
      bleSerial.print("!MTR,");
      // print the current pose
      xyzData pose = wheels.getDriveTarget();
      for(auto i = 0; i < 3; i++) {
        Serial.print(pose.get(i),4);
        bleSerial.print(pose.get(i),4);
        if(i < 2) {
          Serial.print(",");
          bleSerial.print(",");
        }
      }

      Serial.println(";");
      bleSerial.println(";");
      break;
    }
    // sets the target pose of the robot
    case MOTOR_WRITE:{
      if(args_length < 3) break;
      Serial.print("!MTR_WRT,");
      bleSerial.print("!MTR_WRT,");
      for(int i = 1; i < args_length; i++) {
        Serial.print(args[i]);
        bleSerial.print(args[i]);
        Serial.print(",");
        bleSerial.print(",");
      }
      Serial.println(";");
      bleSerial.println(";");
      // set the new target pose
      wheels.setDriveTarget(args[1], args[2]);
      break;
    }
    // TODO: enable this when we get encoders on the wheels
    // sets the PID constants for the drive system
    // case SET_STATE_CONSTANTS:{
    //   if(args_length < 4) break;
    //   wheels.setPID(args[1], args[2], args[3]);
    //   break;
    // }
    // TODO: enable this when we get encoders on the wheels
    // case SET_MOTOR_PID:{
    //   if(args_length < 5) break;
    //   if(args[1] == 0){
    //     steerMotor.setPID(args[2], args[3], args[4]);
    //   }
    //   else if(args[1] == 1){
    //     driveMotor.setPID(args[2], args[3], args[4]);
    //   }
    //   break;
    // }

    default:{
      Serial.println("ERR");
      break;
    }
  }
}

unsigned long timer = 0;

void loop() {
  // update the serial if there's anything available
  ser.update();
  bleSer.update();
  // process the new serial data if there is any
  if (ser.isNewData()) {
    int * args = ser.getArgs();
    int args_length = ser.getPopulatedArgs();

    doSerialCommand(args, args_length);
    ser.clearNewData();
  }
  // process the new bluetooth serial data if there is any
  if(bleSer.isNewData()) {
    int * args = bleSer.getArgs();
    int args_length = bleSer.getPopulatedArgs();

    doSerialCommand(args, args_length);
    bleSer.clearNewData();
  }

  // update the wheels with the new pose
  wheels.update();
}
