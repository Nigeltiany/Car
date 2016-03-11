#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include "Car.h"
#include "Engine.h"
#include "Servoi2c.h"
#include "BackgroundTask.h"
#include <VSync.h>

char steer_command;
char move_command;
char gear_command;

//Trig and Echo pins of the Ultrasonic Sensor
const int trigPin = 7;
const int echoPin = 6;

Car car;
Servoi2c radar;
BackgroundTask async;
ValueSender<2> radarData;
int angle,objectDistance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  car.create();
  car.initEngine(2,3);
  radar.setServo(1);
  radar.setPace(30);
  long interval = 0;
  async = BackgroundTask(interval);

  radarData.observe(angle);
  radarData.observe(objectDistance);
}

void loop() {
  async.Update();
  
  radar.rotate(0,180);
  angle = radar.getAngle();
  objectDistance = calculateDistance();
  radarData.sync();
  
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
      case 'v':
        car.setTransmission('s');
        break;
      case 'm':
        car.setTransmission('m');
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
}

// Function for calculating the distance
int calculateDistance(){ 
  long duration;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Set trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Read sound wave travel time in microseconds
  objectDistance = duration*0.034/2;
  return objectDistance;
}
