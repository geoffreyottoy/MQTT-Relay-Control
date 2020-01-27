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
 *  Description: Implementation of the NVConfig class that handles 
 *      storing and retrieving configurations from non volatile 
 *      storage; in this case an SD card.
 * 
 */

/* config.txt format
 *
 * # Number of circuits -> C:<number>
 * C:2
 * # Configuration for first cicuit
 * # Name -> N:<name>
 * N:Circuit1
 * # Button pin -> P:<pin> (interrupt pin!)
 * B:1
 * # Indicator LED pin -> L:<pin>
 * L:2
 * # Relays controlled -> R:<number>,{<relaynr>,...}
 * R:1,1
 * N:Circuit2
 * B:3
 * L:4
 * R:2,8,7
 */

#include "NVConfig.h"


NVConfig::NVConfig(/* args */){
}

int NVConfig::openConfig(){
    this->_configFile = new SdFile("config.txt", O_RDWR);

    if(!this->_configFile->isOpen()) {
    //error("demoFgets");
        return -1;
    }

    return 0;
}

int NVConfig::getNrOfCircuits(void){
    char line[32];
    uint8_t n;

    // check for open error
    if (!this->_configFile->isOpen()) {
    //error("demoFgets");
        return -1;
    }

    this->_configFile->rewind();

    while((n = this->_configFile->fgets(line, sizeof(line))) > 0) {
        if (line[n - 1] != '\n') {
            // error
            return -2;
        }
        else {
            if(line[0] == 'C' && line[1] == ':'){
                return strtol(line+2, NULL, 10);
            }
        }
    }
}
