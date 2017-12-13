// Control code for the Sense/Think Arduino of Team Clicks and Whistles robotic dolphin.

// Clicks and Whistles sense/think Arduino testing code.
// Author: Katya

#include "DolphinVariables.h" // Handles dolphin state and mission variables that are used between modules (pixy/mission com/act params)
#include "SensorLogic.h" // Handles sensors and system check functions
#include "PixyLogic.h" // Handles pixycam and image processing
#include "ActCommunicationLogic.h" // Handles I2C communication with ACT Arduino
#include "MissionCommunicationLogic.h" // Handles download of mission from XBee

// DEBUGGING VARIABLES
#define debugDolphinState true

// check that systems are okay; assign initial state accordingly
void setup() {
  setupPins();
  setupPixy();
  setupI2C();
  setupMissionCom();
  
  if(!areSystemsOK()){
    dolphinState = HELPME;
    printDolphinState();
  }else{
    dolphinState = STANDBY;
    printDolphinState();
    XBee.print("Input Mission (r-red, y-yellow, w-white): ");
  }
  //freeze_motors(); // Make sure servos are off at the beginning of mission.
  release_motors(); // Allow power to the motors.

}

void loop() {
  if(dolphinState == STANDBY){
//    XBee.println("Waiting");
    hasMission = downloadMission();//attempt to download mission here with XBee
    if(hasMission){ // when get one, start searching
      dolphinState = SEARCH;  
      current_mission_step = 0;
      XBee.print("\nMission recieved: ");
      XBee.print(mission);
      XBee.println();
      printDolphinState();
    }
    else return; // Otherwise keep waiting for mission, remain in STANDBY mode
  }

  readPixyCam();
  updateDolphinState();
  sendActParams(); // Ping the ACT Arduino
}

void updateDolphinState(){
//  XBee.println("Updating state");
    // THINK: Figures out which state the robot should be in.
  if(!areSystemsOK()){
    dolphinState = HELPME;
    freeze_motors();
    printDolphinState();
    freeze_motors();
  }
  
  if(dolphinState == SEARCH){
    if(canSeeMissionBuoy){
      dolphinState = APPROACH;
      printDolphinState();
    }
  }
  
  else if(dolphinState == APPROACH){
    if(missionBuoyIsClose){
      dolphinState = VICTORY; // Use this state to update mission.
      printDolphinState();
    } else if(!canSeeMissionBuoy){
      dolphinState = SEARCH;
      printDolphinState();
    }
  }
  
  else if(dolphinState == VICTORY){
    incrementMissionTarget();
    if(current_mission_step == -1){
      dolphinState = STANDBY;
      resetMission();
      freeze_motors();
      XBee.print("Input Mission (r-red, y-yellow, w-white): ");
    }
    else
      dolphinState = SEARCH;
      XBee.println(current_mission_step);
      printDolphinState();
  }
}
// Update the target buoy when one is found.
void incrementMissionTarget(){
  current_mission_step++;
  if(current_mission_step >= lengthMission){
    current_mission_step = -1; // Indicate mission is over.
  }
}

void printDolphinState(){
  #ifdef debugDolphinState
    switch(dolphinState){
      case STANDBY:
        XBee.println("STANDBY");
        break;
      case SEARCH:
        XBee.println("SEARCH");
        break;
      case APPROACH:
        XBee.println("APPROACH");
        break;
      case VICTORY:
        XBee.println("VICTORY");
        break;
      case HELPME:
        XBee.println("HELPME");
        break;
      default:
        XBee.println("DODO");
        break;
    }
  #endif
}

