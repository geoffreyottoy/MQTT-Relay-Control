/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: EthernetManager.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the EthernetManager class that offers
 *      functionality to setup and maintain an Ethernet connection.
 *
 */

#ifndef __ETHERNET_MANAGER_H__
#define __ETHERNET_MANAGER_H__

#include <Arduino.h>
#include <Ethernet3.h>
#include "settings.h"

/* The EthernetManager class maintains connection to an Ethernet Network.
 */
class EthernetManager{
    public:
        EthernetManager(void);
        int begin(void);
        void reconnect(bool reconnect=false);
        bool isConnected(void);

        IPAddress localIP();

    protected:
        
    private:
        byte macAddress[6] = MAC_ADDRESS;

        int resetHW(void);
};

#endif /*__ETHERNET_MANAGER_H__*/
