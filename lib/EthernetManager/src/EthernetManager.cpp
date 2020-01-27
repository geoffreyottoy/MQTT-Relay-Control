/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: EthernetManager.cpp
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Implementation of the EthernetManager class that offers
 *      functionality to setup and maintain an Ethernet connection.
 *
 */

#include "EthernetManager.h"
#include "SerialDebug.h"

#define ETHCS_CPIN      10 //W5500 CS

EthernetManager::EthernetManager(){

}


int EthernetManager::begin(){
    Serial.print("CS pin: ");
    Serial.println(ETHCS_CPIN);
    Ethernet.setCsPin(ETHCS_CPIN);

    while(!this->resetHW()) delay(100);

    Serial.print("MAC address: ");
    for(uint8_t i=0; i<6; i++){
        if(i) Serial.print('-');
        Serial.print(macAddress[i], HEX);
    }
    Serial.println();
    
    // Start ethernet operation
    uint8_t retries = 5;
#ifdef LOCAL_IP
    IPAddress localIP(LOCAL_IP);
    while((Ethernet.begin(macAddress, localIP) == 0) && retries--){
#else
    while((Ethernet.begin(macAddress) == 0) && retries--){
#endif
        Serial.println("Init failed. Trying reset.");
        this->resetHW();
    };

    // Check connection and retry if necessary
    this->reconnect();

    return 0;
}

bool EthernetManager::isConnected(void){
    return (Ethernet.linkReport()[0] == 'L');
}

int EthernetManager::resetHW(void){
    Serial.print("Soft reset ");
    if(Ethernet.softreset()){
         Serial.println("OK.");
    }
    else{
         Serial.println("FAILED.");
         return 0;
    }
    return 1;
}

void EthernetManager::reconnect(bool reconnect){
    if(!this->isConnected()){
        if(reconnect){
            this->resetHW();
#ifdef LOCAL_IP
            IPAddress localIP(LOCAL_IP);
            Ethernet.begin(macAddress, localIP);
#else
            Ethernet.begin(macAddress);
#endif
        }
        else{
            Serial.println(F("FAILURE: link error."));
        }
    }
    Serial.print(F("Local IP: "));
    Serial.println(Ethernet.localIP());
}

IPAddress EthernetManager::localIP(void){
    return Ethernet.localIP();
}
