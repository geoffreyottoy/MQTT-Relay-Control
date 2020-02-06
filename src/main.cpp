/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: main.cpp
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: 
 *
 */

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
#include <SdFat.h>

#include "settings.h"
#include "SerialDebug.h"
#include "MQTT.h"
#include "EthernetManager.h"
#include "RelayControl.h"

// Maximum length of an MQTT message
#define MAX_MESSAGE_LEN 64

#define SD_CS_PIN       4

SdFat sd;
EthernetManager net;
MQTT mqtt(net);

RelayControl rc;

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

/* Setup for main operation.
 */
void setup(){
  DEBUG.begin();
  delay(5000);

  // DEBUG.begin();
  Serial.println(F("Running setup...\n"));

  // Start Ethernet hardware
  Serial.println(F("1. Ethernet"));
  net.begin();
  Serial.println();

  // Allow the hardware to sort itself out
  delay(1500);

  Serial.println(" - Relay board:");
  rc.begin();

  // MQTT
  Serial.println(F("2. MQTT"));
  mqtt.setSubCallback((MqttSubCallback) mqttSubCallback);
  mqtt.setSubTopic("circuits/update");
  mqtt.reconnect();
  Serial.println();

  DEBUG.println("Setup complete.\n");
}


/* Main loop.
 */
void loop(){
  // Keep the connection with the MQTT broker
  mqtt.reconnect();
  
  // Run the necessary MQTT operations
  mqtt.loop();

  // Process any command that has been received
  if(cmd_received){
    cmd_received = false;
    rc.updateStatus(cmd_message);
  }

  rc.loop();

  // send MQTT status update
  if(mqtt.isConnected()){
    char statusStr[17];
    memset(statusStr,0,17);
    if(rc.getStatus(statusStr)){
      Serial.println("new status");
      Serial.println(statusStr);

      // TODO: new retained message
      mqtt.publishRetained("circuits/status", statusStr);
    }
  }
}
