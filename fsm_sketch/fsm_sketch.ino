enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

//declare dolphinState as a robotState data structure
enum robotState dolphinState;

//check that systems are okay; assign initial state accordingly
void setup() {
  setupPins();
  if(!areSystemsOK()){
    dolphinState = HELPME;
  }else{
    dolphinState = STANDBY;
  }
}

void loop() {
  if(dolphinState == STANDBY){ 
    //attempt to download mission here with Serial
    if(downloadMissionSuccessful()){ //when get one, start searching
      dolphinState = SEARCH;
    } // else keep waiting!
    return;
  }

  // SENSE
  checkSystem(); // When we do this, we might want to have global parameters that remember the readings.
                 // Then, in our Think section, the functions just access those variables instead of reading anew from sensors.
  readPixyCam();

  // THINK: Figures out which state the robot should be in.
  if(!areSystemsOK()){
    dolphinState = HELPME; 
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

  // Send state, yaw, pitch1, pitch2 to ACT Arduino.

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
      softEstop();
      blinkDistressSignal();
      break;
    default: // Probably some error checking code here, because it really shouldn't hit this statement!
      break;
  }
  pingOCU(); // Input for the message should be decided from serial message (see Think: VICTORY comment).
            // Sinze XBee is on Sense-Think Arduino, pinOCU will happen on here instead of the ACT.
}

void areSystemsOK(){
  // Could break this up into separate check system functions and if one returns bad change state to helpme?
  return !isFloodSensorOK || !isTempSensorOK() || !isBatteryVoltageOK() || !isCheckEStopOK();
}

