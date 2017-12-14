#include <SPI.h> // Serial Peripheral Interface: the way we communicate with the our PixyCam
#include <Pixy.h>

Pixy pixy; // This object handles the pixy cam

// Pixycam vision variables
bool canSeeMissionBuoy = false;
int buoyX = -1, buoyY = -1; // Position of the buoy: X is 0 to 319, Y is 0 to 199.
                            //-1 indicates we don't know.
int CLOSE_BUOY_AREA = 1000; // Change to tune how close robot comes to buoy before turning
bool missionBuoyIsClose = false; // True indicates we have reached the buoy and can switch to 
                                 // a new target.

long lastTimePixySampled = 0; // Do not sample the pixy too fast, to avoid switching states
int pixySampleTime = 500; // Read pixycam every 500 ms.

// From Arduino API section in http://www.cmucam.org/projects/cmucam5/
// wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
#define X_MAX 319 // maximum horizontal position on pixycam. Min is 0.
#define X_CENTER X_MAX / 2 // horizontal center of the pixycam
#define Y_MAX 199 // maximum vertical position on pixycam. Min is 0.

void setupPixy() {
  pixy.init();
}

int getCharPixyColor(char c){
  switch(c){
    case 'r':
      return 1; // Pixy trained on red as signature 1
    case 'y':
      return 2; // Pixy trained on yellow as signature 2
    case 'w':
      return 3; // Pixy trained on white as signature 3
    default:
      return -1; // This color doesn't exist
  }
}

bool readPixyCam() {
  int blockCount = pixy.getBlocks(); // Number of blocks detected
  missionBuoyIsClose = false;
  canSeeMissionBuoy = (blockCount > 0);
//  XBee.println(blockCount);
  int maxIndex = -1; // Define as -1 in case no detected blocks have the correct color
  int maxArea = 0;
  if (canSeeMissionBuoy) { // If there are potential buoys
    for (int i = 0; i < blockCount; i++) {
      // Only check blocks that are of the color that is being searched for
      if(pixy.blocks[i].signature == getCharPixyColor(mission[current_mission_step])){
        // Choose the block with the largest area to be the supposed buoy
        int area = pixy.blocks[i].width * pixy.blocks[i].height;
        if (area > maxArea) {
          maxArea = area;
          maxIndex = i;
        }
      }
    }
  }
  // If we have detected a buoy, decide whether we are close enough to the buoy as victory
  if(maxIndex > -1){
    buoyX = pixy.blocks[maxIndex].x;
    buoyY = pixy.blocks[maxIndex].y;
//    XBee.println(maxArea);
//    XBee.println(millis() - approach_start_time);
//    XBee.println(millis() - approach_start_time >= min_approach_time);
    if(millis() - approach_start_time >= min_approach_time){
      // Could potentially be close to a buoy, compare area
      XBee.println("Check buoy area!");
      missionBuoyIsClose = (maxArea > CLOSE_BUOY_AREA) ? true : false;
    }
    
//    XBee.println("Found buoy at " + String(buoyX) + "," + String(buoyY));
    digitalWrite(13, HIGH);
    if(missionBuoyIsClose){
      XBee.println("Mission buoy is close");  
    }
  }
  
  else {
    digitalWrite(13, LOW);
//    XBee.println("No buoy found.");
    canSeeMissionBuoy = false;
    buoyX = -1;
    buoyY = -1;
  }
  
  return canSeeMissionBuoy;
}

