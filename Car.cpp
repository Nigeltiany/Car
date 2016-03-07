
#include "Car.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Engine.h"
#include "Servoi2c.h"

Car::Car(){
}

void Car::create(){
  pwm = Adafruit_PWMServoDriver();
  pwm.begin();
  pwm.setPWMFreq(50);
  setLargestCornerAngle(13);
  setCornerServo(0);
  setCornerCenter(73);
  setCornerAngle(cornerCenter);
  setServoMinPulse(133);
  setServoMaxPulse(533);
  setCornerSpeed(0.1);
  setGear('p');
  Serial.println("INSTANTIATED");
  turnRight();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR CORNERING CODE            /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void Car::initCorneringI2C(int corneringServoIndex){
  int pulselength = map(cornerCenter, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(corneringServoIndex, 0, pulselength);
}

int Car::getCornerAngle(){
  return cornerAngle;
}

void Car::setCornerCenter(int center){
  cornerCenter = center;
}

int Car::getCornerCenter(){
  return cornerCenter;
}

void Car::setCornerAngle(int angle){
  cornerAngle = angle;
}

void Car::forwardSteer(){
  unsigned long corneringMillis = 0;
  unsigned long currentMillis = millis();
  // the following code is repeated every interval milliseconds
  int angle = getCornerAngle();
  if(angle < cornerCenter){
    if (currentMillis - corneringMillis >= cornerSpeed){
      corneringMillis = currentMillis; 
      int pulselength = map(angle+=1, 0, 180, SERVOMIN, SERVOMAX);
      pwm.setPWM(corneringServoIndex, 0, pulselength);
      //set gloable corner angle
      setCornerAngle(angle+1);
    }
  }else if(angle > cornerCenter){
    if (currentMillis - corneringMillis >= cornerSpeed){
      corneringMillis = currentMillis; 
      int pulselength = map(angle-=1, 0, 180, SERVOMIN, SERVOMAX);
      pwm.setPWM(corneringServoIndex, 0, pulselength);
      //set gloable corner angle
      setCornerAngle(angle-1);
    }
  }else{
    int pulselength = map(cornerCenter, 0, 180, SERVOMIN, SERVOMAX);
    corneringMillis = currentMillis; 
    pwm.setPWM(corneringServoIndex, 0, pulselength);
    //set gloable corner angle
    setCornerAngle(cornerCenter);
  }
}

void Car::setServoMinPulse(int m){
  SERVOMIN = m;
}

void Car::setServoMaxPulse(int m){
  SERVOMAX = m;
}


int Car::getLargestCornerAngle(){
  return largestCornerAngle;
}

void Car::setLargestCornerAngle(int angle){
  largestCornerAngle = angle;
}

void Car::turnLeft(){
  unsigned long currentMillis = millis();
  Serial.println("called left");
  // the following code is repeated every interval milliseconds
    int angle  = getCornerAngle();
    if(angle < cornerCenter-largestCornerAngle){ //from center
      if (currentMillis - corneringMillis >= cornerSpeed){
        corneringMillis = currentMillis; 
        int pulselength = map(angle+=1, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(corneringServoIndex, 0, pulselength);
        //set gloable corner angle
        setCornerAngle(angle+=1);
      }
    }else if(angle > cornerCenter-largestCornerAngle){
      if (currentMillis - corneringMillis >= cornerSpeed){
        corneringMillis = currentMillis; 
        int pulselength = map(angle-=1, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(corneringServoIndex, 0, pulselength);
        //set gloable corner angle
        setCornerAngle(angle-=1);
      }
    }else{
      int pulselength = map(cornerCenter-largestCornerAngle, 0, 180, SERVOMIN, SERVOMAX);
      corneringMillis = currentMillis; 
      pwm.setPWM(corneringServoIndex, 0, pulselength);
      //set gloable corner angle
      setCornerAngle(cornerCenter-largestCornerAngle);
    }
  }

void Car::turnRight(){
  unsigned long currentMillis = millis();
  Serial.println("called Right");
  // the following code is repeated every interval milliseconds
    int angle  = getCornerAngle();
    if(angle < cornerCenter+largestCornerAngle){ //from center
      if (currentMillis - corneringMillis >= cornerSpeed){
        corneringMillis = currentMillis; 
        int pulselength = map(angle+=1, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(corneringServoIndex, 0, pulselength);
        //set globle corner angle
        setCornerAngle(angle+=1);
      }
    }else if(angle > cornerCenter+largestCornerAngle){
      if (currentMillis - corneringMillis >= cornerSpeed){
        corneringMillis = currentMillis; 
        int pulselength = map(angle-=1, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(corneringServoIndex, 0, pulselength);
        //set globle corner angle
        setCornerAngle(angle-=1);
      }
    }else{
      int pulselength = map(cornerCenter+largestCornerAngle, 0, 180, SERVOMIN, SERVOMAX);
      pwm.setPWM(corneringServoIndex, 0, pulselength);
      corneringMillis = currentMillis; 
      //set gloable corner angle
      setCornerAngle(cornerCenter+largestCornerAngle);
    }
}

void Car::setCornerServo(int index){ 
  corneringServoIndex = index;
}

int Car::getCornerServo(){ 
  return corneringServoIndex;
}

void Car::setCornerSpeed(double x){
  cornerSpeed = x;
}

void Car::setServo(int index){ 
  Servoi2c servo;
  servo.setServo(index);
}

/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR ENGINE CODE            ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void Car::initEngine(int pinL,int pinR){
  _engine.setPinL(pinL);
  _engine.setPinR(pinR);
}

  //Pass instance of pwm always
void Car::brake(){ 
  _engine.halt(pwm);
}
void Car::accelerate(){ 
  _engine.throttle(pwm);
}
void Car::deccelerate(){
  _engine.channelize(pwm);
}

/////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////
/////////             CAR TRANSMISSION CODE            //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void Car::setGear(char v){
  _engine.setGear(v);
  gear = v;
}

char Car::getGear(){
  return gear;
}

void Car::setTransmission(char t){
  _engine.setTransmission(t);
}

/////////////////////////////////////////////////////////////////////////////////////////////


