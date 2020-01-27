/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: NVConfig.cpp
 *      Created: 2020-01-25
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the NVConfig class that handles 
 *      storing and retrieving configurations from non volatile 
 *      storage; in this case an SD card.
 * 
 */


#ifndef __NV_CONFIG__
#define __NV_CONFIG__

#include <Arduino.h>
#include <SdFat.h>

class NVConfig{
    private:
        SdFile * _configFile;

    public:
        NVConfig(/* args */);
        int openConfig();
        int getNrOfCircuits();
            
        int getCircuitName(int circuit, char * name);
        int getCircuitButtonPin(int circuit, int * pin);
        int getCircuitLedPin(int circuit, int * pin);

        int setCircuitName(int circuit, char * name);
};



#endif /*__NV_CONFIG__*/
