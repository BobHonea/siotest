#include <Wire.h>
#include "debug.h"
#include "siotypes.h"
#include "packetio.h"
#include "statedefs.h"
#include "printpacket.h"
#include "i2c_io.h"

#define DEFAULT_I2C_CLOCKRATE 400000L
#define DEFAULT_I2C_SLAVE_ADDRESS 0x55

byte slaveAddress = DEFAULT_I2C_SLAVE_ADDRESS;

void i2cInitHardware(void) {
	Wire.setClock(DEFAULT_I2C_CLOCKRATE);
}

void i2cInitSlaveSession(byte slaveAddress) {
	i2cInitHardware();
	Wire.begin(slaveAddress);
}

void i2cRegisterRequestHandler(i2cOnRequestHandler requestHandler) {
	Wire.onRequest(requestHandler);
}

void i2cRegisterReceiveHandler(i2cOnReceiveHandler receiveHandler) {
	Wire.onReceive(receiveHandler);
}
void i2cInitMasterSession(void) {
	i2cInitHardware();
	Wire.begin();
}

void i2cSessionEnd(void) {
	Wire.end();
}


int i2cMasterRequestBytes( byte slaveAddress, int byteCount) {
	return(Wire.requestFrom((int)slaveAddress, byteCount));
}

UPacket txPkt;

eotxRESULT i2cMasterSendPacket(byte slave_address, byte *data, int byteCount) {

	// send  packet
	Wire.beginTransmission(slave_address);
	Wire.write(data, byteCount);
	int eotxResult = Wire.endTransmission();
	return ((eotxRESULT)eotxResult);
}

eotxRESULT i2cSlaveSendPacket(byte *data, int byteCount) {
	int bytesEchoed = Wire.write(data, byteCount);
	if (bytesEchoed == byteCount) {
		return eotxSuccess;
	} else {
		return eotxOtherError;
	}
}

bool receiveI2CPacket(int byteCount) {
	// this event receives a packet of data
	// package format  [bytecount][byte 0, byte 1, ... , byte (bytecount)]

	// presuming receiveEvent is entered when receive trasaction is complete
	// otherwise the Init() will destroy information
	receivePacketInit();
	rxpkgSizeReported = byteCount;

#ifdef PRINTPKTIO
  char spbuff[40];
#endif

	int bytesRxd;
	for (bytesRxd = 0; Wire.available() > 0; bytesRxd++) {
		rxPkt.byteArray[bytesRxd] = Wire.read();
	}

#ifdef PRINTPKTIO
  printf("rxPkt bytes: expect(%d) actual(%d) size(%d)\n",
            byteCount, bytesRxd, sizeof(rxPkt.byteArray));
  #endif

	if (bytesRxd == sizeof(rxPkt.byteArray)) {
		rxpkgComplete = true;
		pktRxCount++;
		rxpkgGood = true;

	} else {
		rxpkgComplete = false;
		pktRxBadCount++;
		rxpkgGood = false;
	}

#ifdef PRINTPKTIO
  sprintf(spbuff, "rxPkt (%s): ", rxpkgGood? "OK" : "ERR");
  displayBytes(spbuff, rxPkt.byteArray, bytesRxd);
#endif
	return rxpkgGood;
}
