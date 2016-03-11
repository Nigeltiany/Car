/* Car library for controlling a car with the arduino
 * Created by Nigel Tiany
 * March 1 2016.
 * For personal non-commercial use only
 */
#ifndef Car_h
#define Car_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_PWMServoDriver.h>
#include "Engine.h"
#include "Servoi2c.h"

class Car{
  public:
    Car();
    void create();
    
/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR CORNERING VARIABLES            ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

    int SERVOMIN = 133;
    int SERVOMAX = 533;
    int cornerCenter;
    int corneringServoIndex;
    int largestCornerAngle;
    int cornerAngle;
    unsigned long corneringMillis = 0;
    double cornerSpeed = 0; //affect cornering speed 0.1 is fast 1000.0 is slow
    
    void turnLeft();
    void turnRight();
    void forwardSteer();
    void initCornering(int pin);
    void initCorneringI2C(int servonum);
    void setCornerCenter(int center);
    int getCornerCenter();
    void setServoMinPulse(int min);    
    void setServoMaxPulse(int max);   
    void setCornerServo(int index);
    int getCornerServo();
    void setServo(int index);
    void setCornerSpeed(double x);  
    void setCornerAngle(int angle);
    int getCornerAngle(); 
    void setLargestCornerAngle(int angle);
    int getLargestCornerAngle();
    
/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR ENGINE VARIABLES            ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

    Engine _engine;
    bool electricDiff; 
    
    void initEngine(int pinL,int pinR);
    void brake();
    bool setElectricDiff(bool state);
    
/////////////////////////////////////////////////////////////////////////////////////////////



                    //ENGINE AND TRANSMISSION VARIABLES//
            
    void accelerate();
    void deccelerate();

                    /////////////////////////////////////


    
/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR TRANSMISSION VARIABLES            /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

    char gear;
    char transmission;
    
    char getGear();
    char getTransmission();
    void setTransmission(char t);
    void setGear(char v);
   
/////////////////////////////////////////////////////////////////////////////////////////////
    
  private:
};

#endif

