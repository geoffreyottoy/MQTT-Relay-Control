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

RelayControl::RelayControl(uint8_t address, uint8_t nrCircuits /*, NVConfig config*/){
  _mcp = new Adafruit_MCP23017();
  _address = address;
  
}

void RelayControl::begin(void){
  _mcp->begin(_address);
  Serial.print("\tboard address: ");
  Serial.println(_address);
  Serial.print("\tboard relays: ");
  Serial.println(NUM_RELAYS);
  // update pins
  //updatePins();
}

void RelayControl::set(uint8_t * relay, uint8_t * state, uint8_t count){
  
  // update pins
  updatePins();
}

void RelayControl::printStates(void){
  Serial.println("\trelay states:");
}

