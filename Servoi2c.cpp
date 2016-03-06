#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Car.h"
#include "Servoi2c.h"

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




