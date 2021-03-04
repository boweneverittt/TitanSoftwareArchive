#ifndef ORIENTATION_H
#define ORIENTATION_H
 
#include "Quaternion.h"
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>

struct LocalOrientation
{
    float x;
    float y;
    float z;
};

struct LSM9DS1RawData
{
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float magX;
    float magY;
    float magZ;
};

struct LSM9DS1CalGyroData
{
    float gXArr;
    float gYArr;
    float gZArr;

    float gXOffset;
    float gYOffset;
    float gZOffset;

    float gXOut;
    float gYOut;
    float gZOut;
};

struct EulerAngles
{
    float yaw;
    float pitch;
    float roll;
};
 
class Orientation
{
public:
    Quaternion orientation; 
    Quaternion worldGravity;
 
    Orientation() { orientation = Quaternion(); }
    EulerAngles quaternionToEuler(Quaternion q);

    /*
    IMPORTANT NOTES FOR UPDATE FUNCTION:
    dt is measured in seconds
    Conversion from milliseconds to seconds is / 1,000, microseconds to seconds is / 1,000,000

    After getting gyro measurements, be sure to convert them into the proper coordinate space before passing to update
    Example of correct coordinate space can be found at https://drive.google.com/file/d/1mANkLke7mAjCwUEb6_7WpxMwtRImbzFx/view?usp=sharing
    In the example, rotation around X is roll, rotation around Y is pitch, and rotation around Z is yaw
    Linear arrow points in drection of positive acceleration
    Rotational arrow points in direction of positive roll

    Same coordinate frame applies to euler angles output
    */

    void update(float yaw, float pitch, float roll, float dt);
    void update(EulerAngles gyroMeasure, float dt);
    void updateApprox(float yaw, float pitch, float roll, float dt);
    void updateApprox(EulerAngles gyroMeasure, float dt);

    void updateGravity(float x, float y, float z);
    
    /*
    Apply a basic complemetary algorithm to existing orientation data.
    NOTE: Code here is written for a reference frame in which x is longitudinal and vertical, and may not be optimal for your purpose.
    ADDITIONALLY: Roll will become unstable when complementary filter is enabled. Use with caution.
    */
    void applyComplementary(Quaternion measuredGravity, float alpha);

    /*
    Zero out roll of stored orientation
    USE ONLY ON PAD WHEN COUPLED WITH COMPLEMENTARY FILTER
    IMPROPER USE CAN RESULT IN EULER SINGULARITIES
    */
    void zeroRoll();

    EulerAngles toEuler() { return quaternionToEuler(orientation); } 
};

#endif