/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: SerialDebug.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file for the SerialDebug class that offers
 *      functionality to easily enable/disable debug output on the 
 *      serial port.
 *
 */

#ifndef __SERIAL_DEBUG_H__
#define __SERIAL_DEBUG_H__

#include <Arduino.h>
#include "settings.h"

#define DEBUG_BAUD  57600

class SerialDebug{
    public:
        SerialDebug();
        void begin(unsigned long baud=DEBUG_BAUD);
        void flush(void);

        size_t print(const __FlashStringHelper *);
        size_t print(const String &);
        size_t print(const char[]);
        size_t print(char);
        size_t print(unsigned char, int = DEC);
        size_t print(int, int = DEC);
        size_t print(unsigned int, int = DEC);
        size_t print(long, int = DEC);
        size_t print(unsigned long, int = DEC);
        size_t print(double, int = 2);

        size_t println(const __FlashStringHelper *);
        size_t println(const String &s);
        size_t println(const char[]);
        size_t println(char);
        size_t println(unsigned char, int = DEC);
        size_t println(int, int = DEC);
        size_t println(unsigned int, int = DEC);
        size_t println(long, int = DEC);
        size_t println(unsigned long, int = DEC);
        size_t println(double, int = 2);
        size_t println(void);
};

extern SerialDebug DEBUG;

#endif /*__SERIAL_DEBUG_H__*/
