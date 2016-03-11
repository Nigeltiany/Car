#include "Car.h"
#include "Engine.h"
#include "BackgroundTask.h"
#include "Global.h"


BackgroundTask::BackgroundTask(){ 
  refresh = 10;
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
    //Serial.print(brake);
  }
}

