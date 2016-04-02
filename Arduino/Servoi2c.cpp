#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"
#include "Servoi2c.h"
#include "Global.h"

Servoi2c::Servoi2c(){
  index = 0;
}

Servoi2c::Servoi2c(int in){
  index = in;
}

int Servoi2c::getIndex(){
  return index;
}

void Servoi2c::setServo(int in){
  index = in;
}

void Servoi2c::setServoMin(int m){
  SERVOMIN = m;
}

void Servoi2c::setServoMax(int m){
  SERVOMAX = m;
}

void Servoi2c::setStartAngle(int angle){
  startAngle = angle;
}

void Servoi2c::setEndAngle(int angle){
  endAngle =  angle;
}

int Servoi2c::getStartAngle(){
  return startAngle;
}

int Servoi2c::getEndAngle(){
  return endAngle;
}

void Servoi2c::setPace(int pace){
  interval = pace;
}

void Servoi2c::rotate(int angle1,int angle2){
  unsigned long currentMillis = millis();
  if(startAngle == 0 && endAngle == 0){
    setStartAngle(angle1);
    setEndAngle(angle2);
  }
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(getStartAngle() < angle2){
      int pulselength = map(startAngle, 0, 180, SERVOMIN, SERVOMAX);
      _angle = startAngle;
      pwm.setPWM(getIndex(), 0, pulselength);  
      startAngle = startAngle+1; 
    }else if(getEndAngle() > angle1){
      int pulselength = map(endAngle, 0, 180, SERVOMIN, SERVOMAX);
      _angle = endAngle;
      pwm.setPWM(getIndex(), 0, pulselength);  
      endAngle = endAngle-1; 
    }else{
      //reset and loop
      startAngle = 0;
      endAngle = 0;
    }
  }
}

int Servoi2c::getAngle(){
  return _angle;
}



