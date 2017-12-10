// Test to check that oscillatory up-down motion of tail can be performed independent of other updates to the system.
// States of the robot are cycled through, and the output of the tailServoPos is calculated. Yaw is not included, since it depends on the pixycam for now.

#define debugDolphinState true

enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

// declare dolphinState as a robotState data structure
enum robotState dolphinState;
#include "ActParams.h"

void setup(){
  Serial.begin(9600);
  dolphinState = STANDBY;
}

long last_update_time = 0;
int time_between_states = 5000;

void loop(){
  if(millis() - last_update_time > time_between_states){
    last_update_time = millis();
    updateState();
    printDolphinState();
  }
  getActParams();
}

void updateState(){
  switch(dolphinState){
    case STANDBY:
      dolphinState = SEARCH;
      break;
    case SEARCH:
      dolphinState = APPROACH;
      break;
    case APPROACH:
      dolphinState = VICTORY;
      break;
    case VICTORY:
      dolphinState = HELPME;
      break;
    case HELPME:
    default:
      dolphinState = STANDBY;
      break;
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

