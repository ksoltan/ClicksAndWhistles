enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

enum robotState dolphinState;

void setup() {
  setupPins();
  if(!areSystemsOK()){
    dolphinState = HELPME;
  }else{
    dolphinState = STANDBY;
  }
}

void loop() {
  if(dolphinState == STANDBY){ // Can explicitly check here for go function, or use an interrupt from OCU Serial input!
    // Don't know that serial communication can use interrupts. We also need to download the mission here.
    if(downloadMissionSuccessful()){
      dolphinState = SEARCH;
    } // else keep waiting!
    return;
  }

  // SENSE
  checkSystem();
  readPixyCam();

  // THINK: Figures out which state the robot should be in.
  if(!areSystemsOK()){
    dolphinState = HELPME; // Might need to add something to the top of the loop to wait for input from OCU,
                            // but might be fine to make user restart whole system.
  }
  if(dolphinState == SEARCH){
    if(foundMissionBuoy()){
      dolphinState = APPROACH;
    }
  } else if(dolphinState == APPROACH){
    if(isTooCloseToMissionBuoy()){
      dolphinState = VICTORY; // Use this state to update mission.
    } else if(lostMissionBuoy()){
      dolphinState = SEARCH;
    } // Maybe need an else statement to get the coordinates of next movement
  } else if(dolphinState == VICTORY){
    if(!finishedMission()){
      incrementMissionTarget();
      dolphinState = SEARCH;
    } // Maybe need else statement to send to OCU a final victory report.
  }

  // ACT: All we need to pass is the current state of the system to the ACT Arduino.
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
      blinkDestressSignal();
      break;
    default: // Probably some error checking code here, because it really shouldn't hit this statement!
      break;
  }
  pinOCU(); // Input for the message should be decided from serial message (see Think: VICTORY comment).
}

void areSystemsOK(){
  // Could break this up into separate check system functions and if one returns bad change state to helpme?
  return !isFloodSensorOK || !isTempSensorOK() || !isBatteryVoltageOK() || !isCheckEStopOK();
}

