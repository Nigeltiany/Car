/* Car library for controlling a car with the arduino
 * Created by Nigel Tiany
 * March 1 2016.
 * For personal non-commercial use only
 */
#ifndef Servoi2c_h
#define Servoi2c_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"

#include <Adafruit_PWMServoDriver.h>
 
class Servoi2c{
   public:
     Servoi2c();
     Servoi2c(int index);
     void setServo(int index);
     int getIndex();
     int index;
     int SERVOMAX = 533;
     int SERVOMIN = 133;
     void setServoMax(int m);
     void setServoMin(int m);
     void rotate(int angle1,int angle2);
     unsigned long previousMillis = 0;
     int interval = 10;
     int _angle;

     void setPace(int pace);
     int startAngle;
     int endAngle;
     int getStartAngle();
     int getEndAngle();
     int getAngle();

   private:
     void setStartAngle(int i);
     void setEndAngle(int i);
};

#endif

