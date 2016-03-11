
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"
#include "Engine.h"
#include "Global.h"

Engine::Engine(){
  setMinSpeed(900);
  setMaxSpeed(4096);
  setStopSpeed(0);
  setPace(getMinSpeed());
  setTransmission('a');
}

Engine::Engine(int pinL,int pinR){
  _pinL = pinL;
  _pinR = pinR;
  setMinSpeed(704);
  setMaxSpeed(4096);
  setStopSpeed(0);
  setPace(getMinSpeed());
  setTransmission('a');
  //changeGear();
  long interval = 100;
}
void Engine::setPinL(int pinL){
  _pinL = pinL;
}
void Engine::setPinR(int pinR){
  _pinR = pinR;
}
int Engine::getPinL(){
  return _pinL;
}
int Engine::getPinR(){
  return _pinR;
}

void Engine::setStopSpeed(int sp){
  stopSpeed = sp;
}
int Engine::getStopSpeed(){
  return stopSpeed;
}
void Engine::setMaxSpeed(int sp){
  maxSpeed = sp;
}
int Engine::getMaxSpeed(){
  return maxSpeed;
}
void Engine::setMinSpeed(int sp){
  minSpeed = sp;
}
int Engine::getMinSpeed(){
  return minSpeed;
}

int Engine::getPace(){
  return pace;
}

void Engine::setPace(int val){
   //Wouldn't wanna accelerate over the max speed
  if(val <= getMaxSpeed() && val >= getMinSpeed()){
    pace = val;
  }
}

void Engine::setGear(char g){
  gear = g;
}

char Engine::getGear(){
  return gear;
}

void Engine::halt(Adafruit_PWMServoDriver pwm){
  pwm.setPin(getPinL(),getStopSpeed());
  pwm.setPin(getPinR(),getStopSpeed());
  setPace(getMinSpeed());
  //Serial.print("Stopped ");
  //Serial.println(getPace());
}

void Engine::setTransmission(char s){
  transmission = s;
}

char Engine::getTransmission(){
  return transmission;
}

void Engine::setInterrupt(bool *v){
  interrupt = v;
}

bool Engine::getInterrupt(){
  bool v = interrupt;
  return Serial.print(v);
}

void Engine::throttle(Adafruit_PWMServoDriver pwm){
  switch(getGear()){
    case '1':
        setMaxSpeed(gear1);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
            //Serial.println(getGear());
            //Serial.println(checkGear());
            //Serial.print(getGear());
            if (interrupt) break; halt(pwm);
            changeGear();
        }while(pace<getMaxSpeed());
        if(getTransmission() != 'a'){
          break; // else woun't break out of the loop so that gears can change in auto
        }
    case '2':
        setMaxSpeed(gear2);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
            //Serial.println(getGear());
            //Serial.println(checkGear());
            //Serial.print(getGear());
            if (interrupt) break; halt(pwm);
            changeGear();
        }while(pace<getMaxSpeed());
        if(getTransmission() != 'a'){
          break; // else woun't break out of the loop so that gears can change in auto
        }
    case '3':
        setMaxSpeed(gear3);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
            //Serial.println(getGear());
            //Serial.println(checkGear());
            if (interrupt) break; halt(pwm);
            changeGear();
        }while(pace<getMaxSpeed());
        if(getTransmission() != 'a'){
          break; // else woun't break out of the loop so that gears can change in auto
        }
    case '4':
        setMaxSpeed(gear4);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
            //Serial.println(getGear());
            //Serial.println(checkGear());
            if (interrupt) break; halt(pwm);
            changeGear();
        }while(pace<getMaxSpeed());
        if(getTransmission() != 'a'){
          break; // else woun't break out of the loop so that gears can change in auto
        }
    case '5':
        setMaxSpeed(gear5);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
            //Serial.println(getGear());
            //Serial.println(checkGear());
            if (interrupt) break; halt(pwm);
            changeGear();
        }while(pace<getMaxSpeed());

    ////////////////////////////////////////////////////////////////////////////////
      break;
    ////////////////////////////////////////////////////////////////////////////////
    
      case 'p':
        setMaxSpeed(parking);
        setPace(parking);
        pwm.setPin(getPinL(),parking);
        pwm.setPin(getPinR(),parking);
        Serial.print("Parking...");
        Serial.println(getPace());
        break;
    }
}

char Engine::checkGear(){
  switch(pace){
    case parking:
        return 'p';
        break;
    case parking+1 ... gear1:
        return '1';
        break;
    case gear1+1 ... gear2:
        return '2';
        break;
    case gear2+1 ... gear3:
        return '3';
        break;
    case gear3+1 ... gear4:
        return '4';
        break;
    case gear4+1 ... gear5:
        return '5';
        break;
    default:
        return 'n';
        Serial.print("Neutral");
        break;
   }
}

void Engine::changeGear(){
  switch(getTransmission()){
    case 'a':
        switch(getGear()){
        // For any of these gears...
        case '1':
            if(getPace() <= gear1 && getPace() < gear2){
              setGear('2');
              //Serial.println("changing to 2");
            }
            break;
        case '2':
            if(getPace() <= gear2 && getPace() > gear1){
              setGear('3');
              //Serial.println("changing to 3");
            }
            break;
        case '3':
            if(getPace() <= gear3 && getPace() > gear2){
              setGear('4');
              //Serial.println("changing to 4");
            }
            break;
        case '4':
            if(getPace() <= gear4 && getPace() > gear3){
              setGear('5');
              //Serial.println("changing to 5");
            }
            break;
        case '5':
            if(getPace() > gear4 && getPace() <= gear5){
              //setGear('5');
            }
            break;
      }// End AUTOMATIC TRANSMISSION 
    break;
    case 's':
    break;

    ///////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////     MANUAL TRANSMISSION     /////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    
    case 'm':
      switch(getGear()){
        // For any of these gears...
        case '1':
            if(getPace() <= gear1 && getPace() > getStopSpeed()){
              getGear(); // from where? The user Silly :-) , Manual transmission, Ever driven one?
            }
            break;
        case '2':
            if(getPace() <= gear2 && getPace() > gear1){
              getGear();
            }
            break;
        case '3':
            if(getPace() <= gear3 && getPace() > gear2){
              getGear();
            }
            break;
        case '4':
            if(getPace() <= gear4 && getPace() > gear3){
              getGear();
            }
            break;
        case '5':
            if(getPace() > gear4 && getPace() <= gear5){
              getGear();
            }
            break;
      }
    break;// End MANUAL TRANSMISSION
  }
}

void Engine::downShift(){
  Serial.println((int)getGear()-48);
  Serial.println((int)checkGear()-48);
  if (((int)getGear()-48) - ((int)checkGear()-48) > 0){
    Serial.println("DOWN SHIFTING");
    if(getPace() > gear1 && getPace() <= gear2){
      setGear('1'); // set gear to next level;
      // then throttle at this gear
      throttle(pwm);
    }else if(getPace() > gear2 && getPace() <= gear3){
      setGear('2');
      // then throttle at this gear
      throttle(pwm);
    }
    else if(getPace() > gear3 && getPace() <= gear4){
      setGear('3');
      // then throttle at this gear
      throttle(pwm);
    }
    else if(getPace() > gear4 && getPace() <= gear5){
      setGear('4');
      // then throttle at this gear
      throttle(pwm);
    }
  }
}

void Engine::channelize(Adafruit_PWMServoDriver pwm){
  setPace(pace-819);
  pwm.setPin(getPinL(),pace);
  pwm.setPin(getPinR(),pace);
  Serial.println(getPace());
}



