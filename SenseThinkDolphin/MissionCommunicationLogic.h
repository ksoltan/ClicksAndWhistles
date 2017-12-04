void setupMissionCom(){
  XBee.begin(9600);
}

bool downloadMission() {
//  hasMission = false;
<<<<<<< HEAD
  if (Serial.available()){ // Serial port available for communication
    Serial.println("Waiting");
    mission = Serial.readStringUntil('\n'); // Read until a newline
=======
  if (XBee.available()){ // Serial port available for communication
    mission = XBee.readStringUntil('\n'); // Read until a newline
>>>>>>> debug_logic
    lengthMission = mission.length();

    if (lengthMission == 0) {
      XBee.println("Got invalid mission string.");
      return false;
    }

    // Read the mission string, check for invalid characters,
    // 'r' = red, 'y' = yellow, 'w' = white
    for (int i = 0; i < lengthMission; i++) {
      char char_i = mission[i];
      if (char_i != 'r' && char_i != 'y' && char_i != 'w') {
        XBee.println("Got invalid mission string.");
        return false;
      }
    }
    return true;
  }
  else{
    return false;
  }
  
}


