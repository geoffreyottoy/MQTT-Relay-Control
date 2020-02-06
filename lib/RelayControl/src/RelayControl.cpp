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

#define LONG_PRESS      3000 // ms
#define SHORT_PRESS     1000 // ms
#define BLINK_INTERVAL  1000 // ms

volatile bool btnPressed[MAX_NR_CIRCUITS];
bool btnPresHandled[MAX_NR_CIRCUITS];
unsigned long prevPress[MAX_NR_CIRCUITS];
unsigned long prevMillis = 0;

// pin interrupt callbacks
void intPin0Callback(void){
  btnPressed[0] = true;
  Serial.println("cb0");
}

void intPin1Callback(void){
  btnPressed[1] = true;
  Serial.println("cb1");
}

void intPin2Callback(void){
  btnPressed[2] = true;
  Serial.println("cb2");
}

void intPin3Callback(void){
  btnPressed[3] = true;
  Serial.println("cb3");
}

// dummy callback for pin interrupt
void nullCb(void){
  return;
}

typedef void (*BtnPressCb)(void);
BtnPressCb btnPressCallbacks[MAX_NR_CIRCUITS] = {
  intPin0Callback,
  intPin1Callback,
  intPin2Callback,
  intPin3Callback
};

RelayControl::RelayControl(uint8_t address, uint8_t nrCircuits /*, NVConfig config*/){
  this->_mcp = new Adafruit_MCP23017();
  this->_address = address;
  this->_nrCircuits = nrCircuits;
  
  // currently a fixed initialization
  // goal is to make it flexible later
  if(nrCircuits == 4){
    this->_circuits = (Circuit_t *)malloc(sizeof(Circuit_t)*nrCircuits);

    this->setName(0, "Stopcontact");
    this->_circuits[0]._buttonPin = 0;
    this->_circuits[0]._ledPin = 4;
    this->_circuits[0]._relays[0] = 0; // REL8
    this->_circuits[0]._relays[1] = 1; // REL6
    
    this->setName(1, "Licht poortje");
    this->_circuits[1]._buttonPin = 1;
    this->_circuits[1]._ledPin = 5;
    this->_circuits[0]._relays[0] = 10; // REL7
    this->_circuits[0]._relays[1] = 11; // REL5

    this->setName(2, "Licht terras");
    this->_circuits[2]._buttonPin = 2;
    this->_circuits[2]._ledPin = 6;
    this->_circuits[0]._relays[0] = 2; // REL4
    this->_circuits[0]._relays[1] = 3; // REL2

    this->setName(3, "Licht planten");
    this->_circuits[3]._buttonPin = 3;
    this->_circuits[3]._ledPin = 7;
    this->_circuits[0]._relays[0] = 12; // REL3
    this->_circuits[0]._relays[1] = 13; // REL1

    for(uint8_t i=0; i<4; i++){
      this->_circuits[i]._enabled = false;
      this->_circuits[i]._onOff = false;
      this->_circuits[i]._updated = true;
    }
  }
}

void RelayControl::begin(void){
  //this->_mcp->begin(_address);
  Serial.print("\tboard address: ");
  Serial.println(_address);
  Serial.print("\tboard relays: ");
  Serial.println(NUM_RELAYS);

  // init hw
  for(uint8_t i=0; i<this->_nrCircuits; i++){
    // configure button
    pinMode(this->_circuits[i]._buttonPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(this->_circuits[i]._buttonPin), btnPressCallbacks[i], FALLING);
    btnPressed[i] = false;
    btnPresHandled[i] = true;

    // configure led
    pinMode(this->_circuits[i]._ledPin, OUTPUT);
    digitalWrite(this->_circuits[i]._ledPin, HIGH); // led off

    // configure MCP
    //this->_mcp->pinMode(this->_circuits[i]._relays[0], OUTPUT);
    //this->_mcp->pinMode(this->_circuits[i]._relays[1], OUTPUT);
  }
  // turn all circuits off
  //this->_mcp->writeGPIOAB(0x0000);
}

void RelayControl::loop(void){
  unsigned long currentMillis = millis();
  bool itsToggleTime = false;

  if(currentMillis - prevMillis > BLINK_INTERVAL){
    prevMillis = currentMillis;
    itsToggleTime = true;
  }


  for(uint8_t i=0; i<this->_nrCircuits; i++){
    // check button press
    if(btnPressed[i]){
      // register time
      btnPressed[i] = false;
      btnPresHandled[i] = false;
      prevPress[i] = millis();
    }

    // pin went high again (button released)
    if(!btnPresHandled[i]){
      if(digitalRead(this->_circuits[i]._buttonPin)){
        if(millis() - prevPress[i] > LONG_PRESS){
          Serial.print("Long press on btn");
          Serial.println(i+1);
          this->toggleEnabled(i);
        }
        if(millis() - prevPress[i] < SHORT_PRESS){
          Serial.print("Short press on btn");
          Serial.println(i+1);
          this->toggleOnOff(i);
        }
        btnPresHandled[i] = true;
      }
    }

    // check if we need to blink the led (circuit enabled, but not on)
    if(itsToggleTime && this->_circuits[i]._enabled && !this->_circuits[i]._onOff ){
      digitalWrite(this->_circuits[i]._ledPin, !digitalRead(this->_circuits[i]._ledPin));
    }

  }
}

