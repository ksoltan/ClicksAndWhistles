
static int ledPin = 0; //What pin is this actually?



enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

enum robotState dolphinState;

//check that systems are okay; assign initial state accordingly
void setup() {
  setupActPins();
}

void loop() {
  // Receive instructions from Sense/Think Arduino
  receiveActParameters(); // Receive state, yaw, pitch1, pitch2 from SENSE/THINK Arduino.
  
  switch(dolphinState){
    case SEARCH:
      searchForTarget(); // Could involve revolving in place by some number of degrees.
      blinkSearchSignal();
      break;
    case APPROACH:
      moveTowardsTarget();
      blinkApproachSignal();
      break;
    case VICTORY:
      doVictoryWiggle();
      blinkVictorySignal();
      break;
    case HELPME:
      softEstop();
      blinkDistressSignal();
      break;
    default: // Standby mode or not yet assigned
      // Do nothing!
      blinkStandbySignal();
      break;
  }
}

void setupActPins(){
  
  pinMode(ledPin, OUTPUT);

}

void receiveActParameters(){
  // Receive Serial connections
  // Update state, yaw, pitch1, pitch2 global variables.
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
