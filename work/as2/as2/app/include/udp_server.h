// udp_server.h
// Listens for incoming UDP packets using a thread
// Provides light sampling data from the light detector
// Takes in commands [help, ?, count, length, dips, history, stop]

#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include <stdbool.h>

// Start the UDP server in its own thread
void UdpServer_start(void);

// Stop the UDP server and clean up resources.
void UdpServer_stop(void);

// Checks if UDP server is online or offline
bool UdpServer_isOnline(void);

#endif