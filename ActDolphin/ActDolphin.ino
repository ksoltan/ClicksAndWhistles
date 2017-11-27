/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
#include <Wire.h>
#include <Servo.h>
#define ACT_ADDRESS 8
//String message;
//char servo;
//int servoPos;

int tailServoPos = 0;
int yawServoPos = 0;
#define yawServoPin 5
#define tailServoPin 6
Servo yawServo;
Servo tailServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

enum robotState {
  STANDBY, // No movement
  SEARCH, // Searching movement, rotating about itself
  APPROACH, // Moving towards target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // E-Stop
};

enum robotState dolphinState;

void setup() {
  tailServo.attach(tailServoPin);  // attaches the servo on pin 9 to the servo object
  yawServo.attach(yawServoPin);
  Serial.begin(4800);
  Wire.begin(ACT_ADDRESS);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent); // Receive state, yaw position, and tail position from Sense/Think Arduino

}

void loop() {
//  updateTailPosition(slowFlapPeriod);
  tailServo.write(tailServoPos);
  yawServo.write(yawServoPos);
}

void receiveEvent(int bytes) {  // Receive state, yaw, pitch (tail) from SENSE/THINK Arduino.
  if (Wire.available() == 6){ // 'state' ',' 'yaw position' ',' 'tail position' , ';' (6 bytes in all)
    dolphinState = Wire.read(); // Read the state
//    int dolphinState = Wire.read();
    // Next is a comma.
    Wire.read();

//    yawServoPos = Wire.read(); // Read the way position
    yawServoPos = Wire.read();
    // Next is a comma.
    Wire.read();

    tailServoPos = Wire.read(); // Read the tail position as a byte
    Serial.println("TAIL: " + (int)tailServoPos);
    
    // Check that the last character is semicolon
    if(Wire.read() != ';'){
      Serial.println("INCORRECTLY RECEIVED");
    }
  }
}

