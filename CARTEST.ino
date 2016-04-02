#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include "Car.h"
#include "Engine.h"
#include "Servoi2c.h"
#include "BackgroundTask.h"
#include <VSync.h>
#include <NewPing.h>
//Number of data items being sent = <9>
ValueSender<9> sender;
ValueSender<1> carSender;
ValueReceiver<1> cmd;

char steer_command;
char move_command;
char gear_command;

int motorPin1 = 32;
int motorPin2 = 33;

Car car;
Servoi2c radar;
BackgroundTask async;

//SONAR VARIABLES
#define SONAR_NUM 5
#define maxDistance 1000
#define PING_INTERVAL 33

// VARIABLES FOR STORING DISTANCE ON EACH SENSOR
int right;        //  6 // 7 its echo pin
int left;         //  9 // 10 its echo pin
int centerRight;  //  2 // 3 its echo pin
int centerLeft;   //  4 // 5 its echo pin
int rear;         // 11 // 12 its echo

//CAR VARAIBLES TO SEND TO PROCESSING SKETCH
int maxSpeed;
int carSpeed;
int gearMaxSpeed;
int minSpeed;
int command;
int carGear;

//CAR VARIABLES TO RECEIVE FROM PROCESSING SKETCH
int pCommand;

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;  

NewPing sonar[SONAR_NUM] = {
  // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(10, 9, maxDistance), // left 
  NewPing(6, 7, maxDistance),  // right
  NewPing(4, 5, maxDistance),  // center left
  NewPing(2, 3, maxDistance),  // center right
  NewPing(11, 12, maxDistance) // rear
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  car.create();
  car.initEngine(2,3);
  radar.setServo(1);
  radar.setPace(30);
  long interval = 0;
  async = BackgroundTask(interval);
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++){ // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  }
  
  sender.observe(left);
  sender.observe(right);
  sender.observe(centerLeft);
  sender.observe(centerRight);
  sender.observe(rear);
  sender.observe(maxSpeed);
  sender.observe(minSpeed);
  sender.observe(carSpeed);
  sender.observe(carGear);

  carSender.observe(minSpeed);
  //carSender.observe(gearMaxSpeed);
  //carSender.observe(command);

  cmd.observe(pCommand);
}

void loop() {

  cmd.sync();
  char tempCommand = (char)pCommand;
  steer_command = tempCommand;
  move_command = tempCommand;
  gear_command = tempCommand;
  async.Update();

  checkSurroudings();
  maxSpeed = car.getMaxSpeed();
  minSpeed = car.getMinSpeed();
  carSpeed = car.getSpeed();
  carGear = int(car.getGear());
  sender.sync();

  command = int(tempCommand);
  gearMaxSpeed = car.getGearSpeed();
  carSender.sync();

//  if(Serial.available()){
//    char temp = Serial.read();
//    steer_command = temp;
//    move_command = temp;
//    gear_command = temp;
//    async.Update();
//  }//else{
  //}
  
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
        digitalWrite(motorPin1,HIGH);
        digitalWrite(motorPin2,LOW);
        car.accelerate();
        break;
      case 'x':
        car.deccelerate();
        break;
      case 'q':
        digitalWrite(motorPin1,LOW);
        digitalWrite(motorPin2,HIGH);
        car.accelerate();
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
        car.accelerate();
        break;
      case '2':
        car.setGear('2');
        car.accelerate();
        break;
      case '3':
        car.setGear('3');
        car.accelerate();
        break;
      case '4':
        car.setGear('4');
        car.accelerate();
        break;
      case '5':
        car.setGear('5');
        car.accelerate();
        break;
    }
}

void checkSurroudings(){
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    //Serial.print(i);
    //Serial.print("=");
    //Serial.print(cm[i]);
    //Serial.print("cm ");
    left = cm[0];
    right = cm[1];
    centerLeft = cm[2];
    centerRight = cm[3];
    rear = cm[4];
  }
}

