/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: RelayParams.h
 *      Created: 2020-01-25
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the RelayParams class that handles 
 *      modifying of the relay-related paramaters.
 * 
 */


#ifndef __RELAY_PARAMS_H__
#define __RELAY_PARAMS_H__

#include <Arduino.h>

class RelayParams{
    public:
        // TODO: add init from en store to SD card

        typedef enum {RP_OK, RP_ERROR} RP_Status_t;

        RelayParams();

        RP_Status_t setName(char * name);
        RP_Status_t getName(char * name);

        bool getEnabled(void){
            return _enabled;
        }

        bool getOnOff(void){
            return _onOff;
        }

        bool setEnabled(bool enabled);
        bool setOnOff(bool onOff);

    private:
        /* data */
        char * _name;
        bool _enabled;
        bool _onOff;
        uint8_t _buttonPin;
        uint8_t _ledPin;
        uint8_t _relays[2];
};


#endif /*__RELAY_PARAMS_H__*/
