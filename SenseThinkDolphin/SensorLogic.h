//Set up pins and outputs
//#define tempPin A0
//#define floodPin 5
//#define batteryPin A2 // ??Shouldn't there be two battery pins??
#define estop_pin 8

void setupPins(){
  #ifdef tempPin
    pinMode(tempPin, INPUT);
  #endif

  #ifdef floodPin
    pinMode(floodPin, INPUT); 
  #endif

  #ifdef batteryPin
    pinMode(batteryPin, INPUT);
  #endif

<<<<<<< HEAD
  pinMode(estop_pin, OUTPUT);
}

// Press the E-stop in code
=======
  #ifdef estop_pin
    pinMode(estop_pin, OUTPUT);
  #endif
}

// Press the E-stop in code. Writing 0 to the relay will stop passing current through it to the servos (make it open).
>>>>>>> debug_logic
void freeze_motors(){
  digitalWrite(estop_pin, LOW);
}

<<<<<<< HEAD
// Release the E-stop in code
=======
// Release the E-stop in code. Allow servos to get power by closing the relay.
>>>>>>> debug_logic
void release_motors(){
  digitalWrite(estop_pin, HIGH);
}

bool isEStopOK(){
  return true; // NO ESTOP DEFINED YET MUST CHANGE
}

/*
 * Function: isBatteryVoltageOK 
 *  Relies on: battery pin variable
 *  Returns: is battery ok (based on average of last 10 values)
 */

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
 * Function: isFloodSensorOK 
 *  Relies on: floodPin variable
 *  Returns: if the flood sensor detected water
 */
bool isFloodSensorOK() {
  #ifdef floodPin
    return digitalRead(floodPin); // ??IS THIS JUST A DIGITAL READ??
  #else
    return true; // No flood sensor to check
  #endif
}

/*
 * Function: areSystemsOK 
 *  Returns: True if all system checks read as OK
 */
bool areSystemsOK(){
  return isFloodSensorOK() && isTempSensorOK() && isBatteryVoltageOK() && isEStopOK();
}

