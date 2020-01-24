/*  ____  ____      _    __  __  ____ ___
 * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
 * | | | | |_) |  / _ \ | |\/| | |  | | | |
 * | |_| |  _ <  / ___ \| |  | | |__| |_| |
 * |____/|_| \_\/_/   \_\_|  |_|\____\___/
 *                           research group
 *                             dramco.be/
 *
 *   KU Leuven - Technology Campus Gent,
 *   Gebroeders De Smetstraat 1,
 *   B-9000 Gent, Belgium
 *
 *         File: DNSresolver.h
 *      Created: 2019-09-27
 *       Author: Geoffrey Ottoy
 *      Version: 1
 *
 *  Description: Header file for DNSresolver.cpp. Defines the DNSresolver
 *    class. This is an attempt at resolving domain names on the ESP32.
 *
 *  Commissioned by Stillemans N.V.
 */

#ifndef __DNS_RESOLVER_H__
#define __DNS_RESOLVER_H__

#include <Arduino.h>
#include <EthernetUdp.h>

#ifndef htons
#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#endif

#ifndef ntohs
#define ntohs(x) htons(x)
#endif

#ifndef htonl
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#endif

#ifndef ntohl
#define ntohl(x) htonl(x)
#endif

//#include "WifiManager.h"

/* The DNSresolver class offers functionality to resolve domain names
 */
class DNSresolver{
public:
    DNSresolver(const IPAddress& aDNSServer);

    //IPAddress resolve(String name);
    
    void begin(const IPAddress& aDNSServer);

     /** Resolve the given hostname to an IP address.
        @param aHostname Name to be resolved
        @param aResult IPAddress structure to store the returned IP address
        @result 1 if aIPAddrString was successfully converted to an IP address,
                else error code
    */
    int getHostByName(const char* aHostname, IPAddress& aResult);

protected:

private:
    uint16_t BuildRequest(const char* aName);
    uint16_t ProcessResponse(uint16_t aTimeout, IPAddress& aAddress);
    int inet_aton_ethlib(const char* aIPAddrString, IPAddress& aResult);

    IPAddress iDNSServer;
    uint16_t iRequestId;
    EthernetUDP iUdp;
};

#endif /* __DNS_RESOLVER_H__ */
