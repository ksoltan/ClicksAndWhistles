// Clicks and Whistles sense/think Arduino testing code.
// Author: Katya

#include <SPI.h>
#include <Pixy.h>
#include <Wire.h>

//Set up pins and outputs
//#define tempPin A0
//#define floodPin 5
//#define batteryPin A2 // ??Shouldn't there be two battery pins??

/* Set up Global variables */
#define ACT_ADDRESS 8 // Address at which ACT Arduino is expecting Serial communication

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
// From Arduino API section in http://www.cmucam.org/projects/cmucam5/wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
#define X_MAX 319 // maximum horizontal position on pixycam. Min is 0.
#define X_CENTER X_MAX / 2 // horizontal center of the pixycam
#define Y_MAX 199 // maximum vertical position on pixycam. Min is 0.

// Servo Movement Timing Variables
long unsigned lastMoveTime = 0; // Keeps track of the last time we calculated tail positions. 0 indicates it's never been initialized.
long unsigned moveTimer = 0; // Keeps track of the tail's phase. Always counts up, but speed can change.
int moveDelayTime = 50; // Every couple of ms recalculate tail/yaw positioning.
// Must make sure that the time delay is smaller than the time step of moving the servo at a constant period
int fastFlapPeriod = 1 * 1000; // 1 full cycle in x milliseconds
int slowFlapPeriod = 2 * 1000; // 1 full cycle in x milliseconds

// Servo Positions: ??Need calibration?? Look into making sure that the servoLeft/Right are not negative, since transmitting unsigned bytes.
int yawServoLeft = 0; // degrees
int yawServoRight = 180; // degrees
int tailServoLeft = 0; // degrees
int tailServoRight = 180; // degrees
int servoAngleChange = 5; // Smallest value by which to increment servo position
int yawServoPos = (yawServoRight + yawServoLeft) / 2; // Initialize to the midpoint.
int tailServoPos = (tailServoRight + tailServoLeft) / 2; // Initialize to the midpoint.
int tailDir = 1; // If 1: yaw servo is moving to the right (incrementing). If -1: yaw servo is moving to the left (decrementing)

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
  setupI2C();
  
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
//    Serial.println("Waiting");
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

//  sendActParams(); // Ping the ACT Arduino
}

// Transmits STATE, YAW POSITION, TAIL POSITION
void sendActParams(){
  if(getActParams()){
    Wire.beginTransmission(ACT_ADDRESS);
    // Transmit state
    Wire.write(dolphinState);
    Wire.write(",");
    // Transmit yaw position
    Wire.write(yawServoPos);
    Wire.write(",");
    Wire.write(tailServoPos);
    Wire.write(";");
    Wire.endTransmission();
  }
  
}

// Return true if it is time to send act parameters
boolean getActParams(){
  if(millis() - lastMoveTime >= moveDelayTime){
    // check for updates to the servo positions
    switch(dolphinState){
    case STANDBY:
      getStandbyActParams();
      break;
      
    case SEARCH:
      getSearchActParams();
      break;
      
    case APPROACH:
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
  updateTailPosition(fastFlapPeriod);
}

// When the robot is approaching, want to keep buoy centered in vision
// Can achieve by beating the tail at a reasonable frequency and adjusting yaw continuously
void getApproachActParams(){
  if(buoyX < X_CENTER){ // Buoy us to the left of center
    yawServoPos -= servoAngleChange; // Move the servo to the left
  }
  if(buoyX > X_CENTER){ // Buoy is to the right of center
    yawServoPos += servoAngleChange;
  }
  // otherwise, the buoy is straight on, do not change yaw. We can also change the statements above to give
  // more leeway. Say, if the center of the blob is within 10 of the center, keep going straight.

  updateTailPosition(slowFlapPeriod);
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

// Moving the tail position at a constant frequency
void updateTailPosition(int period){
  int numStepsInCycle = 2 * abs(yawServoRight - yawServoLeft) / servoAngleChange; // Number of steps to complete full period with servoAngleChange update
  int timeToMove = period / numStepsInCycle; // Need to move servo every x milliseconds to achieve this frequency
  
  if(millis() - lastMoveTime >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(tailServoPos + tailDir * servoAngleChange > tailServoRight || tailServoPos + tailDir * servoAngleChange < tailServoLeft){
      // Switch direction
      tailDir *= -1;
    }
    tailServoPos += tailDir * servoAngleChange; // Update tail servo pos
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
      Serial.println("HELPME");
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

void setupI2C(){
  Wire.begin(8); // The address is optional for the master
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
  
  if(maxIndex > -1){
    buoyX = pixy.blocks[maxIndex].x;
    buoyY = pixy.blocks[maxIndex].y;
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
  hasMission = false;
  if (Serial.available()){ // Serial port available for communication
    mission = Serial.readStringUntil('\n'); // Read until a newline
    lengthMission = mission.length();

    if (lengthMission == 0) {
      Serial.println("Got invalid mission string.");
      return;
    }

    // Read the mission string, check for invalid characters,
    // 'r' = red, 'y' = yellow, 'w' = white
    for (int i = 0; i < lengthMission; i++) {
      char char_i = mission[i];
      if (char_i != 'r' && char_i != 'y' && char_i != 'w') {
        Serial.println("Got invalid mission string.");
        return;
      }
    }
    hasMission = true;
  }
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

