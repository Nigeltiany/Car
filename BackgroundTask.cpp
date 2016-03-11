#include "Car.h"
#include "Engine.h"
#include "BackgroundTask.h"
#include "Global.h"


BackgroundTask::BackgroundTask(){ 
  refresh = 2;
  previousMillis = 0;
}

BackgroundTask::BackgroundTask(long checkRate){ 
  refresh = checkRate;
  previousMillis = 0;
}

void BackgroundTask::Update(){
  unsigned long currentMillis = millis();
  
  if(Serial.available()){
    char temp = Serial.read();
    brakeCommand = temp;
  }

  if ((brakeCommand == 's') && (currentMillis - previousMillis >= refresh)){
    brake = 1; //true
    _engine.setInterrupt(&brake);
    previousMillis = currentMillis;
  }else if ((brakeCommand == 'w') && (currentMillis - previousMillis >= refresh)){
    brake = 0;
    _engine.setInterrupt(&brake);
    previousMillis = currentMillis;
  }
}

