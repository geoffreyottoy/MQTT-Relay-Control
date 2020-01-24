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

#include "settings.h"
#include "SerialDebug.h"
#include "MQTT.h"
#include "EthernetManager.h"
#include "RelayControl.h"

// Maximum length of an MQTT message
#define MAX_MESSAGE_LEN 64

EthernetManager net;
MQTT mqtt(net);

Relays relays;

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

  DEBUG.println(F("Setup:"));
  
  // Start Ethernet hardware
  DEBUG.print(F(" - Ethernet "));
  net.begin();

  // Allow the hardware to sort itself out
  delay(1500);

  DEBUG.println(" - Relay board:");
  relays.begin();
  relays.printStates();

  // MQTT
  mqtt.setSubCallback(mqttSubCallback);
  mqtt.reconnect();

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

  }
}
