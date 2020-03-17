/**
Name: robot-arm-control-with-serial-inputs
Author: Minh Nguyen
Description: Arduino uses software serial communication and interrupt handler 
to control with servo
**/

#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial SUART( 3, 4); //SRX  = DPin-3; STX = DPin-4
// payload message received flag and payload content
/*
0 = DOUBLE_TAP, 1 = ROTATE_CW, 2 = ROTATE_CCW, 3 = RAISE, 4 = LOWER, 5 = STRETCH, 6 = SHORTEN 
*/
byte command = 255; 

// servo pin layout
const byte servo1 = 5;
const byte servo2 = 6;
const byte servo3 = 9;
const byte servo4 = 10;

 // create servo object to control arm
Servo servo_1;  
Servo servo_2;
Servo servo_3;
Servo servo_4;

// servo motors records
int hand_angle; //0 = closed, 70 = opened
int arm_angle;
int arm_altitude;
int arm_reach; 

// Extreme values
const unsigned max_serv_1 = 70;
const unsigned min_serv_1 = 10;
const unsigned max_serv_2 = 180;
const unsigned min_serv_2 = 0;
const unsigned max_serv_3 = 160;
const unsigned min_serv_3 = 80;
const unsigned max_serv_4 = 85;
const unsigned min_serv_4 = 15;

const byte interrupt_pin = 2;
bool rest = false;

void restISR(){
  rest = true;
  Serial.println("Arm stopped!");
}

void setup()
{
  Serial.begin(9600);
  SUART.begin(115200);   //to match with NodeMCU which prefers higher Bd to work
  // Attach servo to gpios of esp8266
  servo_1.attach(servo1); 
  servo_2.attach(servo2); 
  servo_3.attach(servo3); 
  servo_4.attach(servo4); 

  // reset arm position
  hand_angle = 70; 
  arm_angle = 90;
  arm_altitude = 140;
  arm_reach = 50;
  
  setServoPos(servo_1, hand_angle, 40);
  setServoPos(servo_2, arm_angle, 40);
  setServoPos(servo_3, arm_altitude, 40);
  setServoPos(servo_4, arm_reach, 40);

  //attach arm rest interrupt 
  pinMode(interrupt_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), restISR, RISING);
}

void setServoPos(Servo servoX,unsigned int targetPosition, int speedX){
  while(servoX.read() != targetPosition && !rest){
    if (servoX.read() < targetPosition){
      servoX.write(servoX.read()+1);
      delay(speedX);
    }
    else if (servoX.read() > targetPosition){
      servoX.write(servoX.read()-1);
      delay(speedX);
    }
    //Serial.println(servoX.read());
  }
}
 
void loop()
{
  if (SUART.available() != 0){
     /** Handling the command messages **/
    command = SUART.read();
    Serial.print("Command received: ");
    Serial.println((char)command);
    // hand servo
    if(command == '0'){
      if(hand_angle >= 35){ // open hand 
        while(hand_angle > min_serv_1 ){
          hand_angle = servo_1.read();
          servo_1.write(hand_angle-1);
          delay(20);
        }
      }
      else{ // close hands
        while(hand_angle < max_serv_1 ){
          hand_angle = servo_1.read();
          servo_1.write(hand_angle+1);
          delay(20);
        }
      }
    }
    // bottom servo
    // CCW rotation
    else if(command == '1'){
      while(arm_angle < max_serv_2 && !rest){
        arm_angle = servo_2.read();
        servo_2.write(arm_angle+1);
        delay(40);
      }
    }
    // CW rotation
    if(command == '2'){
      while(arm_angle > min_serv_2 && !rest){
        arm_angle = servo_2.read();
        servo_2.write(arm_angle-1);
        delay(40);
      }
    }
    /* left servo */
    // Raising the arm
    else if(command == '3'){
      while(arm_altitude < max_serv_3 && !rest){
        arm_altitude = servo_3.read();
        servo_3.write(arm_altitude+1);
        delay(40);
      } 
    }
    // Lowering the arm
    else if(command == '4'){
      while(arm_altitude > min_serv_3 && !rest){
        arm_altitude = servo_3.read();
        servo_3.write(arm_altitude-1);
        delay(40);
      }
    }
  
    /* Right servo */
    // extends the arm
    else if(command == '5'){   
      while(arm_reach > min_serv_4 && !rest){
        arm_reach = servo_4.read();
        servo_4.write(arm_reach-1);
        delay(40);
      }
    }
    // shorten the arm
    else if(command == '6'){
      while(arm_reach < max_serv_4 && !rest){
        arm_reach = servo_4.read();
        servo_4.write(arm_reach+1);
        delay(40);
      }
    }
    else{
      Serial.println("Unregcognised command");
    }
    
    Serial.println();
  }
  rest = false;
}
