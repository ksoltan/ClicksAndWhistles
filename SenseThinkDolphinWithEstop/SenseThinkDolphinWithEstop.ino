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
  }else{
    dolphinState = STANDBY;
  }
  freeze_motors(); // Make sure servos are off at the beginning.
  
  printDolphinState();
}

void loop() {
  if(dolphinState == STANDBY){
//    Serial.println("Waiting");
    hasMission = downloadMission();//attempt to download mission here with Serial
    if(hasMission){ // when get one, start searching
      dolphinState = SEARCH;
      release_motors(); // Allow power to the motors.
      current_mission_step = 0;
      printDolphinState();
      Serial.print("Mission recieved: ");
      Serial.print(mission);
      Serial.println();
    }
    else return; // Otherwise keep waiting for mission, remain in STANDBY mode
  }

  readPixyCam();
  updateDolphinState();
  sendActParams(); // Ping the ACT Arduino
}

void updateDolphinState(){
    // THINK: Figures out which state the robot should be in.
  if(!areSystemsOK()){
    dolphinState = HELPME;
    freeze_motors();
    printDolphinState();
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
    } // Maybe need an else statement to get the coordinates of next movement
  }
  
  else if(dolphinState == VICTORY){
    incrementMissionTarget();
    if(current_mission_step == -1){
      dolphinState = STANDBY; // Maybe need else statement to send to OCU a final victory report.
      resetMission();
      freeze_motors();
    }
    else
      dolphinState = SEARCH;
      Serial.println(current_mission_step);
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
        Serial.println("STANDBY");
        break;
      case SEARCH:
        Serial.println("SEARCH");
        break;
      case APPROACH:
        Serial.println("APPROACH");
        break;
      case VICTORY:
        Serial.println("VICTORY");
        break;
      case HELPME:
        Serial.println("HELPME");
        break;
      default:
        Serial.println("DODO");
        break;
    }
  #endif
}

