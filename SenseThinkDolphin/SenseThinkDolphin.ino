//Clicks and Whistles sense/think Arduino code
//Version 0

#include <SPI.h>
#include <Pixy.h>

/* Set up Global variables */

//Set up pins and outputs
//#define XBEE_INPUT // Will use the XBEE for user input

//#define tempPin A0
//#define floodPin 5
//#define batteryPin A2 // ??Shouldn't there be two battery pins??

// Mission definition variables
bool hasMission = false; // The mission comes from the computer
String mission = ""; //r for red, y for yellow, w for white, e for end (or just look at the length)
int lengthMission = 0;
int current_mission_step = 0; // 0 is the first step of the mission, increment by one until get to the end of the mission

// Pixycam vision variables
Pixy pixy; // This object handles the pixy cam
bool canSeeMissionBuoy = false;
int buoyX = -1, buoyY = -1; // Position of the buoy: X is 0 to 319, Y is 0 to 199. -1 indicates we don't know.
int CLOSE_BUOY_AREA = 3000; // Change to tune how close robot comes to buoy before turning
bool missionBuoyIsClose = false; // Set depending on the size of the buoy the pixyCam sees

long unsigned lastMoveTime = 0; // Keeps track of the last time we calculated tail positions. 0 indicates it's never been initialized.
long unsigned moveTimer = 0; // Keeps track of the tail's phase. Always counts up, but speed can change.
int tailPitch, tailYaw; // The position where the tail *should* be

enum robotState {
  STANDBY, // Waiting for GO signal
  SEARCH, // Searching for next target
  APPROACH, // Approaching target
  VICTORY, // Victory dance on finding target (could also use to go into center of pool to search for next target)
  HELPME // Sensors picked up problem in robot, such as flooding, overheating, or e-stop
};

// declare dolphinState as a robotState data structure
enum robotState dolphinState;

// check that systems are okay; assign initial state accordingly
void setup() {
  setupPins();
  setupPixy();

  Serial.begin(9600); // Start for serial communication
  if(!areSystemsOK()){
    dolphinState = HELPME;
  }else{
    dolphinState = STANDBY;
  }
  printDolphinState();
}

void loop() {
  if(dolphinState == STANDBY){
    downloadMission();//attempt to download mission here with Serial
    if(hasMission){ // when get one, start searching
      dolphinState = SEARCH;
      printDolphinState();
      Serial.print("Mission recieved: ");
      Serial.print(mission);
      Serial.println();
    }
    else return; // Otherwise keep waiting for mission, remain in STANDBY mode
  }

  readPixyCam();

  // THINK: Figures out which state the robot should be in.
  if(!areSystemsOK()){
    dolphinState = HELPME; 
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
    
    if(current_mission_step < lengthMission){
      dolphinState = SEARCH;
      Serial.println(current_mission_step);
      printDolphinState();
    } // Maybe need else statement to send to OCU a final victory report.
  }
}

void incrementMissionTarget(){
  if(current_mission_step >= lengthMission){
    return; // Do not increment mission if the end of it is reached
  }
  current_mission_step++;
}

void printDolphinState(){
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
      Serial.println("HELME");
      break;
    default:
      Serial.println("DODO");
      break;
  }
}

/*
 * Function: areSystemsOK 
 *  Returns: True if all system checks read as OK
 */
bool areSystemsOK(){
  return isFloodSensorOK() && isTempSensorOK() && isBatteryVoltageOK() && isEStopOK();
}

void setupPins(){
//  pinMode(tempPin, INPUT);
//  pinMode(floodPin, INPUT); 
//  pinMode(batteryPin, INPUT);
}

void setupPixy() {
  pixy.init();
}

bool readPixyCam() {
  int blockCount = pixy.getBlocks(); // Number of blocks detected
  missionBuoyIsClose = false;
  canSeeMissionBuoy = (blockCount > 0);
  
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
  
  if(maxIndex > -1){ // There are detected blocks of the correct blocks
    buoyX = pixy.blocks[maxIndex].x;
    buoyY = pixy.blocks[maxIndex].y;
    // Decide if the buoy is close based on the size of the area that the pixycam sees.
    missionBuoyIsClose = (maxArea > CLOSE_BUOY_AREA) ? true : false;
//    Serial.println("Found buoy at " + String(buoyX) + "," + String(buoyY));
    if(missionBuoyIsClose){
      Serial.println("Mission buoy is close");  
    }
    
  }
  else {
//    Serial.println("No buoy found.");
    canSeeMissionBuoy = false;
    buoyX = -1;
    buoyY = -1;
  }
  
  return canSeeMissionBuoy;
}


/*
 * Function: isBatteryVoltageOK 
 *  Relies on: battery pin variable
 *  Returns: is battery ok (based on average of last 10 values)
 */
bool isEStopOK(){
  return true; // NO ESTOP DEFINED YET MUST CHANGE
}

bool isBatteryVoltageOK() {
  #ifdef batteryPin
    static int measurementArray[10] = {1024,1024,1024,1024,1024,1024,1024,1024,1024,1024};
    static int index = 0;
    int voltage = 0;
  
    measurementArray[index] = analogRead(batteryPin); //replace oldest voltage sample
    index = (index+1)%10; //make sure index is within size of the array
  
    for (int i=0; i<10; i++){
      voltage += measurementArray[i]; //sum up voltages
    }
    voltage = voltage / 10; //divide by number of samples
  
    float min_acceptable_voltage = 3.8;
    int min_acceptable_reading = (int) (min_acceptable_voltage * 1024 / 5);  // 5 volts = 1024. We want more than 3.8 from voltage divider
    
    return voltage > min_acceptable_reading;
  #else
    return true;
  
  #endif
}

/*
 * Function: isTempSensorOK 
 *  Relies on: temperature pin variable
 *  Returns: is temperature OK (based on average of last 10 values)
 */
bool isTempSensorOK() {
  #ifdef tempPin
    total_temp = 0;
    num_readings = 10;
    // Read temperature sensor "num_readings" times and average temperature readings
    for(int i = 0; i < num_readings; i++){
      val = analogRead(tempPin); // Read temperature sensor LM35
      total_temp += val;
    }
    // Get average temperature reading
    double avg_temp = total_temp / num_readings;
    // Convert to Celsius, from https://www.allaboutcircuits.com/projects/monitor-temperature-with-an-arduino/
    double avg_temp_C = avg_temp * (5 / 10.24);
    
    return temp < 60; // 60 degrees C is a lot and therefore should not be okay above this.
   
   #else
    return true;
   
   #endif
}

/*
 * Function: isBatteryVoltageOK 
 *  Relies on: floodPin variable
 *  Returns: has the flood sensor detected water
 */
bool isFloodSensorOK() {
  #ifdef floodPin
    return digitalRead(floodPin); // ??IS THIS JUST A DIGITAL READ??
  #else
    return true; // No flood sensor to check
  #endif
  
}

void downloadMission() {
  #ifdef XBEE_INPUT
    // DO SOMETHING
  #else
    hasMission = false;
    if (Serial.available()){ // Serial port available for communication
      mission = Serial.readStringUntil('\n'); // Read until a newline
      lengthMission = mission.length();

      if (lengthMission == 0) {
        Serial.println("Got invalid mission string.");
        return;
      }
      for (int i = 0; i < lengthMission; i++) {
        char char_i = mission[i];
        if (char_i != 'r' && char_i != 'y' && char_i != 'w') {
          Serial.println("Got invalid mission string.");
          return;
        }
      }
      hasMission = true;
    }
  #endif
  
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

