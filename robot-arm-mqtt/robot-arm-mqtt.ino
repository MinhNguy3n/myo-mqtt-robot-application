/**
Name: robot-arm-mqtt
Author: Minh Nguyen
Description: ESP8266 as MQTT client connected with Mosquitto broker 
and send commands through Software serial communication with Arduino
**/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial SUART(4, 5); //SRX=Dpin-D2; STX-DPin-D1

// Connect to the WiFi
const char* ssid = "Private_lab"; //"LabNet";
const char* password = "M1603072"; //"dftlab1!";
const char* mqtt_server = "192.168.1.109";
const char* user_id = "esp12e-lab";
const char* user = "espnode";
const char* user_pass = "EspHome!";
const int interrupt_pin = 0; // DPin-D3

WiFiClient espWifiClient;
PubSubClient client(espWifiClient);

String payloadMsg = "";

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
  if(payloadMsg == "ROTATE_CW"){
    SUART.print('1');
  }
  else if(payloadMsg == "ROTATE_CCW"){
    SUART.print('2');
  }
  else if(payloadMsg == "RAISE"){
    SUART.print('3');
  }
  else if(payloadMsg == "LOWER"){
    SUART.print('4');
  }
  else if(payloadMsg == "STRETCH"){
    SUART.print('5');
  }
  else if(payloadMsg == "SHORTEN"){
    SUART.print('6');
  }
  else if(payloadMsg == "DOUBLE_TAP"){
    SUART.print('0');
  }
  else if(payloadMsg == "REST"){
    digitalWrite(interrupt_pin, HIGH);
    delay(100);
    digitalWrite(interrupt_pin, LOW);
  }
  else{
    Serial.println("Unrecognized payload!");
  }

  payloadMsg = "";
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), user, user_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/Espnode/status", "ESP node reconnected!");
      // ... and resubscribe
      client.subscribe("/Myo/pose");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second.");
      // Wait 1 second before retrying
      delay(1000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  SUART.begin(115200);
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
  pinMode(interrupt_pin, OUTPUT);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
