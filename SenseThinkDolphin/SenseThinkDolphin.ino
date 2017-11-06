//Clicks and Whistles sense/think Arduino code
//Version 0

#include <SPI.h>
#include <Pixy.h>

/* Set up Global variables */

//Set up pins and outputs
int ledPin = 8;
int tempPin = A0;
int floodPin = 5;
int batteryPin = A2;

bool hasMission = false; //The mission comes from the computer
String mission = ""; //r for red, y for yellow, w for white, e for end (or just look at the length)

Pixy pixy; //This object handles the pixy cam
bool canSeeBuoy = false;
int buoyX = -1, buoyY = -1; //X is 0 to 319, Y is 0 to 199. -1 indicates we don't know.

long unsigned lastMoveTime = 0; //Keeps track of the last time we calculated tail positions. 0 indicates it's never been initialized.
long unsigned moveTimer = 0; //Keeps track of the tail's phase. Always counts up, but speed can change.
int tailPitch, tailYaw; //The position where the tail *should* be

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
    downloadMission();//attempt to download mission here with Serial
    if(hasMission){ //when get one, start searching
      dolphinState = SEARCH;
    } // else keep waiting!
    else return;
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
  findActParameters(); // Calculate tail positions
  sendActParameters(); // Send state, yaw, pitch to ACT Arduino.
  pingOCU(); // Input for the message should be decided from serial message (see Think: VICTORY comment).
            // Since XBee is on Sense-Think Arduino, pingOCU will happen on here instead of the ACT.

}

/*
 * Function: areSystemsOK 
 *  Returns: True if all system checks read as OK
 */
bool areSystemsOK(){
  // Could break this up into separate check system functions and if one returns bad change state to helpme?
  return isFloodSensorOK && isTempSensorOK() && isBatteryVoltageOK() && isCheckEStopOK();
}

void setupPins(){
  pinMode(ledPin, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(floodPin, INPUT); 
  pinMode(batteryPin, INPUT);
}

void setupPixy() {
  pixy.init();
}

bool readPixyCam() {
  
  int blockCount = pixy.getBlocks();

  canSeeBuoy = (blockCount > 0);
  
  if (canSeeBuoy) {
    int maxArea = 0, maxIndex;
    for (int i = 0; i < blockCount; i++) {
      int area = pixy.blocks[i].width * pixy.blocks[i].height;
      if (area > maxArea) {
        maxArea = area;
        maxIndex = i;
      }
    }

    buoyX = pixy.blocks[maxIndex].x;
    buoyY = pixy.blocks[maxIndex].y;

    Serial.println("Found buoy at " + String(buoyX) + "," + String(buoyY));
  } else {
    Serial.println("No buoy found.");
    buoyX = -1;
    buoyY = -1;
  }
  
}



/*
 * Function: isBatteryVoltageOK 
 *  Relies on: battery pin variable
 *  Returns: is battery ok (based on average of last 10 values)
 */
bool isBatteryVolatageOK() {
  static int measurementArray[10] = {1024,1024,1024,1024,1024,1024,1024,1024,1024,1024};
  static int index = 0;
  int voltage = 0;

  measurementArray[index] = analogRead(batteryPin); //replace oldest voltage sample
  index = (index+1)%10; //make sure index is within size of the array

  for (int i=0; i<10; i++){
    voltage += measurementArray[i]; //sum up voltages
  }
  voltage = voltage/10; //divide by number of samples
  if (voltage > 776){ //5 volts = 1024. We want more than 3.8 from voltage divider
    return True
  }
  else {
    return False
  }
}

/*
 * Function: isTempSensorOK 
 *  Relies on: temperature pin variable
 *  Returns: is temperature OK (based on average of last 10 values)
 */
bool isTempSensorOK() {
  static double measurementArray[10] = {0,0,0,0,0,0,0,0,0,0};
  static int index = 0;
  uint16_t val;
  double dat;
  double temp = 0;
  
  val=analogRead(A0);//Connect LM35 on Analog 0
  dat = (double) val * (5/10.24); //temp in celcius
  measurementArray[index] = dat;//replace oldest temp sample
  index={index+1}%10; //make sure index is within size of the array
  
  for (int i=0; i<10; i++){
    temp += measurementArray[i]; //sum up voltages
  }
  temp = temp/10; //divide by number of samples
  
  if (temp < 60) return(true); //60 degrees C is a lot.
  else return(false);

}

/*
 * Function: isBatteryVoltageOK 
 *  Relies on: floodPin variable
 *  Returns: has the flood sensor sensed water
 */
bool isFloodSensorOK() {
  return digitalRead(floodPin);
}

//This finds the angles for the tail servos. It depends on the robot's
// state, the pixycam's info, and time.
void findActParameters() {

  //First, do some timing stuff.

  //If lastMoveTime has not been initialized, initialize it. Note that this makes the current iteration return no motion, probably.
  if (lastMoveTime == 0) lastMoveTime = millis();

  switch (robotState){ //Mess with the timer
    case SEARCH:
    case APPROACH:
      moveTimer += (millis() - lastMoveTime); //Increment the move timer at one unit per millisecond
      break;
    case VICTORY:
      moveTimer += (millis() - lastMoveTime)/2; //Half speed in victory state
      break;
    case STANDBY:
    case HELPME:
    default:
      moveTimer += 0; //Don't move if we haven't started, or there's a problem
      break;
  } //End switch

  tailPitch = (127.5 * sin(moveTimer/6000.0)) + 127.5; //Sine wave, period of six seconds. All math is floats but output is int from 0-256.

  int tailYawGoal; //Where the tail "wants" to be. We don't want it moving too fast, though.

  if (robotState == APPROACH && buoyX != -1)  //If we're going towards a buoy, and we can see it
    tailYawGoal = (buoyX/319.0)*255.0; //Convert from (0-319) to (0-255)sssssssss
  else //Otherwise...
    tailYawGoal = 127; //Center the tail
    //Note that technically, the center of the range is 127.5. But this should be close enough.

  //In order to yaw the tail slowly, we just move it a bit, based on the cycle time
  if (tailYaw > tailYawGoal) tailYaw -= (millis() - lastMoveTime) / 4; //About 1 second to go thru the full yaw range
  else if (tailYaw < tailYawGoal) tailYaw += (millis() - lastMoveTime) / 4;
  else tailYaw += 0; //If it's where it should be, don't adjust it

}


void downloadMission() {

  hasMission = false;

  if (!Serial.available()) return;

  mission = Serial.readStringUntil('\n'); //Read until a newline

  if (mission.length == 0 || mission[mission.length-1] != e) {
    Serial.println("Got invalid mission string.");
    return;

  for (int i = 0; i < mission.length-1; i++) {
    if (mission[i] != 'r' && mission[i] != 'y' && mission[i] != 'w') {
      Serial.println("Got invalid mission string.");
      return;
    } 
  }

  hasMission = true;

}
