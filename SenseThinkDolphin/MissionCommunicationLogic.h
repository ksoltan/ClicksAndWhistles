void setupMissionCom(){
  Serial.begin(9600);
}

bool downloadMission() {
//  hasMission = false;
  if (Serial.available()){ // Serial port available for communication
    mission = Serial.readStringUntil('\n'); // Read until a newline
    lengthMission = mission.length();

    if (lengthMission == 0) {
      Serial.println("Got invalid mission string.");
      return false;
    }

    // Read the mission string, check for invalid characters,
    // 'r' = red, 'y' = yellow, 'w' = white
    for (int i = 0; i < lengthMission; i++) {
      char char_i = mission[i];
      if (char_i != 'r' && char_i != 'y' && char_i != 'w') {
        Serial.println("Got invalid mission string.");
        return false;
      }
    }
    return true;
  }
  else{
    return false;
  }
  
}


