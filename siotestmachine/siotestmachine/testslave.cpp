#include "debug.h"
#include "i2c_io.h"
#include "statedefs.h"
#include "utility.h"
#include "payload.h"
#include "siotypes.h"
#include "packetio.h"
#include "printpacket.h"

int slavetestRxRequested = false;
bool slavetestEchoRequested = false;
bool slavetestEchoOnRequestComplete = false;
bool slavetestEchoOnRequestSuccess = false;

byte slavetestRxPacketGood;
bool slavetestRxPacketOccurred;
int slavetestRxByteCount;


void slavetestEchoOnRequest(void) {

	slavetestEchoOnRequestComplete = true;

	if (slavetestEchoRequested) {
		// rxPkt is good, echo it to master
		eotxRESULT eotxResult = i2cSlaveSendPacket(rxPkt.byteArray,
				sizeof(rxPkt.byteArray));

		if (eotxResult == eotxSuccess) {
			slavetestEchoOnRequestSuccess = true;
		} else {
			slavetestEchoOnRequestSuccess = false;
		}
	}
}

void slavetestOnRequest(void) {
	// on receive event, transition state to
	// sxlvtestReceive
	slavetestEchoRequested = false;
	slavetestEchoOnRequestComplete = false;
	slavetestRxRequested = true;
}

void slavetestOnRequestHandlerInit(void) {
	slavetestRxRequested = false;
	slavetestEchoRequested = false;
	slavetestEchoOnRequestComplete = false;
	i2cRegisterRequestHandler(slavetestEchoOnRequest);
}


void slavetestOnReceive(int bytesReceived) {
	slavetestRxByteCount = bytesReceived;
	slavetestRxPacketGood = receiveI2CPacket(bytesReceived);
	slavetestRxPacketOccurred = true;
}

void slavetestOnReceiveHandlerInit(void) {
	slavetestRxByteCount = 0;
	slavetestRxPacketGood = false;
	slavetestRxPacketOccurred = false;
	i2cRegisterReceiveHandler(slavetestOnReceive);
}

void initSlaveTestSM(void) {
	dprintf("InitSlaveTestSM\n");
	slaveTestState = slvtestSetup;
	statemachineRetryCount = 0;
}

void i2cSlaveHandlersInit(void) {
	slavetestOnRequestHandlerInit();
	slavetestOnReceiveHandlerInit();
}

void smTESTSLAVE(void) {

	switch (slaveTestState) {
	case slvtestNull:
		fatal("illegal State");
		break;

	case slvtestSetup:

		if (statemachineRetryCount == 0) {
			i2cInitSlaveSession(slaveAddress);
		}

		i2cSlaveHandlersInit();
		slaveTestState = slvtestReceiveWait;
		break;

	case slvtestReceiveWait:
		//for (int i=0;i<sizeof(rxPkt.byteArray); i++) rxPkt.byteArray[i]=0xcc;

		if (usTimeoutOnBoolean(&slavetestRxPacketOccurred, true, 4000000L,
				20)) {
			// one second timeout on packet reception expired
			slaveTestState = slvtestFail;
		} else {
			if (!slavetestRxPacketOccurred) {
				fatal("slavetestRxPacketOccurred fault!");
			}
			// packet received, advance
			slavetestEchoRequested = true;
			slavetestEchoOnRequestComplete=false;
			slaveTestState = slvtestReceive;
		}
		break;

	case slvtestReceive:
		if (rxPkt.packet.count == sizeof(rxPkt.packet.data)) {
			// advance if packet has integrity
			slaveTestState = slvtestEchoWait;
		} else {
			slaveTestState = slvtestFail;
		}
		break;

	case slvtestEchoWait:
		if (usTimeoutOnBoolean(&slavetestEchoOnRequestComplete, true, 4000000L,
				20)) {
			if (slavetestEchoOnRequestSuccess) {
				slaveTestState = slvtestEchoSuccess;
#ifdef PRINTPKTIO
				displayBytes("EchoRxPkt: ", rxPkt.byteArray,
						sizeof(rxPkt.byteArray));
#endif
				break;
			} else {
				printf("Echo Failed\n");
				slaveTestState = slvtestFail;
			}
		}
		break;

	case slvtestFail:
		if (++statemachineRetryCount < MAXRETRIES) {
			slaveTestState = slvtestSetup;
		} else {
			slaveTestState = slvtestFailExit;
		}
		break;

	case slvtestFailExit:
		nodeRole = roleNULL;
		slaveTestState = slvtestNull;
		nullState = nullNull;
		statemachineRetryCount = 0;
		i2cSessionEnd();
		break;

	case slvtestEchoSuccess:
		slaveTestState = slvtestSuccess;
		break;

	case slvtestSuccess:
		slaveTestState = slvtestSuccessExit;
		break;

	case slvtestSuccessExit:
		slaveTestState = slvtestSetup;
		statemachineRetryCount = 0;
		i2cSessionEnd();
		break;

	}

}
