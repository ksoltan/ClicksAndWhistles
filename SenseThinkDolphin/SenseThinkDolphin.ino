//Clicks and Whistles sense/think Arduino code
//Version 0

#include <SPI.h>
#include <Pixy.h>

Pixy pixy; //This object handles the pixy cam
bool canSeeBuoy = false;
int buoyX = -1, buoyY = -1; //X is 0 to 319, Y is 0 to 199. -1 indicates we don't know.

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
  setupPixy();

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

  // Communicate with Act and computer
  sendActParameters(); // Send state, yaw, pitch1, pitch2 to ACT Arduino.
  pingOCU(); // Input for the message should be decided from serial message (see Think: VICTORY comment).
            // Sinze XBee is on Sense-Think Arduino, pinOCU will happen on here instead of the ACT.
  
void areSystemsOK(){
  // Could break this up into separate check system functions and if one returns bad change state to helpme?
  return isFloodSensorOK && isTempSensorOK() && isBatteryVoltageOK() && isCheckEStopOK();
}

void setupPixy() {
  pixy.init();
}

bool readPixyCam(int ) {
  
  int blockCount = pixy.getBlocks();

  canSeeBuoy = (blockCount > 0);
  
  if (canSeeBuoy) {
    int maxArea = 0, maxIndex;
    for (int i = 0; i < blockCount; i++) {
      area = pixy.blocks[i].width * pixy.blocks[i].height > maxArea
      if (area > maxArea) {
        maxArea = area;
        maxIndex = i;
      }
    }

    buoyX = pixy.blocks[maxIndex].x;
    buoyY = pixy.blocks[maxIndex].y;
  } else {
    buoyX = -1;
    buoyY = -1;
  }
}
