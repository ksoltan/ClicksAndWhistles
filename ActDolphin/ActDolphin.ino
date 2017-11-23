#include <Wire.h>
#include <Servo.h>
#define ledPin 13 // ?? WE HAVE THREE LEDS ??
#define eStopPin 8
#define ACT_ADDRESS 8

// Servos need PWM pins
#define yawServoPin 9
#define tailServoPin 10

int x = 0;
String message;
char servo;
int servoPos;

Servo yawServo;
Servo tailServo;

int yawServoPos; //tail servo
int tailServoPos; //body servo

enum robotState {
  STANDBY, // No movement
  SEARCH, // Searching movement, rotating about itself
  APPROACH, // Moving towards target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // E-Stop
};

enum robotState dolphinState;

//check that systems are okay; assign initial state accordingly
void setup() {
  setupActPins();
//  setupServos();

  // Start the I2C Bus as Slave on address 8
  Wire.begin(ACT_ADDRESS);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent); // Receive state, yaw position, and tail position from Sense/Think Arduino
  Serial.begin(9600); //Enable the serial comunication
}

void loop() {
  switch(dolphinState){
    case STANDBY:
      freezeAllMotors();
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
      pushEStop(); // Turn off servos
      break;
      
    default:
      break;
  }
  
  analogWrite(yawServoPin, yawServoPos);
  analogWrite(tailServoPin, tailServoPos);
}

void setupActPins(){
  pinMode(ledPin, OUTPUT);
  pinMode(eStopPin, OUTPUT);
}

void setupServos(){
  yawServo.attach(yawServoPin);
  tailServo.attach(tailServoPin);
}

void freezeAllMotors(){
  yawServo.write(90);
  tailServo.write(90);
}

void pushEStop(){
  digitalWrite(eStopPin, LOW); // No current should be going to the Relay
}

void receiveEvent(int bytes) {  // Receive state, yaw, pitch (tail) from SENSE/THINK Arduino.
  if (Wire.available() == 6){ // 'state' ',' 'yaw position' ',' 'tail position' , ';' (6 bytes in all)
    dolphinState = Wire.read(); // Read the state
    printDolphinState();

    // Next is a comma.
    Wire.read();

    yawServoPos = Wire.read(); // Read the way position

    // Next is a comma.
    Wire.read();

    tailServoPos = Wire.read(); // Read the tail position as a byte

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
