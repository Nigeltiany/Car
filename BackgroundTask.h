#ifndef BackgroundTask_h
#define BackgroundTask_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Engine.h"
 

class BackgroundTask{
  BackgroundTask(boolean &interrupt,long checkRate);
  bool brake;
  long refresh;
  char brakeCommand;
 
  bool breakRequest;
  unsigned long previousMillis;

  void Update();
};

#endif
