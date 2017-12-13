#include <Wire.h>
#include "ActParams.h"
//#include <Servo.h>

//#define debugYawServo true
#ifdef debugYawServo
  Servo yawServo;
#endif

//#define wireOn true

#define ACT_ADDRESS 8 // Address at which ACT Arduino is expecting Serial communication

void setupI2C(){
//  #ifdef wireOn
    Wire.begin(); // The address is optional for the master
//  #endif
  
  #ifdef debugYawServo
    yawServo.attach(6);
  #endif
 
}



// Transmits STATE, YAW POSITION, TAIL POSITION
void sendActParams(){
  XBee.println("try get act params" );
  if(getActParams()){
//    Serial.println(yawServoPos);
//    #ifdef wireOn
      Wire.beginTransmission(ACT_ADDRESS);
      // Transmit state
      Wire.write(dolphinState);
      Wire.write(",");
      // Transmit yaw position
      Wire.write(yawServoPos);
      Wire.write(",");
      Wire.write(tailServoPos);
      Wire.write(";");
      Wire.endTransmission();
//    #endif
    
    #ifdef debugYawServo
      yawServo.write(yawServoPos);
    #endif
  }
}
