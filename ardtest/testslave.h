#include "i2cstate.h"

int slavetestRxRequested=false;
bool slavetestEchoRequested=false;
bool slavetestEchoOnRequestComplete=false;
bool slavetestEchoOnRequestSuccess=false;

void slavetestEchoOnRequest() {
  
  slavetestEchoOnRequestComplete=true;

  if (slavetestEchoRequested) {
    // rxPkt is good, echo it to master
    int bytesEchoed=Wire.write(rxPkt.byteArray, sizeof(rxPkt.byteArray));
    
    if (bytesEchoed==sizeof(rxPkt.byteArray)) {
      slavetestEchoOnRequestSuccess=true;
    } else {
      slavetestEchoOnRequestSuccess=false;
    }
  }
}

void slavetestOnRequest(int howMany) {
  // on receive event, transition state to
  // slvtestReceive
  slavetestEchoRequested=false;
  slavetestEchoOnRequestComplete=false;
  slavetestRxRequested=true;  
}

void slavetestOnRequestHandlerInit(void) {
  slavetestRxRequested=false;
  slavetestEchoRequested=false;
  slavetestEchoOnRequestComplete=false;
}

byte slavetestRxPacketGood;
bool slavetestRxPacketOccurred;
int  slavetestRxByteCount;

void slavetestOnReceiveHandlerInit(void) {
  slavetestRxByteCount=0;
  slavetestRxPacketGood=false;
  slavetestRxPacketOccurred=false;
}

void slavetestOnReceive(int bytesReceived) {
  slavetestRxByteCount=bytesReceived;
  slavetestRxPacketGood=receiveI2CPacket(bytesReceived);
  slavetestRxPacketOccurred=true;
}


void initSlaveTestSM() {
  dprintf("InitSlaveTestSM\n");
  slaveTestState = slvtestSetup;
  statemachineRetryCount=0;
}

void i2cSlaveHandlersInit() {
  slavetestOnRequestHandlerInit();
  slavetestOnReceiveHandlerInit();
  Wire.onReceive(slavetestOnReceive);
  Wire.onRequest(slavetestEchoOnRequest);
}

smTESTSLAVE() 
{


  switch(slaveTestState) {
  
    case slvtestSetup:
 
      if (statemachineRetryCount==0) {
        i2cInitSlaveSession(slaveAddress);
      }

      i2cSlaveHandlersInit();
      slaveTestState=slvtestReceiveWait;
      break;
    
    case slvtestReceiveWait:
      //for (int i=0;i<sizeof(rxPkt.byteArray); i++) rxPkt.byteArray[i]=0xcc;
      
      if (usTimeoutOnBoolean(&slavetestRxPacketOccurred, true, 4000000L, 20)) {
        // one second timeout on packet reception expired
        slaveTestState=slvtestFail;
      } else {
        if (!slavetestRxPacketOccurred) {
          fatal("slavetestRxPacketOccurred fault!");
        }
        // packet received, advance
        slavetestEchoRequested=true;
        slaveTestState=slvtestReceive;
      }      
      break;
      
    case slvtestReceive:
      if (rxPkt.packet.count == sizeof(rxPkt.packet.data)) {
        // advance if packet has integrity
        slaveTestState=slvtestEchoWait;
      } else
      {
        slaveTestState=slvtestFail;
      }
      break;

    case slvtestEchoWait:
      if (usTimeoutOnBoolean(&slavetestEchoOnRequestComplete, true, 4000000L, 20)) {
        if (slavetestEchoOnRequestSuccess) {
          slaveTestState=slvtestEchoSuccess;
#ifdef PRINTPKTIO
          displayBytes("TxPkt: ", rxPkt.byteArray, sizeof(rxPkt.byteArray));
#endif
          break;
        } 
        slaveTestState=slvtestFail;
      }
      break;
      
      
    case slvtestFail:
      if (++statemachineRetryCount<MAXRETRIES) {
        slaveTestState=slvtestSetup;
      } else {
        slaveTestState=slvtestFailExit;
      }
      break;
      
    case slvtestFailExit:
      nodeRole=roleNULL;
      slaveTestState=slvtestNull;
      nullState=nullNull;
      statemachineRetryCount=0;
      break;
      
    case slvtestEchoSuccess:
      slaveTestState=slvtestSuccess;
      break;
      
    case slvtestSuccess:
      slaveTestState=slvtestSetup;
      statemachineRetryCount=0;
      break;
      
  }
      
}
