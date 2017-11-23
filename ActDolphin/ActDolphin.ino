#include <Wire.h>
#include <Servo.h>
#define ledPin 13 // ?? WE HAVE THREE LEDS ??
#define eStopPin 8

// Servos need PWM pins
#define bodyServoPin 9
#define tailServoPin 10

int x = 0;
String message;
char servo;
int servoPos;

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

  // Start the I2C Bus as Slave on address 9
  Wire.begin(9);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); //Enable the serial comunication
}

void loop() {
  // Receive instructions from Sense/Think Arduino
//  receiveActParameters(); // Receive state, yaw, pitch1, pitch2 from SENSE/THINK Arduino.
  switch(dolphinState){
    case STANDBY:
//      freezeAllMotors();
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
      blinkDistressSignal();
      pushEStop(); // Turn off servos
      break;
      
    default:
      break;
  }
  
  analogWrite(tailServoPin, tailServoPos);
  analogWrite(bodyServoPin, bodyServoPos);
}

void setupActPins(){
  pinMode(ledPin, OUTPUT);
  pinMode(eStopPin, OUTPUT);
}

void freezeAllMotors(){
  
}

void pushEStop(){
  digitalWrite(eStopPin, LOW); // No current should be going to the Relay
}

void receiveEvent(int bytes) {
  if (Wire.available() == 4){ // check if full message is recieved
    for (int i=0; i<5; i++){
      char c = Wire.read();
      message[i] = c;
    }
    servo = message[0];
    servopos = int(message.substring(1));
    message = "";
    if (servo == "t") {
      tailServoPos = servopos;
    }
    else {
      yawServoPos = servopos;
    }
  }
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

void blinkDistressSignal(){
  digitalWrite(ledPin, millis() % 150 > 75 ? HIGH : LOW); //Blink every 150ms
}
