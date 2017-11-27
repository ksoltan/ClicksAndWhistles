enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

// declare dolphinState as a robotState data structure
enum robotState dolphinState;

// Mission Variables
// Mission definition variables
bool hasMission = false; // The mission comes from the computer
String mission = ""; //r for red, y for yellow, w for white, e for end (or just look at the length)
int lengthMission = 0;
int current_mission_step = 0; // 0 is the first step of the mission, increment by one until get to the end of the mission
