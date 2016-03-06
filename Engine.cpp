
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"
#include "Engine.h"

extern Adafruit_PWMServoDriver p;

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
  setPace(getStopSpeed());
  setGear('p');
  Serial.println(getPace());
}

void Engine::setTransmission(char s){
  transmission = s;
}

char Engine::getTransmission(){
  return transmission;
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
            changeGear();
        }while(pace<getMaxSpeed());
        Serial.println(getGear());
        //break; //woun't break out of the loop so that gears can change
    case '2':
        setMaxSpeed(gear2);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
        }while(pace<getMaxSpeed());
        changeGear();
        Serial.println(getGear());
        //break; //woun't break out of the loop so that gears can change
    case '3':
        setMaxSpeed(gear3);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
        }while(pace<getMaxSpeed());
        changeGear();
        Serial.println(getGear());
        //break; //woun't break out of the loop so that gears can change
    case '4':
        setMaxSpeed(gear4);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
        }while(pace<getMaxSpeed());
        changeGear();
        Serial.println(getGear());
        //break; //woun't break out of the loop so that gears can change
    case '5':
        setMaxSpeed(gear5);
        do{
            pwm.setPin(getPinL(),pace);
            pwm.setPin(getPinR(),pace);
            setPace(pace+1);
            Serial.println(getPace());
        }while(pace<getMaxSpeed());
        changeGear();
        Serial.println(getGear());
        
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
  //checkGear();
  switch(getTransmission()){
    case 'a':
        if(getMaxSpeed() == gear1){
          setPace(gear1+1); // add more pace so that gear changes and a new maxspeed is set
        }else if(getMaxSpeed() == gear2){
          setPace(gear2+1);
        }
        else if(getMaxSpeed() == gear3){
          setPace(gear3+1);
        }
        else if(getMaxSpeed() == gear4){
          setPace(gear4+1); // head over to gear five, the last gear
        }
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
        case '2':
        case '3':
        case '4':
        case '5':
          if (getGear() != checkGear()){ // meaning possibly the user switched gears but...
            // its is not the gear the transmission is in right now
            // Now all the logic for automatic gear shifting with a tiny twist

            //////////////////////////////////////////////////////////
            /////////////////       UP SHIFTING     //////////////////
            //////////////////////////////////////////////////////////
            
            if(getPace() <= gear1 && getPace() > getStopSpeed()){
              setGear('1'); // set gear to next level;
              // then throttle at this gear
              //throttle(p);
            }else if(getPace() <= gear2 && getPace() > gear1){
              setGear('2');
              // then throttle at this gear
              //throttle();
            }
            else if(getPace() <= gear3 && getPace() > gear2){
              setGear('3');
              // then throttle at this gear
              //throttle();
            }
            else if(getPace() <= gear4 && getPace() > gear3){
              setGear('4');
              // then throttle at this gear
              //throttle();
            }else{
              setGear('5');
              // then throttle at this gear
              //throttle();
            }

            //////////////////////////////////////////////////////////
            ////////////////       DOWN SHIFTING     /////////////////
            //////////////////////////////////////////////////////////

            
            
          }
        break;
      }
    break;
  }
}

void Engine::channelize(Adafruit_PWMServoDriver pwm){
  setPace(pace-819);
  pwm.setPin(getPinL(),pace);
  pwm.setPin(getPinR(),pace);
  Serial.println(getPace());
}



