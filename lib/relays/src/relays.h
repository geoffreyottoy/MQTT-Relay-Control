#ifndef __RELAYS_H__
#define __RELAYS_H__

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

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

class Relays{
  public:
    Relays(uint8_t address=DEFAULT_ADDRESS);
    void begin(void);
    void set(uint8_t * relay, uint8_t * state, uint8_t count);
    uint8_t get(uint8_t relay);
    uint8_t toggle(uint8_t * relay);

    void printStates(void);

  private:
    Adafruit_MCP23017 * _mcp;
    uint8_t _address;
    Relay_t _relays[NUM_RELAYS];

    void updatePins(void);
};


#endif /*__RELAYS_H__*/
