#define yawPin 5
#define tailPin 6
#include <Servo.h>
Servo yawServo;
Servo tailServo;

// Servo Movement Timing Variables
long unsigned lastMoveTimeTail = 0; // Keeps track of the last time we calculated tail positions. 0 indicates it's never been initialized.
long unsigned lastMoveTimeYaw = 0;
long unsigned moveTimer = 0; // Keeps track of the tail's phase. Always counts up, but speed can change.
int moveDelayTime = 50; // Every couple of ms recalculate tail/yaw positioning.
// Must make sure that the time delay is smaller than the time step of moving the servo at a constant period
int fastFlapPeriod = 1 * 1000; // 1 full cycle in x milliseconds
int slowFlapPeriod = 2 * 1000; // 1 full cycle in x milliseconds
int slowerFlapPeriod = 4 * 1000;

// Servo Positions: ??Need calibration?? Look into making sure that the servoLeft/Right are not negative, since transmitting unsigned bytes.
int yawServoLeft = 10; // degrees
int yawServoRight = 150; // degrees
int tailServoLeft = 0; // degrees
int tailServoRight = 180; // degrees
int servoAngleChange = 5; // Smallest value by which to increment servo position
int yawServoPos = (yawServoRight + yawServoLeft) / 2; // Initialize to the midpoint.
int tailServoPos = (tailServoRight + tailServoLeft) / 2; // Initialize to the midpoint.
int tailDir = 1; // If 1: yaw servo is moving to the right (incrementing). If -1: yaw servo is moving to the left (decrementing)
int yawDir = 1;
// Moving the tail position at a constant frequency
void updateTailPosition(int period){
  int numStepsInCycle = 2 * abs(tailServoRight - tailServoLeft) / servoAngleChange; // Number of steps to complete full period with servoAngleChange update
  int timeToMove = period / numStepsInCycle; // Need to move servo every x milliseconds to achieve this frequency
  
  if(millis() - lastMoveTimeTail >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(tailServoPos + tailDir * servoAngleChange > tailServoRight || tailServoPos + tailDir * servoAngleChange < tailServoLeft){
      // Switch direction
      tailDir *= -1;
    }
    tailServoPos += tailDir * servoAngleChange; // Update tail servo pos
    lastMoveTimeTail = millis();
  }
}

// Moving the tail position at a constant frequency
void updateYawPosition(int period){
  int numStepsInCycle = 2 * abs(yawServoRight - yawServoLeft) / servoAngleChange; // Number of steps to complete full period with servoAngleChange update
  int timeToMove = period / numStepsInCycle; // Need to move servo every x milliseconds to achieve this frequency
  
  if(millis() - lastMoveTimeYaw >= timeToMove){ // Update servo direction only if correct amount of time has passed
    // Check to see if the servo will move out of bounds with current direction, either left (too much decrement) or right (too much increment)
    if(yawServoPos + yawDir * servoAngleChange > yawServoRight || yawServoPos + yawDir * servoAngleChange < yawServoLeft){
      // Switch direction
      yawDir *= -1;
    }
    yawServoPos += yawDir * servoAngleChange; // Update tail servo pos
    lastMoveTimeYaw = millis();
  }
}


void setup(){
  Serial.begin(9600);
  yawServo.attach(yawPin);
  tailServo.attach(tailPin);
  tailServo.write(90);
}

void loop(){
  if(millis() - lastMoveTimeTail >= moveDelayTime){
    updateTailPosition(slowFlapPeriod);
    tailServo.write(tailServoPos);
    lastMoveTimeTail = millis(); // Update timer
    Serial.println(tailServoPos);
  }

  if(millis() - lastMoveTimeYaw >= moveDelayTime){
    updateYawPosition(slowFlapPeriod);
    yawServo.write(yawServoPos);
    lastMoveTimeYaw = millis(); // Update timer
    Serial.println(yawServoPos);
  }
}

