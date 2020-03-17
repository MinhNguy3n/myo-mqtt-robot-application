/**
Name: robot-arm-control-with-serial-inputs
Author: Minh Nguyen
Description: ESP8266 uses MQTT communication to control robot arm's servo
Except from hand servo, the others are controlled with step size of 15 deg
**/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Connect to the WiFi
const char* ssid = "vittu_sinua";
const char* password = "3YdtFRBEhi";
const char* mqtt_server = "192.168.1.109";
const char* user_id = "esp12elab";
const char* user = "espnode";
const char* user_pass = "EspHome!";

WiFiClient espClient;
PubSubClient client(espClient);

// create servo pin layout
const byte servo1 = 2;
const byte servo2 = 0;
const byte servo3 = 5;
const byte servo4 = 4;
 // create servo object to control arm
Servo servo_1;  
Servo servo_2;
Servo servo_3;
Servo servo_4;

// servo motors records
byte hand_state = 0; //0 = closed, 1 = opened
unsigned int hand_pos;
unsigned int arm_angle;
unsigned int arm_altitude;
unsigned int arm_reach; 

// set step size to 5
const unsigned step = 15;
// Extreme values
const unsigned max_serv_1 = 70;
const unsigned min_serv_1 = 10;
const unsigned max_serv_2 = 180;
const unsigned min_serv_2 = 0;
const unsigned max_serv_3 = 160;
const unsigned min_serv_3 = 80;
const unsigned max_serv_4 = 85;
const unsigned min_serv_4 = 15;
String payloadMsg = "";
unsigned prev_pos;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  /** read payload message **/
  for(int i = 0; i < length; i++){
    if(payload[i] != 0 && payload[i] != ' '){
      payloadMsg.concat((char)payload[i]);
    }
  }

  Serial.println(payloadMsg);
  /** Handling the messages **/
  // hand servo
  if(payloadMsg == "DOUBLE_TAP"){
    if(hand_state == 0){
      for(hand_pos = servo_1.read(); hand_pos > min_serv_1; hand_pos--)
      {
        servo_1.write(hand_pos);
        delay(20);
      }
      hand_state = 1;
    }
    else if(hand_state == 1){
      for(hand_pos = servo_1.read(); hand_pos < max_serv_1; hand_pos++)
      {
        servo_1.write(hand_pos);
        delay(20);
      }
      hand_state = 0;
    }
  }
  // bottom servo
  // CCW rotation
  else if(payloadMsg == "ROTATE_CCW"){
    prev_pos = servo_2.read();
    for(arm_angle = servo_2.read(); arm_angle < prev_pos+step && arm_angle < max_serv_2 ; arm_angle++)
    {
      servo_2.write(arm_angle);
      delay(20);
    }
  }
  // CW rotation
  else if(payloadMsg == "ROTATE_CW"){
    prev_pos = servo_2.read();
    for(arm_angle = servo_2.read(); arm_angle > prev_pos-step && arm_angle > min_serv_2; arm_angle--)
    {
      servo_2.write(arm_angle);
      delay(20);
    }
  }
  /* left servo */
  // Raising the arm
  else if(payloadMsg == "RAISE"){
    prev_pos = servo_3.read();
    for(arm_altitude = servo_3.read(); arm_altitude < prev_pos+step && arm_altitude < max_serv_3; arm_altitude++)
    {
      servo_3.write(arm_altitude);
      delay(20);
    }
  }
  // Lowering the arm
  else if(payloadMsg == "LOWER"){
    prev_pos = servo_3.read(); 
    for(arm_altitude = servo_3.read(); arm_altitude > prev_pos-step && arm_altitude > min_serv_3; arm_altitude--)
    {
      servo_3.write(arm_altitude);
      delay(20);
    }
  }

  /* Right servo */
  // extends the arm
  else if(payloadMsg == "STRETCH"){   
    prev_pos = servo_4.read();
    for(arm_reach = servo_4.read(); arm_reach > prev_pos-step && arm_reach > min_serv_4; arm_reach--)
    {
      servo_4.write(arm_reach);
      delay(20);
    }
  }
  // shorten the arm
  else if(payloadMsg == "SHORTEN"){
    prev_pos = servo_4.read();
    for(arm_reach = servo_4.read(); arm_reach < prev_pos+step && arm_reach < max_serv_4; arm_reach++)
    {
      servo_4.write(arm_reach);
      delay(20);
    }
  }
  else{
    Serial.println("Unregcognised payload");
  }
  payloadMsg = "";
}
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 
 // Attempt to connect to MQTT broker
 if (client.connect(user_id, user, user_pass)) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("/Myo/pose",1);
 } 
 else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
  Serial.begin(9600);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  client.setServer(mqtt_server, 1883); //establish connection with mosquitto broker of GW
  client.setCallback(callback);
  // Attach servo to gpios of esp8266
  servo_1.attach(servo1); 
  servo_2.attach(servo2); 
  servo_3.attach(servo3); 
  servo_4.attach(servo4); 

  // reset arm position
  arm_angle = 90;
  arm_altitude = 130;
  arm_reach = 50;
  
  servo_1.write(70);
  servo_2.write(arm_angle);
  servo_3.write(arm_altitude);
  servo_4.write(arm_reach);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 else{
  client.loop();
 }
}
