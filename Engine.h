/* Car library for controlling a car with the arduino
 * Created by Nigel Tiany
 * March 1 2016.
 * For personal non-commercial use only
 */
#ifndef Engine_h
#define Engine_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"
#include "Car.h"

const int parking = 0;
const int gear1 = 1300;
const int gear2 = 2180;
const int gear3 = 2999;
const int gear4 = 3700;
const int gear5 = 4096;

class Engine{
  public:
    Engine();
    Engine(int pinL,int pinR);
    int _pinL;
    int _pinR;
    int minSpeed;
    int maxSpeed;
    int stopSpeed;
    int pace;
    char gear;
    char transmission;

    void setTransmission(char t);
    char getTransmission();
    void changeGear();
    char checkGear();
    void setGear(char g);
    char getGear();
    int getPace();
    void setPace(int val);
    int getStopSpeed();
    void setStopSpeed(int sp);
    void setMinSpeed(int sp);
    int getMinSpeed();
    void setMaxSpeed(int sp);
    int getMaxSpeed();
    void setPinL(int pinL);
    void setPinR(int pinR);
    int getPinL();
    int getPinR();
    void halt(Adafruit_PWMServoDriver pwm);
    void throttle(Adafruit_PWMServoDriver pwm);
    void channelize(Adafruit_PWMServoDriver pwm);

};

#endif

