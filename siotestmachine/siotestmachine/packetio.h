/*
 * packetio.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef PACKETIO_H_
#define PACKETIO_H_

#include <Arduino.h>
#include "payload.h"

extern volatile int lastEotxResult;
extern volatile int pktRxCount;
extern volatile int pktTxCount;
extern volatile int pktRxBadCount;
extern int TxPkgCountReported;
extern int RxPkgCountReported;
extern int RxBadPacketsReported;
extern UPacket rxPkt;

extern int rxpkgNdx;
extern byte rxpkgComplete;
extern int rxpkgSizeReported;
extern int rxpkgBytesRead;
extern bool rxpkgGood;

void receivePacketInit(void);

#endif /* PACKETIO_H_ */
