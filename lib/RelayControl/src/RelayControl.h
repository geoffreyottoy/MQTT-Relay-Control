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

#define DEFAULT_ADDRESS   0
#define NUM_RELAYS        8

#define MAX_NR_CIRCUITS   4

typedef struct circuit{
  /* data */
  char * _name;
  bool _enabled;
  bool _onOff;
  uint8_t _buttonPin;
  uint8_t _ledPin;
  uint8_t _relays[2];
  //uint16_t _gpioAB;
  bool _updated;
  bool _btnReleased;
  bool _pressHandled;
  unsigned long _prevPress;
} Circuit_t;

class RelayControl{
  public:
    RelayControl(uint8_t address=DEFAULT_ADDRESS, uint8_t nrCircuits=MAX_NR_CIRCUITS /*, NVConfig config*/);
    
    void begin(void);
    void setName(uint8_t cIdx, const char * name);
    void loop(void);

    bool getStatus(char * statusStr);
    bool updateStatus(char * statusStr);

    void printStates(void);

    void readySequence(void);

  private:
    Adafruit_MCP23017 * _mcp;
    uint8_t _address;
    uint8_t _nrCircuits;
    Circuit_t * _circuits;

    uint8_t _toggleState = LOW;

    void setOnOff(uint8_t cIdx, bool onOff);
    void toggleOnOff(uint8_t cIdx);
    void setEnabled(uint8_t cIdx, bool enable);
    void toggleEnabled(uint8_t cIdx);
    void updateRelays(uint8_t cIdx);

};


#endif /*__RELAY_CONTROL_H__*/
