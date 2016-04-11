#include <Servo.h>

#include <stdio.h>
// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

int LIGHT_LEVEL = 140;
int LIGHT_DELTA[4];
int currentState[4];
int average[4];
int ledval[4];
Servo servos[4];
int SERVO_MIN[4];
int SERVO_MAX[4];

void setup() {
  Serial.begin(9600);
  
  //Setup Servo
  servos[0].attach(3);
  servos[1].attach(5);
  servos[2].attach(6);
  servos[3].attach(9);

  average[0] = 130;
  average[1] = 130;
  average[2] = 280;
  average[3] = 130;
  
  SERVO_MIN[0] = 140;
  SERVO_MAX[0] = 70;
  SERVO_MIN[1] = 160;
  SERVO_MAX[1] = 80;
  SERVO_MIN[2] = 160;
  SERVO_MAX[2] = 80;
  SERVO_MIN[3] = 161;
  SERVO_MAX[3] = 80;
  
  LIGHT_DELTA[0] = 5;
  LIGHT_DELTA[1] = 3;
  LIGHT_DELTA[2] = 5;
  LIGHT_DELTA[3] = 6;
}

void turnOnSensorAndSendValue(int sensor) {
  int val = analogRead(sensor);
//  Serial.println(val);
 
 ledval[sensor] = val;
//  val = constrain(val, 0, 92);
//  
//  ledval[sensor] = map(val, 0, 92, 0, 255);
  
  // Calculate delta to previous weighted average.
  int delta = ledval[sensor] - average[sensor];
  
  // Update weighted average.
  average[sensor] = ((average[sensor] * 3) + ledval[sensor]) / 4;
//  
//    Serial.print("average[sensor]: ");
//    Serial.println(average[sensor]);
   if(delta * delta > 9){
//    Serial.print("delta: ");
//    Serial.println(delta);
   }

  // CurrentState refers to light, has it been hit or not hit?
  if ((!currentState[sensor] && delta > LIGHT_DELTA[sensor]) || (currentState[sensor] && delta < -LIGHT_DELTA[sensor])) {      
//    Serial.print("average[sensor]: ");
//    Serial.println(average[sensor]);
//    Serial.print("delta: ");
//    Serial.println(delta);
    currentState[sensor] = !currentState[sensor];
    Serial.print(sensor);
    Serial.print(currentState[sensor]);
  }
}

void listenForServoInstructions() {
  int servoNumber = 0;
  int servoState = 0;
  // send data only when you receive data:
  if (Serial.available() > 0) {
    
    // read the incoming byte:
    servoNumber = Serial.read();
    servoNumber -= '0';
    
    // Blocking, waiting on the next input
    while (Serial.available() == 0) {
    }
    

    servoState = Serial.read();
    servoState -= '0';
//        Serial.print("weve recieved servo info ");
//        Serial.print(servoNumber);
//        Serial.print(" ");
//        Serial.println(servoState);
    moveServo(servoNumber, servoState);
    }
}

void moveServo(int servoNumber, int servoState) {
   if (servoState == 1) {
//     Serial.print("servo state 1");
      servos[servoNumber].write(SERVO_MAX[servoNumber]);              // tell servo to go to position in variable 'pos' 
   } else {
//          Serial.print("servo state 0");
     servos[servoNumber].write(SERVO_MIN[servoNumber]);
   }
}

void loop() {
  listenForServoInstructions();
  // Reset
  for(int lightSensor = 0; lightSensor < 4; lightSensor += 1)  {
   turnOnSensorAndSendValue(lightSensor);
  } 
//turnOnSensorAndSendValue(0);
  delay(10);
}
