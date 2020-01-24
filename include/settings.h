/*  
 *   ___|             _|  _|                  _ \  |   |               
 *  |      _ \  _ \  |   |    __| _ \ |   |  |   | __| __|  _ \  |   | 
 *  |   |  __/ (   | __| __| |    __/ |   |  |   | |   |   (   | |   | 
 * \____|\___|\___/ _|  _|  _|  \___|\__, | \___/ \__|\__|\___/ \__, | 
 *                                   ____/                      ____/  
 *
 *         File: settings.h
 *      Created: 2020-01-24
 *       Author: Geoffrey Ottoy
 *      Version: 1.0
 *
 *  Description: Header file with various "global" program settings.
 *
 */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/*****************************************************************************
 * MQTT settings                                                             *
 *****************************************************************************/
#define MQTT_BROKER         "postman.cloudmqtt.com"
#define MQTT_PORT           11921
#define MQTT_USER           "nycrtcth"
#define MQTT_PASSKEY        "KYwygsu9kJPI"


/*****************************************************************************
 * Ethernet and IP settings                                                  *
 *****************************************************************************/
// Uncomment and change to your preferred fixed IP address if you want your
// device to have a IP address
//#define FIXED_IP          { 192, 168, 0, 56 }

// Ethernet MAC address
//  -> generated with https://www.miniwebtool.com/mac-address-generator/
#define MAC_ADDRESS         { 0x97, 0x69, 0xCB, 0xF9, 0xEA, 0x17 };


/*****************************************************************************
 * Setttings for debugging and logging                                       *
 *****************************************************************************/
// Comment if you don't want any debug output on the serial port
#define SERIAL_DEBUG_OUTPUT


#endif /*__SETTINGS_H__*/
