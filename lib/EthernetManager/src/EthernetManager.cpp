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
#include "settings.h"
#include "SerialDebug.h"

EthernetManager::EthernetManager(){

}


void EthernetManager::begin(){
    uint8_t macAddress[] = MAC_ADDRESS;

    // Start ethernet operation
#ifdef FIXED_IP
    IPAddress localIP(FIXED_IP);
    Ethernet.begin(macAddress, localIP);
#else
    Ethernet.begin(macAddress);
#endif

    // Check connection and retry if necessary
    this->reconnect();
}

bool EthernetManager::isConnected(void){
    return (Ethernet.linkStatus() == LinkON);
}

void EthernetManager::reconnect(bool reconnect){
    // reconnect = false -> first time setup 
    if(Ethernet.hardwareStatus() == EthernetNoHardware){
        DEBUG.println(F("fail: no hardware found."));
        return;
    }
    if(Ethernet.linkStatus() != LinkON){
        DEBUG.println(F("fail: link error."));
        return;
    }
    DEBUG.print(F("local ip: "));
    DEBUG.println(Ethernet.localIP());
}

IPAddress EthernetManager::localIP(void){
    return Ethernet.localIP();
}
