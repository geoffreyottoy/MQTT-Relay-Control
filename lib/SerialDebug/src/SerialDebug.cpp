/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: SerialDebug.cpp
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Implementation of the SerialDebug class that offers
 *      functionality to easily enable/disable debug output on the 
 *      serial port.
 *
 */

#include "SerialDebug.h"

SerialDebug DEBUG;

SerialDebug::SerialDebug(){
}

void SerialDebug::begin(unsigned long baud){
#ifdef SERIAL_DEBUG_OUTPUT
    Serial.begin(baud);
    while(!Serial);
    Serial.flush();
#endif
}

void SerialDebug::flush(){
#ifdef SERIAL_DEBUG_OUTPUT
    Serial.flush();
#endif
}

size_t SerialDebug::print(const __FlashStringHelper *ifsh){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(reinterpret_cast<const char *>(ifsh));
#else
    return -1;
#endif
}

size_t SerialDebug::print(const String &s){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(s);
#else
    return -1;
#endif
}

size_t SerialDebug::print(const char str[]){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(str);
#else
    return -1;
#endif
}

size_t SerialDebug::print(char c){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(c);
#else
    return -1;
#endif
}

size_t SerialDebug::print(unsigned char b, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(b, base);
#else
    return -1;
#endif
}

size_t SerialDebug::print(int n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::print(unsigned int n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::print(long n , int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::print(unsigned long n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::print(double n, int digits){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.print(n, digits);
#else
    return -1;
#endif
}


size_t SerialDebug::println(const __FlashStringHelper *ifsh){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(reinterpret_cast<const char *>(ifsh));
#else
    return -1;
#endif
}

size_t SerialDebug::println(const String &s){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(s);
#else
    return -1;
#endif
}

size_t SerialDebug::println(const char str[]){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(str);
#else
    return -1;
#endif
}

size_t SerialDebug::println(char c){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(c);
#else
    return -1;
#endif
}

size_t SerialDebug::println(unsigned char b, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(b, base);
#else
    return -1;
#endif
}

size_t SerialDebug::println(int n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::println(unsigned int n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::println(long n , int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::println(unsigned long n, int base){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(n, base);
#else
    return -1;
#endif
}

size_t SerialDebug::println(double n, int digits){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println(n, digits);
#else
    return -1;
#endif
}

size_t SerialDebug::println(void){
#ifdef SERIAL_DEBUG_OUTPUT
    return Serial.println();
#else
    return -1;
#endif
}
