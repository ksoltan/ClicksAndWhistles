/* Set up Global variables
*/

//Set up pins and outputs
int ledPin = 8;
int tempPin = A0;
int floodPin = 5; //Actually a digital sensor
int batteryPin = A2;

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
  if(!areSystemsOK()){
    dolphinState = HELPME;
  }else{
    dolphinState = STANDBY;
  }
}

void loop() {
  if(dolphinState == STANDBY){ 
    //attempt to download mission here with Serial
    if(downloadMissionSuccessful()){ //when get one, start searching
      dolphinState = SEARCH;
    } // else keep waiting!
    return;
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
  sendActParameters(); // Send state, yaw, pitch1, pitch2 to ACT Arduino.
  pingOCU(); // Input for the message should be decided from serial message (see Think: VICTORY comment).
            // Sinze XBee is on Sense-Think Arduino, pinOCU will happen on here instead of the ACT.

}

/*
 * Function: areSystemsOK 
 *  Returns: True if all system checks read as OK
 */
bool areSystemsOK(){
  // Could break this up into separate check system functions and if one returns bad change state to helpme?
  return !isFloodSensorOK || !isTempSensorOK() || !isBatteryVoltageOK() || !isCheckEStopOK();
}

/*
 * Function: isBatteryVoltageOK 
 *  Relies on: battery pin variable
 *  Returns: is battery ok (based on average of last 10 values)
 */
bool isBatteryVolatageOK(){
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
bool isTempSensorOK(){
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
  if (temp < 60){ //60 degrees C is a lot.
    return True
  }
  else {
    return False
  }
}

/*
 * Function: isBatteryVoltageOK 
 *  Relies on: floodPin variable
 *  Returns: has the flood sensor sensed water
 */
bool isFloodSensorOK(){
  return digitalRead(floodPin);
}


