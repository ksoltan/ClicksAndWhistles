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
#define yellowLedPin 13
#define blueLedPin 12
int tailServoPos = 0;
int yawServoPos = 0;

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
  Serial.begin(115200);
  Wire.begin(ACT_ADDRESS);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent); // Receive state, yaw position, and tail position from Sense/Think Arduino
  Serial.begin(115200);
  Serial.println("Hi");
}

void setupServos(){
  yawServo.attach(yawServoPin);
  tailServo.attach(tailServoPin);  // attaches the servo on pin 9 to the servo object
}

void loop() {
//  yawServo.write(90);
//  tailServo.write(90);
  yawServo.write(yawServoPos);
  tailServo.write(tailServoPos);
  Serial.println(tailServoPos);
  switch(dolphinState){
    case STANDBY:
      blinkStandbySignal();
      break;
      
    case SEARCH:
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
      break;
      
    default:
      break;
  }
}

void receiveEvent(int bytes) {  // Receive state, yaw, pitch (tail) from SENSE/THINK Arduino.
//  Serial.println("In the receive event");
  if (Wire.available() == 6){ // 'state' ',' 'yaw position' ',' 'tail position' , ';' (6 bytes in all)
    dolphinState = Wire.read(); // Read the state
//    printDolphinState();
//    int dolphinState = Wire.read();
    // Next is a comma.
    Wire.read();

//    yawServoPos = Wire.read(); // Read the way position
    yawServoPos = Wire.read();
    // Next is a comma.
    Wire.read();

    tailServoPos = Wire.read(); // Read the tail position as a byte
//    Serial.print("TAIL: ");
//    Serial.print((int)tailServoPos);
//    Serial.print("\n");
    
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
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(blueLedPin, HIGH);
}
void blinkSearchSignal(){
  digitalWrite(yellowLedPin, millis() % 500 > 250 ? HIGH : LOW); //Blink every 500ms
  digitalWrite(blueLedPin, millis() % 500 > 250 ? HIGH : LOW); //Blink every 500ms
}

void blinkApproachSignal(){
 digitalWrite(yellowLedPin, millis() % 500 > 250 ? HIGH : LOW); //Blink every 500ms
  digitalWrite(blueLedPin, millis() % 500 < 250 ? HIGH : LOW); //Blink every 500ms
}

void blinkVictorySignal(){
  digitalWrite(yellowLedPin, millis() % 650 > 500 ? HIGH : LOW); //Blink every 650ms, asymmetrically
  digitalWrite(blueLedPin, millis() % 650 > 500 ? HIGH : LOW); //Blink every 650ms, asymmetrically
}

void blinkHelpmeSignal(){
  digitalWrite(yellowLedPin, millis() % 150 > 75 ? HIGH : LOW); //Blink every 150ms
  digitalWrite(blueLedPin, millis() % 150 > 75 ? HIGH : LOW); //Blink every 150ms
}
