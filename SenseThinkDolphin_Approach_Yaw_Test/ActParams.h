// Servo Movement Timing Variables
long unsigned lastTailMoveTime = 0; // Keeps track of the last time we calculated tail servo positions. 0 indicates it's never been initialized.
long unsigned lastUpdateTime = 0; // Keeps track of the last time at which we checked to update act parameters (yaw and tail positions).
long unsigned lastYawMoveTime = 0;

int updateDelayTime = 10; // Every couple of ms recalculate tail/yaw positioning. This value has to be low if you want the tail to flap at a very high frequency, because otherwise, it doesn't update fast enough.
// Must make sure that the time delay is smaller than the time step of moving the servo at a constant period
int fastFlapPeriod = 1.8 * 1000; // 1 full cycle in x milliseconds
int slowFlapPeriod = 2 * 1000; // 1 full cycle in x milliseconds

// Servo Positions: ??Need calibration?? Look into making sure that the servoLeft/Right are not negative, since transmitting unsigned bytes.
int yawServoLeft = 30; // degrees
int yawServoRight = 130; // degrees
int tailServoLeft = 0; // degrees
int tailServoRight = 180; // degrees
int servoAngleChange = 2; // Smallest value by which to increment servo position
int yawServoPos = (yawServoRight + yawServoLeft) / 2; // Initialize to the midpoint.
int tailServoPos = (tailServoRight + tailServoLeft) / 2; // Initialize to the midpoint.
int tailDir = 1; // If 1: yaw servo is moving to the right (incrementing). If -1: yaw servo is moving to the left (decrementing)
int numStepsInTailCycle = 2 * abs(tailServoRight - tailServoLeft) / servoAngleChange; // Number of steps to complete full period (up, down, up) motion for the tail with servoAngleChange update
int numStepsInYawApproachAdjustment = 2*abs(yawServoRight - yawServoLeft) / servoAngleChange; // Number of steps to complete full sweep (right, left) motion for yaw with servoAngleChange update
int yawDir = 1;

// Moving the tail position at a constant frequency up and down
void updateTailPosition(int period){
  int timeToMove = period / numStepsInTailCycle; // Need to move servo every x milliseconds to achieve this frequency
  
  if(millis() - lastTailMoveTime >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(tailServoPos + tailDir * servoAngleChange > tailServoRight || tailServoPos + tailDir * servoAngleChange < tailServoLeft){
      // Switch direction
      tailDir *= -1;
    }
    
    tailServoPos += tailDir * servoAngleChange; // Update tail servo pos
    lastTailMoveTime = millis();
  }
}

void updateYawPosition(int period){
//  int timeToMove = period / numStepsInYawApproachAdjustment; // Need to move servo every x milliseconds to achieve this frequency
  int timeToMove = 50;
  if(millis() - lastYawMoveTime >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(yawServoPos + yawDir * servoAngleChange > yawServoRight || yawServoPos + yawDir * servoAngleChange < yawServoLeft){
      // Don't move.
//      yawServoPos = yawServoPos;
  } else{
    yawServoPos += yawDir * servoAngleChange;
  }
    lastYawMoveTime = millis();
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
  XBee.print(X_CENTER - buoyX);
  if(X_CENTER - buoyX > 5){ // Buoy is to the left of center
    yawDir = -1; // Compensate left
    updateYawPosition(slowFlapPeriod);
    XBee.print("Yaw Left: ");
    XBee.print(yawServoPos);
    XBee.print("\n");
  }
  if(buoyX - X_CENTER > 5){ // Buoy is to the right of center
    yawDir = 1; // Compensate right
    updateYawPosition(slowFlapPeriod);
    XBee.print("Yaw Right: ");
    XBee.print(yawServoPos);
    XBee.print("\n");
  }
//  // otherwise, the buoy is straight on, do not change yaw. We can also change the statements above to give
//  // more leeway. Say, if the center of the blob is within 10 of the center, keep going straight.

  updateTailPosition(fastFlapPeriod);
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
// Currently, defined to send updates at a fixed time interval (which is pretty small).
// Can change to update only when the tail has moved, or the pixycam not being in the center would trigger a yaw adjustment.
boolean getActParams(){
  if(millis() - lastUpdateTime >= updateDelayTime){
    XBee.println("*");
//    Serial.println(lastTailMoveTime);
    // check for updates to the servo positions
    switch(dolphinState){
    case STANDBY:
      getStandbyActParams();
      break;
      
    case SEARCH:
      getSearchActParams();
      break;
      
    case APPROACH:
//      XBee.println("APPROACH MODE");
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
//    Serial.println(tailServoPos);
    
    lastUpdateTime = millis(); // Update timer
    return true;
  }
  
  else{
    return false;
  }
}

