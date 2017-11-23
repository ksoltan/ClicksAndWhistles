// Include the required Wire library for I2C
#include <Wire.h>
int x = 0;
void setup() {
  // Start the I2C Bus as Master
  Wire.begin(); 
  Serial.begin(9600); //Enable the serial comunication
}
void loop() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(x);              // sends x 
  Wire.endTransmission();    // stop transmitting
  Serial.println("Writing: " + String(x));
  x++; // Increment x
  
  if (x > 5) x = 0; // `reset x once it gets 6
  delay(1000);
}

void commandMotor(char motor, int motorPos){
  Wire.write(motor);
  if (motorPos < 100) Wire.write('0');
  if (motorPos < 10) Wire.write('0');
  Wire.write(motorPos);
}

