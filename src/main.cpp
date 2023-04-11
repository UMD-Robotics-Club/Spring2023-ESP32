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

Motor steerMotor(STEER_MOTOR_PWM_PIN, 0);
Motor driveMotor(DRIVE_MOTOR_PWM_PIN, 1);
SwerveDrive wheels(&steerMotor, &driveMotor, 1); //151/1.86
IMU imu;

// Object to handle serial communication
SerialMessage ser;
BluetoothSerial bleSerial;
BluetoothSerialMessage bleSer(&bleSerial);

void setup() {
  Serial.begin(115200);
  bleSer.init();
  delay(1000);
  imu.begin();

  Serial.println("Starting up...");
  bleSerial.println("Starting up...");
  //TODO: set the units per step for the motors when we get encoders.
  wheels.begin();
  wheels.setPID(0.2, 12, 40);
  // wheels.setPID(0, 0, 0);
  // // attach the interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A_PIN), leftEncoderInc, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A_PIN), rightEncoderInc, CHANGE);
  
  // servo.attach(servo_pin);
  // servo.write(90);
  // sonar.attachServo(servo);
  
  // sonar.enableScanMode(false);
  Serial.println("Started");
  bleSerial.println("Started");
}

// takes the serial args and executes the command
void doSerialCommand(int * args, int args_length) {
  switch (args[0]) {
    case ERR:{
      Serial.println("!ERR;");
      bleSerial.println("!ERR;");
      break;
    }
    case MOTOR_READ:{
      Serial.print("!MTR,");
      bleSerial.print("!MTR,");
      // print the current pose
      xyzData pose = wheels.getVelocity();
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
      wheels.setVelocity(args[1], args[2]);
      break;
    }
    case SET_STATE_CONSTANTS:{
      if(args_length < 4) break;
      wheels.setPID(args[1], args[2], args[3]);
      break;
    }
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
  ser.update();
  bleSer.update();
  if (ser.isNewData()) {
    int * args = ser.getArgs();
    int args_length = ser.getPopulatedArgs();

    doSerialCommand(args, args_length);
    ser.clearNewData();
  }

  if(bleSer.isNewData()) {
    int * args = bleSer.getArgs();
    int args_length = bleSer.getPopulatedArgs();

    doSerialCommand(args, args_length);
    bleSer.clearNewData();
  }
  wheels.update(&imu);
}
