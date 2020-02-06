/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: MQTT.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the MQTT class that offers
 *      functionality to connect the an MQTT broker (via Ethernet)
 *      and publish on and subscribe to topics.
 *
 */

#ifndef __MQTT_H__
#define __MQTT_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include "EthernetManager.h"

typedef void (&MqttSubCallback) (char*, uint8_t * , unsigned int);

/* The MQTT class offers functionality to connect to and communicate
 * with an MQTT broker.
 */
class MQTT{
public:
    MQTT(EthernetManager &wifi);

    void setRetained(const char topic[], const char message[]);
    void setSubTopic(const char topic[]);
    void setSubCallback(MqttSubCallback cb);
    void reconnect(void);
    bool isConnected(void);
    void publish(const char topic[], const char message[]);
    void publishRetained(const char topic[], const char message[]);

    void loop(void);
    
protected:
    

private:
    char * subTopic;
    char * retTopic;
    char * retMessage;

    EthernetManager * ethManager;
    EthernetClient * ethClient;
    PubSubClient * mqttClient;
};

#endif /* __MQTT_H__ */
