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

  #ifdef estop_pin
    pinMode(estop_pin, OUTPUT);
  #endif
}

// Press the E-stop in code. Writing 0 to the relay will stop passing current through it to the servos (make it open).
void freeze_motors(){
  digitalWrite(estop_pin, LOW);
}

// Release the E-stop in code. Allow servos to get power by closing the relay.
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
 * Function: areSystemsOK 
 *  Returns: True if all system checks read as OK
 */
bool areSystemsOK(){
  return isBatteryVoltageOK() && isEStopOK();
}

