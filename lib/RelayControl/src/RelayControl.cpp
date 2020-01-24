/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: RelayControl.cpp
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Implementation of the RelayControl class that offers
 *      an easy-to-use API to control the relays on the relay_board 
 *      (take caution, this is hardware-specific!
 *        -> see https://github.com/geoffreyottoy/MQTT-Relay-Control/blob/master/doc ).
 *
 */

#include <Arduino.h>
#include <Wire.h>

#include "RelayControl.h"

// TODO: check pins (schematic;)
static uint8_t relPinLUT[8]{
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7
};

Relays::Relays(uint8_t address){
  _mcp = new Adafruit_MCP23017();
  _address = address;
  for(uint8_t i=0; i<NUM_RELAYS; i++){
    _relays[i].name = i+1;
    _relays[i].pin = relPinLUT[i];
    _relays[i].state = LOW;
  }
}

void Relays::begin(void){
  _mcp->begin(_address);
  Serial.print("\tboard address: ");
  Serial.println(_address);
  Serial.print("\tboard relays: ");
  Serial.println(NUM_RELAYS);
  // update pins
  //updatePins();
}

void Relays::set(uint8_t * relay, uint8_t * state, uint8_t count){
  for(uint8_t i=0; i<count; i++){
    for(uint8_t j=0; j<NUM_RELAYS; j++){
      if(_relays[j].name == relay[i]){
        _relays[j].state = state[i];
      }
    }
  }
  // update pins
  updatePins();
}

void Relays::printStates(void){
  Serial.println("\trelay states:");
  for(uint8_t i=0; i<NUM_RELAYS; i++){
    Serial.print("\t - relay: REL");
    Serial.println(_relays[i].name);
    Serial.print("\t - pin: ");
    Serial.println(_relays[i].pin);
    Serial.print("\t - state: ");
    if(_relays[i].state == HIGH){
      Serial.println("HIGH");
    }
    else{
      Serial.println("LOW");
    }
  }
}


void Relays::updatePins(void){
   for(uint8_t i=0; i<NUM_RELAYS; i++){
     _mcp->digitalWrite(_relays[i].pin, _relays[i].state);
   }
}