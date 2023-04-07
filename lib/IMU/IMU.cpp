#include "IMU.h"

void IMU::begin(){
    JY901.StartIIC();
    calibrate();
    Serial.println("IMU online!");
    isInitialized = true;
}

void IMU::calibrate(){
    if(!isInitialized){
        Serial.println("IMU not initialized. Cannot calibrate.");
        return;
    }

    Serial.println("Calibrating Gyro");
    // wait for a little while to allow the gyro to settle.
    delay(10);
    // add up 1000 gyro readings
    int i = 0;
    while(i < 1000){
        gyroOffset = gyroOffset + getGyro();
    }
    // divide the sum by 1000 to get the average
    gyroOffset = gyroOffset / 1000.0;

    i = 0;
    while(i < 1000){
        accelOffset = accelOffset + getAccel();
    }
    accelOffset = accelOffset / 1000.0;

    Serial.print("Accel Offset: ");
    accelOffset.print();
    Serial.println();
    Serial.print("Gyro Offset: ");
    gyroOffset.print();
    Serial.println();
    Serial.print("Mag Offset: ");
    magOffset.print();
    Serial.println();
    Serial.println("Calibration complete");
}

void IMU::print(){
    if(!isInitialized){
        Serial.println("IMU not initialized. Cannot print.");
        return;
    }
    Serial.print("!ACC,");
    getAccel().print();
    Serial.println(";");
    Serial.print("!GYR,");
    getGyro().print();
    Serial.println(";");
    Serial.print("!MAG,");
    getMag().print();
    Serial.println(";");
    Serial.print("!ANG,");
    getAngle().print();
    Serial.println(";");
}

// bound between -pi and pi
float IMU::wrap_angle(float angle) {
    // First, wrap the angle between -2*pi and 2*pi radians
    angle = fmod(angle + PI, TAU) - PI;

    // Then, bound the angle between -pi and pi radians
    if (angle < -PI) {
        angle += TAU;
    } else if (angle > PI) {
        angle -= TAU;
    }

    return angle;
}

xyzData IMU::getAccel(){
    JY901.GetAcc();
    xyzData accel = xyzData((float)JY901.stcAcc.a[0]/32768*16, (float)JY901.stcAcc.a[1]/32768*16, (float)JY901.stcAcc.a[2]/32768*16);
    if(isCalibrated) return accel - accelOffset;
    return accel;
}

xyzData IMU::getGyro(){
    JY901.GetGyro();
    xyzData gyro = xyzData((float)JY901.stcGyro.w[0]/32768*2000, (float)JY901.stcGyro.w[1]/32768*2000, (float)JY901.stcGyro.w[2]/32768*2000);
    if(isCalibrated) return gyro - gyroOffset;
    return gyro;
}

xyzData IMU::getMag(){
    JY901.GetMag();
    xyzData mag = xyzData(JY901.stcMag.h[0], JY901.stcMag.h[1], JY901.stcMag.h[2]);
    if(isCalibrated) return mag - magOffset;
    return mag;
}

xyzData IMU::getAngle(){
    JY901.GetAngle();
    return xyzData((float)JY901.stcAngle.Angle[0]/32768*180, (float)JY901.stcAngle.Angle[1]/32768*180, (float)JY901.stcAngle.Angle[2]/32768*180);
}