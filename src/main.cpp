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

#define DEBUG

// Update these with values suitable for your network.
uint8_t mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 56);
IPAddress server;

Relays relays;

void callback(char* topic, byte* payload, uint16_t length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (uint16_t i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Move this to reconnect (in case IP has changed)
    Serial.print(F("MQTT IP lookup:\n - using DNS IP:"));
    Serial.println(Ethernet.dnsServerIP());
    DNSClient dns;
    dns.begin(Ethernet.dnsServerIP());
    if(dns.getHostByName("postman.cloudmqtt.com", server) == 1) {
      Serial.print(F(" - Found MQTT server IP: "));
      Serial.println(server);
    }
    /*else{
      Serial.println(F(": failed."));
      while(1);
    }*/

    Serial.print(F("Attempting MQTT connection... "));
    // Attempt to connect
    // #define MQTT_USER and MQTT_PASSKEY in mqtt_credentials.h
    if (client.connect("gardencontroller", MQTT_USER, MQTT_PASSKEY)){
      Serial.println(F("connected."));
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
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

  // Start ethernet
  Serial.print(F(" - Ethernet "));
  Ethernet.begin(mac);
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

  client.setServer(server, 11921);
  client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);

  Serial.println(" - Relay board:");
  relays.begin();
  relays.printStates();

  Serial.println("Setup complete.\n");
}

void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();
}
