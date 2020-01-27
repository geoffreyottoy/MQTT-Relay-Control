/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: RelayControl.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the RelayControl class that offers
 *      an easy-to-use API to control the relays on the relay_board 
 *      (take caution, this is hardware-specific!
 *        -> see https://github.com/geoffreyottoy/MQTT-Relay-Control/blob/master/doc ).
 *
 */


#ifndef __RELAY_CONTROL_H__
#define __RELAY_CONTROL_H__

#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "RelayParams.h"

#define DEFAULT_ADDRESS   0
#define NUM_RELAYS        8

enum relay_names{
  REL1=1,
  REL2=2,
  REL3=3,
  REL4=4,
  REL5=5,
  REL6=6,
  REL7=7,
  REL8=8
};

typedef struct relay{
  uint8_t name;
  uint8_t pin;
  uint8_t state;
} Relay_t;

class RelayControl{
  public:
    RelayControl(uint8_t address=DEFAULT_ADDRESS, uint8_t nrCircuits=4 /*, NVConfig config*/);
    void begin(void);
    void set(uint8_t * relay, uint8_t * state, uint8_t count);
    uint8_t get(uint8_t relay);
    uint8_t toggle(uint8_t * relay);

    void printStates(void);

  private:
    Adafruit_MCP23017 * _mcp;
    uint8_t _address;

    RelayParams * _params;

    void updatePins(void);
};


#endif /*__RELAY_CONTROL_H__*/
