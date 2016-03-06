/* Car library for controlling a car with the arduino
 * Created by Nigel Tiany
 * March 1 2016.
 * For personal non-commercial use only
 */
#ifndef Servoi2c_h
#define Servoi2c_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
 
class Servoi2c{
   public:
     Servoi2c();
     Servoi2c(int index);
     void setServo(int index);
     int getIndex();
     int index;
};

#endif

