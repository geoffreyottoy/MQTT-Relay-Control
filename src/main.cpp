/* Ideas:
 *  - add SD logging
 *  - logging with NTP timestamps
 *  - basic web interface
 *  - watchdog reset
 */

// web ui: http://www.steves-internet-guide.com/using-javascript-mqtt-client-websockets/
// en nog meer dit: https://www.cloudmqtt.com/docs/websocket.html

/* Generic MQTT command format (arduino subscribes to)
 *   >>  set REL<id> <HIGH|LOW>
 * examples:
 *   >>  0-8-00001111
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Dns.h>
#include <PubSubClient.h>

#include "mqtt_settings.h"
#include "relays.h"

// Uncomment if you don't want to wait for a serial terminal connection to
// start program execution
#define DEBUG
// Uncomment and change to your preferred fixed IP address if you want your
// device to have a IP address
#define FIXED_IP { 192, 168, 0, 56 }
// Maximum length of an MQTT message
#define MAX_MESSAGE_LEN 64

// Ethernet MAC address
//  -> generated with https://www.miniwebtool.com/mac-address-generator/
uint8_t macAddress[] = {  0x97, 0x69, 0xCB, 0xF9, 0xEA, 0x17 };
// Don't change here
#ifdef FIXED_IP
IPAddress localIP(FIXED_IP);
#endif
IPAddress mqttServerIP;

Relays relays;
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

bool cmd_received = false;
char cmd_message[MAX_MESSAGE_LEN];

// Callback method (subscribed topic message received)
void mqttSubCallback(char* topic, byte* payload, uint16_t length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // Copy to buffer
  memset(cmd_message, '\0', MAX_MESSAGE_LEN);
  for(unsigned int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    // avoid buffer overflow
    if(i<MAX_MESSAGE_LEN){
      cmd_message[i] = (char)payload[i];
    }
  }
  Serial.println();
  // set flag
  cmd_received = true;
}

void mqttReconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    // Move this to reconnect (in case IP has changed)
    Serial.print(F("MQTT IP lookup:\n - using DNS IP:"));
    Serial.println(Ethernet.dnsServerIP());
    DNSClient dns;
    dns.begin(Ethernet.dnsServerIP());
    if(dns.getHostByName("postman.cloudmqtt.com", mqttServerIP) == 1) {
      Serial.print(F(" - Found MQTT server IP: "));
      Serial.println(mqttServerIP);
    }
    /*else{
      Serial.println(F(": failed."));
      while(1);
    }*/

    Serial.print(F("Attempting MQTT connection... "));
    // Attempt to connect
    // #define MQTT_USER and MQTT_PASSKEY in mqtt_settings.h
    if (mqttClient.connect("gardencontroller", MQTT_USER, MQTT_PASSKEY)){
      Serial.println(F("connected."));
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqttClient.state());
      Serial.println(F("retrying in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* Setup for main operation.
 */
void setup(){
  // Start serial connection for debug output
  Serial.begin(115200);
#ifdef DEBUG // no need to wait if we're not debugging
  while(!Serial){}
#endif
  Serial.println(F("Setup:"));

  // Start Ethernet hardware
  Serial.print(F(" - Ethernet "));
#ifdef FIXED_IP
  Ethernet.begin(macAddress, localIP);
#else
  Ethernet.begin(macAddress);
#endif
  if(Ethernet.hardwareStatus() == EthernetNoHardware){
    Serial.println(F("fail: no hardware found."));
    while(1); // to watchdog or not to watchdog?
  }
  if(Ethernet.linkStatus() != LinkON){
    Serial.println(F("fail: link error."));
    while(1); // same here
  }
  Serial.print(F("local ip: "));
  Serial.println(Ethernet.localIP());

  // Basic MQTT config
  mqttClient.setServer(mqttServerIP, MQTT_PORT);
  mqttClient.setCallback(mqttSubCallback);

  // Allow the hardware to sort itself out
  delay(1500);

  Serial.println(" - Relay board:");
  relays.begin();
  relays.printStates();

  Serial.println("Setup complete.\n");
}


/* Main loop.
 */
void loop(){
  // Keep the connection with the MQTT broker
  if(!mqttClient.connected()){
    mqttReconnect();
  }
  // Run the necessary MQTT operations
  mqttClient.loop();

  // Process any command that has been received
  if(cmd_received){
    cmd_received = false;


  }
}
