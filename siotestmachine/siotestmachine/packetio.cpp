#include "payload.h"

volatile int lastEotxResult;
volatile int pktRxCount = 0;
volatile int pktTxCount = 0;
volatile int pktRxBadCount = 0;
int TxPkgCountReported = 0;
int RxPkgCountReported = 0;
int RxBadPacketsReported = 0;
UPacket rxPkt = { 0, 0 };

int rxpkgNdx;
byte rxpkgComplete;
int rxpkgSizeReported;
int rxpkgBytesRead;
bool rxpkgGood;

void receivePacketInit(void) {
	memset(rxPkt.byteArray, 0, sizeof(rxPkt.byteArray));
	rxpkgComplete = false;
	rxpkgNdx = 0;
	rxpkgSizeReported = rxpkgBytesRead = 0;
	rxpkgGood = false;
}

