// XBee Communication
// To avoid using the Serial Monitor, all communication is done through Software Serial.
// The Sparkfun XBee board used pins 2 (DOUT - RX) and 3 (DIN - TX).
#include <SoftwareSerial.h>
SoftwareSerial XBee(2, 3); // RX, TX

enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of
           // pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

// declare dolphinState as a robotState data structure
enum robotState dolphinState;

// Mission Variables

// Mission definition variables
bool hasMission = false; // The mission comes from the computer
String mission = ""; //r for red, y for yellow, w for white, e for end
int lengthMission = 0;
int current_mission_step = 0; // 0 is the first step of the mission, increment until the end

boolean approaching_timer_set = false;
long approach_start_time = 0;
long min_approach_time = 10 * 1000L; // Give robot 15s to move before considering if the 
                   // area of the buoy is large enough, due to different brightnesses of LEDs

void resetMission(){
  hasMission = false;
  mission = "";
  lengthMission = 0;
  current_mission_step = 0;
}

