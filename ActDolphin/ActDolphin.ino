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
#define ledPin 13
int tailServoPos = 0;
int yawServoPos = 0;
boolean attachedServos = false;

#define yawServoPin 5
#define tailServoPin 6
Servo yawServo;
Servo tailServo;

enum robotState {
  STANDBY, // No movement
  SEARCH, // Searching movement, rotating about itself
  APPROACH, // Moving towards target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // E-Stop
};

enum robotState dolphinState;

void setup() {
  setupServos();
  Serial.begin(4800);
  Wire.begin(ACT_ADDRESS);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent); // Receive state, yaw position, and tail position from Sense/Think Arduino

}

void setupServos(){
  if(!attachedServos){
    yawServo.attach(yawServoPin);
    tailServo.attach(tailServoPin);  // attaches the servo on pin 9 to the servo object
    attachedServos = true;
  }
}
void loop() {
  yawServo.write(yawServoPos);
  tailServo.write(tailServoPos);
  
  switch(dolphinState){
    case STANDBY:
      detachAllMotors();
      blinkStandbySignal();
      break;
      
    case SEARCH:
      setupServos(); // If the servos were in standby mode, attach them.
      blinkSearchSignal();
      break;
      
    case APPROACH:
      blinkApproachSignal();
      break;
      
    case VICTORY:
      blinkVictorySignal();
      break;
      
    case HELPME:
      blinkHelpmeSignal();
//      pushEStop(); // Turn off servos
      detachAllMotors();
      break;
      
    default:
      break;
  }
}

void detachAllMotors(){
  yawServo.detach();
  tailServo.detach();
  attachedServos = false;
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
void printDolphinState(){
  switch(dolphinState){
    case STANDBY:
      Serial.println("STANDBY");
      break;
    case SEARCH:
      Serial.println("SEARCH");
      break;
    case APPROACH:
      Serial.println("APPROACH");
      break;
    case VICTORY:
      Serial.println("VICTORY");
      break;
    case HELPME:
      Serial.println("HELME");
      break;
    default:
      Serial.println("404");
      break;
  }
}

void blinkStandbySignal(){
  digitalWrite(ledPin, HIGH);
}
void blinkSearchSignal(){
  digitalWrite(ledPin, millis() % 500 > 250 ? HIGH : LOW); //Blink every 500ms
}

void blinkApproachSignal(){
  digitalWrite(ledPin, millis() % 350 > 250 ? HIGH : LOW); //Blink every 350ms, asymmetrically
}

void blinkVictorySignal(){
  digitalWrite(ledPin, millis() % 650 > 500 ? HIGH : LOW); //Blink every 650ms, asymmetrically
}

void blinkHelpmeSignal(){
  digitalWrite(ledPin, millis() % 150 > 75 ? HIGH : LOW); //Blink every 150ms
}
