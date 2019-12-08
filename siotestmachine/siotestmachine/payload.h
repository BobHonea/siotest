/*
 * payload.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#include <Arduino.h>

//#define BUFSIZE 32    // general i2c receive buffer
#define PKGCOUNT 8    // count of canonical exchange packets
#define PKGSIZE 31     // size of exchange packet payloads
#define PKTSIZE PKGSIZE+1

typedef struct {
	byte count;
	byte data[PKGSIZE];
} DataPacket;

typedef union {
	DataPacket packet;
	byte byteArray[PKTSIZE];
} UPacket;

extern byte patternedPayload[PKGCOUNT][PKGSIZE];
extern void initRandomPackets(void);
extern byte* nextRandomPayload(void);
extern void resetPayloadIndex(void);
#endif /* PAYLOAD_H_ */
