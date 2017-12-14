void setupMissionCom(){
  XBee.begin(9600);
}

bool downloadMission() {
  if (XBee.available()){ // Serial port available for communication
    mission = XBee.readStringUntil('\n'); // Read until a newline
    lengthMission = mission.length();

    if (lengthMission == 0) {
      XBee.println("Got invalid mission string.");
      return false;
    }

    // Read the mission string, check for invalid characters,
    // 'r' = red, 'y' = yellow, 'w' = white
    for (int i = 0; i < lengthMission; i++) {
      char char_i = mission[i];
//      XBee.println(char_i);
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

void communicateWithXBee(){
  if (XBee.available()){ // Serial port available for communication
    String command = XBee.readStringUntil('\n'); // Read until a newline
    int lengthCommand = command.length();
    
    if (lengthCommand == 4 && command[0] == 't') { 
      // User command to turn robot manually and progress to next step.

      XBee.println("Turning, and updating to next mission step.");
      dolphinState = VICTORY;
    }
}
}

