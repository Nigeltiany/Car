#include "Car.h"
#include "Engine.h"
#include "BackgroundTask.h"
#include "Global.h"

BackgroundTask::BackgroundTask(boolean &interrupt,long checkRate){ 
  refresh = checkRate;
  previousMillis = 0;
  brake = &interrupt;
}

void BackgroundTask::Update(){
  unsigned long currentMillis = millis();
  
  if(Serial.available()){
    char temp = Serial.read();
    brakeCommand = temp;
  }
  
  if ((brakeCommand == 's') && (currentMillis - previousMillis >= refresh)){
    brake = 1; //true
    setInterrupt(&brake);
    previousMillis = currentMillis;
  }
}

