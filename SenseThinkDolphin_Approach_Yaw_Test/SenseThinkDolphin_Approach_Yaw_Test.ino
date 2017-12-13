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

//Artifically update x position of buoy.
long movePixyTime = 0; // last time we moved the buoy center.
signed int pixyMoveDir = 50; // are we moving it right (positive) or left (negative)

// check that systems are okay; assign initial state accordingly
void setup() {
  setupPins();
  setupPixy();
  setupI2C();
  setupMissionCom();
//  Serial.begin(9600);
   
  if(!areSystemsOK()){
    Serial.println("HALP");
    dolphinState = HELPME;
    printDolphinState();
  }else{
    dolphinState = APPROACH;
    printDolphinState();
    XBee.print("Approach!");
  }
  release_motors(); // Make sure servos are off at the beginning of mission.
  buoyX = 0;
}

void loop() {
//  if(dolphinState == STANDBY){
////    XBee.println("Waiting");
//    hasMission = downloadMission();//attempt to download mission here with XBee
//    if(hasMission){ // when get one, start searching
//      dolphinState = SEARCH;
//      release_motors(); // Allow power to the motors.
//      current_mission_step = 0;
//      XBee.print("\nMission recieved: ");
//      XBee.print(mission);
//      XBee.println();
//      printDolphinState();
//    }
//    else return; // Otherwise keep waiting for mission, remain in STANDBY mode
//  }

//  readPixyCam();
//  updateDolphinState();
  if(millis() - movePixyTime > 3000){
    // artificially move pixy left and right
    pixyMoveDir *= -1;
    buoyX = X_CENTER + pixyMoveDir * 100;
    XBee.print("Diff: ");
    XBee.print(X_CENTER - buoyX);
    XBee.print(".\n");
    movePixyTime = millis();
  }
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