void RelayControl::setEnabled(uint8_t cIdx, bool enable){
  this->_circuits[cIdx]._enabled = enable;
  this->_circuits[cIdx]._updated = true;
  if(!this->_circuits[cIdx]._enabled){
    this->_circuits[cIdx]._onOff = false;
    this->updateRelays(cIdx);
  }
}

void RelayControl::toggleEnabled(uint8_t cIdx){
  this->_circuits[cIdx]._enabled = !this->_circuits[cIdx]._enabled;
  this->_circuits[cIdx]._updated = true;
  if(!this->_circuits[cIdx]._enabled){
    this->_circuits[cIdx]._onOff = false;
    this->updateRelays(cIdx);
  }
}

void RelayControl::setOnOff(uint8_t cIdx, bool onOff){
  if(this->_circuits[cIdx]._enabled){
      this->_circuits[cIdx]._onOff = onOff;
      this->_circuits[cIdx]._updated = true;
      this->updateRelays(cIdx);
      digitalWrite(this->_circuits[cIdx]._ledPin, this->_circuits[cIdx]._onOff ? LOW : HIGH);
  }
}

void RelayControl::toggleOnOff(uint8_t cIdx){
  if(this->_circuits[cIdx]._enabled){
      this->_circuits[cIdx]._onOff = !this->_circuits[cIdx]._onOff;
      this->_circuits[cIdx]._updated = true;
      this->updateRelays(cIdx);
      digitalWrite(this->_circuits[cIdx]._ledPin, this->_circuits[cIdx]._onOff ? LOW : HIGH);
  }
}

void RelayControl::updateRelays(uint8_t cIdx){
  uint8_t state;
  if(this->_circuits[cIdx]._onOff){
    state = HIGH;
  }
  else{
    state = LOW;
  }

  //this->_mcp->digitalWrite(this->_circuits[cIdx]._relays[0], state);
  //this->_mcp->digitalWrite(this->_circuits[cIdx]._relays[1], state);
}

void RelayControl::printStates(void){
  Serial.println("\trelay states:");
}

void RelayControl::setName(uint8_t cIdx, const char * name){
  this->_circuits[cIdx]._name = (char *)malloc(strlen(name)+1);
  strcpy(this->_circuits[cIdx]._name, name);
}

bool RelayControl::getStatus(char * statusStr){
  if(statusStr == NULL){
    Serial.println("strNull");
    return false;
  }

  // build statusstr
  uint8_t len = 0;
  uint8_t updated = false;
  for(uint8_t i=0; i<this->_nrCircuits; i++){
    if(this->_circuits[i]._updated){
      updated = true;
      this->_circuits[i]._updated = false;
      //Serial.println(statusStr);
    }
  }

  if(updated){
    for(uint8_t i=0; i<this->_nrCircuits; i++){
      sprintf(statusStr+i*4, "%02d", i+1);
      if(this->_circuits[i]._enabled){
        statusStr[i*4+2] = '1';
      }
      else{
        statusStr[i*4+2] = '0';
      }
      if(this->_circuits[i]._onOff){
        statusStr[i*4+3] = '1';
      }
      else{
        statusStr[i*4+3] = '0';
      }
      len+=4;
      statusStr[len] = '\0';
    }
    Serial.println(strlen(statusStr));
  }

  return updated;
}

bool RelayControl::updateStatus(char * statusStr){
  if(statusStr == NULL){
    Serial.println("strNull");
    return false;
  }

  if((strlen(statusStr) % 4) != 0){
    Serial.println("wrong length");
    return false;
  }

  // build statusstr
  uint8_t len = strlen(statusStr) / 4;
  char idStr[3];
  for(uint8_t i=0; i<len; i++){
    idStr[0] = statusStr[4*i];
    idStr[1] = statusStr[4*i+1];
    idStr[2] = '\0';
    uint8_t cIdx = strtol(idStr, NULL, 10) - 1;
    Serial.print("idStr: ");
    Serial.println(idStr);
    Serial.print("cIdx: ");
    Serial.println(cIdx);
    if(cIdx > this->_nrCircuits){
      return false;
    }

    if(statusStr[4*i+2] == '1'){
      this->setEnabled(cIdx, true);
    }
    else{
      this->setEnabled(cIdx, false);
    }

    if(statusStr[4*i+3] == '1'){
      this->setOnOff(cIdx, true);
    }
    else{
      this->setOnOff(cIdx, false);
    }
  }

  return true;
}
