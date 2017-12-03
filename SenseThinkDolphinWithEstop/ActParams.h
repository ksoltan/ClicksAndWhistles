// Servo Movement Timing Variables
long unsigned lastMoveTime = 0; // Keeps track of the last time we calculated tail positions. 0 indicates it's never been initialized.
long unsigned moveTimer = 0; // Keeps track of the tail's phase. Always counts up, but speed can change.
int moveDelayTime = 50; // Every couple of ms recalculate tail/yaw positioning.
// Must make sure that the time delay is smaller than the time step of moving the servo at a constant period
int fastFlapPeriod = 1 * 1000; // 1 full cycle in x milliseconds
int slowFlapPeriod = 2 * 1000; // 1 full cycle in x milliseconds

// Servo Positions: ??Need calibration?? Look into making sure that the servoLeft/Right are not negative, since transmitting unsigned bytes.
int yawServoLeft = 10; // degrees
int yawServoRight = 150; // degrees
int tailServoLeft = 0; // degrees
int tailServoRight = 180; // degrees
int servoAngleChange = 5; // Smallest value by which to increment servo position
int yawServoPos = (yawServoRight + yawServoLeft) / 2; // Initialize to the midpoint.
int tailServoPos = (tailServoRight + tailServoLeft) / 2; // Initialize to the midpoint.
int tailDir = 1; // If 1: yaw servo is moving to the right (incrementing). If -1: yaw servo is moving to the left (decrementing)

// Moving the tail position at a constant frequency
void updateTailPosition(int period){
  int numStepsInCycle = 2 * abs(tailServoRight - tailServoLeft) / servoAngleChange; // Number of steps to complete full period with servoAngleChange update
  int timeToMove = period / numStepsInCycle; // Need to move servo every x milliseconds to achieve this frequency
  
  if(millis() - lastMoveTime >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(tailServoPos + tailDir * servoAngleChange > tailServoRight || tailServoPos + tailDir * servoAngleChange < tailServoLeft){
      // Switch direction
      tailDir *= -1;
    }
    tailServoPos += tailDir * servoAngleChange; // Update tail servo pos
    lastMoveTime = millis();
  }
}

void getStandbyActParams(){
  yawServoPos = (yawServoRight + yawServoLeft) / 2; // Initialize to the midpoint.
  tailServoPos = (tailServoRight + tailServoLeft) / 2; // Initialize to the midpoint.
}

// When the robot is searching and no buoy has yet been found, want it to continuously circle
// Can achieve by setting yaw to extreme (for tight turning radius) and beating the tail
// Want to turn in the same direction because if the transition to the Approach state causes
// the robot to lose the buoy, want to keep searching in that direction again.
void getSearchActParams(){
  yawServoPos = yawServoRight; // stays constant.
  updateTailPosition(slowFlapPeriod);
}

// When the robot is approaching, want to keep buoy centered in vision
// Can achieve by beating the tail at a reasonable frequency and adjusting yaw continuously
void getApproachActParams(){
  if(buoyX < X_CENTER){ // Buoy us to the left of center
    yawServoPos -= servoAngleChange; // Move the servo to the left
  }
  if(buoyX > X_CENTER){ // Buoy is to the right of center
    yawServoPos += servoAngleChange;
  }
  // otherwise, the buoy is straight on, do not change yaw. We can also change the statements above to give
  // more leeway. Say, if the center of the blob is within 10 of the center, keep going straight.

  updateTailPosition(slowFlapPeriod);
}

// For now, the victory transition state can stay in its exact position
// Can do a funky thing when the mission has been ended later.
void getVictoryActParams(){
  yawServoPos = yawServoPos;
  tailServoPos = tailServoPos;
}

// Helpme mode has to press the estop, so the servo positions will not change.
void getHelpmeActParams(){
  yawServoPos = yawServoPos;
  tailServoPos = tailServoPos;
}

// Return true if it is time to send act parameters
boolean getActParams(){
  if(millis() - lastMoveTime >= moveDelayTime){
    // check for updates to the servo positions
    switch(dolphinState){
    case STANDBY:
      getStandbyActParams();
      break;
      
    case SEARCH:
      getSearchActParams();
      break;
      
    case APPROACH:
      getApproachActParams();
      break;
      
    case VICTORY:
      getVictoryActParams();
      break;
      
    case HELPME: // Default case is helpme case. If our state is not one of the above, we have a problem
    default:
      getHelpmeActParams();
      break;
    }
    lastMoveTime = millis(); // Update timer
    return true;
  }
  else{
    return false;
  }
}

