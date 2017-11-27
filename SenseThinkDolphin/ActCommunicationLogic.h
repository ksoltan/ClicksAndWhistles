#include <Wire.h>
#include "ActParams.h"

#define ACT_ADDRESS 8 // Address at which ACT Arduino is expecting Serial communication

void setupI2C(){
  Wire.begin(); // The address is optional for the master
}



// Transmits STATE, YAW POSITION, TAIL POSITION
void sendActParams(){
  if(getActParams()){
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
  }
}
