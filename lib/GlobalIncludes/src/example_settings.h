/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: example_settings.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file with various "global" program settings.
 *      This is an example -> change filename to settings.h
 *
 */

#ifndef __EXAMPLE_SETTINGS_H__
#define __EXAMPLE_SETTINGS_H__


/*****************************************************************************
 * MQTT settings                                                             *
 *****************************************************************************/
#define MQTT_BROKER         "mqtt.broker.url"
#define MQTT_PORT           1234
#define MQTT_USER           "user"
#define MQTT_PASSKEY        "PassKey123"


/*****************************************************************************
 * Ethernet and IP settings                                                  *
 *****************************************************************************/
// Uncomment and change to your preferred fixed IP address if you want your
// device to have a IP address
//#define FIXED_IP          { 192, 168, 0, 1 }
//#define DNS_IP            { 8, 8, 8, 8 }

// Ethernet MAC address
//  -> can be generated with https://www.miniwebtool.com/mac-address-generator/
#define MAC_ADDRESS         { 0x01, 0x23, 0x45, 0xFE, 0xDC, 0xBA }


/*****************************************************************************
 * Setttings for debugging and logging                                       *
 *****************************************************************************/
// Comment if you don't want any debug output on the serial port
#define SERIAL_DEBUG_OUTPUT


#endif /*__EXAMPLE_SETTINGS_H__*/
