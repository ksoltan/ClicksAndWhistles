#include <Wire.h>
static int ledPin = 0; //What pin is this actually?
int x = 0;
char message[10];
char servo[2];
int servopos;

int servotpos; //tail servo
int servobpos; //body servo

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
  receiveActParameters(); // Receive state, yaw, pitch1, pitch2 from SENSE/THINK Arduino.
  
  analogWrite(servotpin, servotpos);
  analogWrite(servobpin, servotpos);
}

void setupActPins(){
  
  pinMode(ledPin, OUTPUT);

}

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
  if (x != ";"){
    message = message + x;
  }
  else {
    servo = message[0];
    servopos = int(message.substring(1));
    message = "";
    if (servo == "t") {
      servotpos = servopos;
    }
    else {
      servobpos = servopos;
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
