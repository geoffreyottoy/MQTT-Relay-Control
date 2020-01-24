/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: MQTT.cpp
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Implementation of the MQTT class that offers
 *      functionality to connect the an MQTT broker (via Ethernet)
 *      and publish on and subscribe to topics.
 *
 */

#include <Dns.h>
#include "MQTT.h"
#include "SerialDebug.h"
#include "DNSresolver.h"

MQTT::MQTT(EthernetManager &wifi){
    this->ethManager = &wifi;
    // Create wifi client as basis for the mqttClient
    this->ethClient = new EthernetClient();
    // Create mqtt client to talk with the broker
    this->mqttClient = new PubSubClient(*(this->ethClient));

    // Basic MQTT config
    //this->mqttClient->setServer(mqttServerIP, MQTT_PORT);

    this->subTopic = NULL;
    this->retTopic = NULL;
    this->retMessage = NULL;
}

void MQTT::setSubCallback(MqttSubCallback cb){
    this->mqttClient->setCallback(cb);
}

void MQTT::setSubTopic(const char topic[]){
    this->subTopic = (char *)malloc(strlen(topic) + 1);
    strcpy(this->subTopic, topic);
    DEBUG.print("Sub. Topic: ");
    DEBUG.println(this->subTopic);
}

// retained message containing sensorlist
void MQTT::setRetained(const char topic[], const char message[]){
    this->retTopic = (char *)malloc(strlen(topic) + 1);
    strcpy(this->retTopic, topic);
    DEBUG.print("Ret. Topic: ");
    DEBUG.println(this->retTopic);
    this->retMessage = (char *)malloc(strlen(message) + 1);
    strcpy(this->retMessage, message);
    DEBUG.print("Ret. Msg: ");
    DEBUG.println(this->retMessage);
}

void MQTT::publish(const char topic[], const char message[]){
    this->mqttClient->publish(topic, message);
}

void MQTT::loop(void){
    this->mqttClient->loop();
}

/* Attemt to reconnect to the mqtt server
 */
void MQTT::reconnect(void){
    // blocking loop to reconnect with wifi network
    while(!this->ethManager->isConnected()){
        this->ethManager->reconnect(true);
    }

    // Loop until we're reconnected
    // This is a blocking loop
    // Might want to change dat later
    while (!this->mqttClient->connected()){
        // Look-up MQTT broker IP
        DEBUG.print(F("Looking up broker ip for "));
        DEBUG.println(MQTT_BROKER);
        DEBUG.print(F("Using DNS IP:"));
        DEBUG.println(Ethernet.dnsServerIP());
        // Create DNSClient
        //DNSClient dns;
        DNSresolver dns(Ethernet.dnsServerIP());
        // Start client
        //IPAddress dnsIP = {8, 8, 8, 8};
        //dns.begin(Ethernet.dnsServerIP());
        //dns.begin(dnsIP);
        // Look-up broker IP
        if(dns.getHostByName(MQTT_BROKER, this->brokerIP) != 1) {
            DEBUG.println("DNS lookup failed!");
            return;
        }
        // Check if valid IP
        if(this->brokerIP != INADDR_NONE){
            DEBUG.print("MQTT server found at: ");
            DEBUG.println(brokerIP);
        }
        else{
            DEBUG.println("DNS lookup failed!");
            return; 
        }

        this->mqttClient->setServer(this->brokerIP, MQTT_PORT);

        DEBUG.print("Attempting MQTT connection... ");
        // Create a random client ID
        String clientId = "gardencontroller";
        // Attempt to connect
        if (this->mqttClient->connect(clientId.c_str(), MQTT_USER, MQTT_PASSKEY)){
            DEBUG.println("connected");
            // Once connected, publish an announcement...
            if((this->retMessage) != NULL && (this->retTopic != NULL)){
                this->mqttClient->publish(this->retTopic, this->retMessage, true);
            }
            // ... and (re)subscribe
            if(this->subTopic != NULL){
                this->mqttClient->subscribe(this->subTopic);
            }
        }
        else{
            DEBUG.print("failed, rc=");
            DEBUG.print(this->mqttClient->state());
            DEBUG.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

bool MQTT::isConnected(void){
    return mqttClient->connected();
}