#include <Arduino.h>
/*
 Basic MQTT example
 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

*/
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Dns.h>
#include <PubSubClient.h>

#include "mqtt_credentials.h"
#include "relays.h"

// uncomment if you don't want to wait for a serial terminal connection to
// start program execution
#define DEBUG
// uncomment and change to your preferred fixed IP address if you want your
// device to have a IP address
#define FIXED_IP { 192, 168, 0, 56 }

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

void callback(char* topic, byte* payload, uint16_t length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (uint16_t i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
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
    // #define MQTT_USER and MQTT_PASSKEY in mqtt_credentials.h
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
    while(1);
  }
  if(Ethernet.linkStatus() != LinkON){
    Serial.println(F("fail: link error."));
    while(1);
  }
  Serial.print(F("local ip: "));
  Serial.println(Ethernet.localIP());

  //
  mqttClient.setServer(mqttServerIP, 11921);
  mqttClient.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);

  Serial.println(" - Relay board:");
  relays.begin();
  relays.printStates();

  Serial.println("Setup complete.\n");
}

void loop(){
  if(!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();
}
