#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include "Car.h"
#include "Engine.h"
#include "Servoi2c.h"

char steer_command;
char move_command;
char gear_command;

Car car;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  car.create();
  car.initEngine(2,3);
}

void loop() {
  // put your main code here, to run repeatedly:
  //car.setGear('d');
  //char gear = car.getGear();
  //Serial.println(gear);
  if(Serial.available()){
    char temp = Serial.read();
    steer_command = temp;
    move_command = temp;
    gear_command = temp;
  }
  
    switch(steer_command){
      case 'l':
        car.turnLeft();
        steer_command == 'l';
        break;
      case 'r':
        car.turnRight();
        steer_command == 'r';
        break;
      case 'f':
        car.forwardSteer();
        //steer_command = 'f';
        break;
    }
    
    switch(move_command){
      case 's':
        car.brake();
        break;
      case 'w':
        car.accelerate();
        break;
      case 'x':
        car.deccelerate();
        break;
    }

    switch(gear_command){
      case 'p':
        car.setGear('p');
        break;
      case 'a':
        car.setTransmission('a');
        break;
      case 's':
        car.setTransmission('a');
        break;
      case 'm':
        car.setTransmission('a');
        break;
      case 'd':
        car.setGear('1');
        break;
      case '1':
        car.setGear('1');
        break;
      case '2':
        car.setGear('2');
        break;
      case '3':
        car.setGear('3');
        break;
      case '4':
        car.setGear('4');
        break;
      case '5':
        car.setGear('5');
        break;
    }
  //Serial.print("car.corneringMillis ");
  //Serial.println(car.corneringMillis);
}
