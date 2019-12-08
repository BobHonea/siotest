#include "debug.h"
#include "statedefs.h"
#include "siotypes.h"
#include "statestrings.h"
#include "i2c_io.h"
#include "utility.h"
#include "payload.h"
#include "debug.h"
#include "printpacket.h"
#include "packetio.h"

//#define SELECTPKT

void initMasterTestSM(void) {
	dprintf("initMasterStateSM\n");
	masterTestState = mstrtestSetup;
	statemachineRetryCount = 0;
}

void smTESTMASTER(void) {

	switch (masterTestState) {
	case mstrtestNull:
		fatal("illegal Null State");
		break;

	case mstrtestSetup:
		if (statemachineRetryCount == 0) {
			i2cInitMasterSession();
		}
		masterTestState = mstrtestSend;
		break;

	case mstrtestSend:

	{
#ifndef SELECTPKT
		// randomize payload bytes
		txPkt.packet.count = sizeof(txPkt.packet.data);
		memmove(txPkt.packet.data, nextRandomPayload(),
				sizeof(txPkt.packet.data));
#else
        
        txPkt.packet.count=sizeof(txPkt.packet.data);
        memmove(txPkt.packet.data, nextPatternedPayload(), sizeof(txPkt.packet.data));
#endif

#ifdef PRINTPKTIO
		displayBytes("TxPkt: ", txPkt.byteArray, sizeof(txPkt.byteArray));
#endif        
		pktRxCount = 0;
		int eotxResult = i2cMasterSendPacket(slaveAddress, txPkt.byteArray,
				sizeof(txPkt.byteArray));

#ifdef PRINTPKTIO
		printf("%s\n", eotxString[eotxResult]);

#endif
		switch (eotxResult) {
		case eotxSuccess:
			masterTestState = mstrtestRequest;
			break;

		case eotxBufferOverrun:
			fatal("txbuf overrun");
			break;

		case eotxAddrNack:
		case eotxDataNack:
		case eotxOtherError:
			masterTestState = mstrtestFail;
			break;
		}
	}
		break;

	case mstrtestRequest:
		// request echo packet
	{
		int returnedBytes=i2cMasterRequestBytes(slaveAddress, sizeof(rxPkt.byteArray));

		if (returnedBytes==sizeof(rxPkt.byteArray)) {
			masterTestState = mstrtestReceiveEcho;
		} else {
			masterTestState = mstrtestFail;
		}
	}
		break;

	case mstrtestReceiveEcho: {
		// packet is buffered, receive it
		bool pktGood = receiveI2CPacket(sizeof(rxPkt.byteArray));

		// receive complete packet only
		if (pktGood) {
			if (memcmp(rxPkt.byteArray, txPkt.byteArray,
					sizeof(txPkt.byteArray)) == 0) {
				masterTestState = mstrtestProcEcho;
				break;
			} else {
				dprintf("Echo Corrupt\n");
			}
		}

		masterTestState = mstrtestFail;
	}
		break;

	case mstrtestProcEcho: {
		int exchangePkgMatch = true;

		for (unsigned int i = 0; i < sizeof(txPkt.packet.data); i++) {
			if (txPkt.packet.data[i] != rxPkt.packet.data[i]) {
				exchangePkgMatch = false;
				break;
			}
		}

		if (exchangePkgMatch) {
			masterTestState = mstrtestSuccessExit;
		} else {
			masterTestState = mstrtestFail;
		}
	}
		break;

	case mstrtestFail:
		if (++statemachineRetryCount < MAXRETRIES) {
			dprintf("testRetry\n");
			masterTestState = mstrtestSetup;
		} else {
			dprintf("retriesExhausted\n");
			masterTestState = mstrtestFailExit;
		}
		break;

	case mstrtestFailExit:
		masterTestState = mstrtestNull;
		nodeRole = roleNULL;
		nullState = nullNull;
		i2cSessionEnd();
		break;

	case mstrtestSuccessExit:
		statemachineRetryCount = 0;
		masterTestState = mstrtestSetup;
		i2cSessionEnd();
		break;
	}
}
