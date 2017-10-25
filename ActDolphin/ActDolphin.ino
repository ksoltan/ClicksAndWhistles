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
  
}

void receiveActParameters(){
  // Receive Serial connections
  // Update state, yaw, pitch1, pitch2 global variables.
}



