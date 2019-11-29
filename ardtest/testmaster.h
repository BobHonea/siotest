#include "i2cstate.h"

//#define SELECTPKT

static int receiveWaitMS;

void initMasterTestSM(void) {
  dprintf("initMasterStateSM\n");
  masterTestState = mstrtestSetup;
  statemachineRetryCount=0;
}

UPacket txPkt;


void smTESTMASTER(void) {

  
  switch (masterTestState) {
    case mstrtestSetup:
      if (statemachineRetryCount==0) {
        Wire.begin();
      }
      masterTestState=mstrtestSend;
      break;
  
    case mstrtestSend:
      
      {
#ifndef SELECTPKT
        // randomize payload bytes
        txPkt.packet.count=sizeof(txPkt.packet.data);
        memmove(txPkt.packet.data, nextRandomPayload(), sizeof (txPkt.packet.data));
#else
        
        txPkt.packet.count=sizeof(txPkt.packet.data);
        memmove(txPkt.packet.data, nextPatternedPayload(), sizeof(txPkt.packet.data));
#endif

#ifdef PRINTPKTIO
        displayBytes("TxPkt: ", txPkt.byteArray, sizeof(txPkt.byteArray));
#endif        
        pktRxCount=0;
        
        // send  packet
        Wire.beginTransmission(slaveAddress);
        Wire.write(txPkt.byteArray, sizeof(txPkt.byteArray));
        int eotxResult=Wire.endTransmission();
        
#ifdef PRINTPKTIO
        printf("%s\n",eotxString[eotxResult]);
#endif
        switch(eotxResult) {
          case eotxSuccess:
            masterTestState=mstrtestRequest;
            break;
            
          case eotxBufferOverrun:
            fatal("txbuf overrun");
            break;
            
          case  eotxAddrNack:
          case  eotxDataNack:
          case  eotxOtherError:
            masterTestState=mstrtestFail;
            break;
        }
      }
      break;
    
    case mstrtestRequest:
      // request echo packet
      {
        int returnedBytes=Wire.requestFrom(slaveAddress, sizeof(rxPkt.byteArray));
        masterTestState=mstrtestReceiveEcho;
      }
      break;
      
    case mstrtestReceiveEcho:
      {
        // packet is buffered, receive it
        bool pktGood=receiveI2CPacket(sizeof(rxPkt.byteArray));
        
        // receive complete packet only
        if (pktGood) {
          if (memcmp( rxPkt.byteArray, txPkt.byteArray, sizeof(txPkt.byteArray))==0) {
            masterTestState=mstrtestProcEcho;
            break;
          } else {
            dprintf("Echo Corrupt\n");
          }
        }
        
        masterTestState=mstrtestFail;
      }
      break;
      
    case mstrtestProcEcho:
      {    
        int exchangePkgMatch=true;

        for (int i=0; i<sizeof(txPkt.packet.data);i++) {
          if (txPkt.packet.data[i]!=rxPkt.packet.data[i]) {
            exchangePkgMatch=false;
            break;
          }
        }

        if (exchangePkgMatch) {
          masterTestState=mstrtestSuccessExit;
        } else {
          masterTestState=mstrtestFail;
        }
      }
      break;

    case mstrtestFail:
      if (++statemachineRetryCount<MAXRETRIES) {
        dprintf("testRetry\n");
        masterTestState=mstrtestSetup;
      } else {
        dprintf("retriesExhausted\n");
        masterTestState=mstrtestFailExit;
      }
      break;

    case mstrtestFailExit:
      masterTestState=mstrtestNull;
      nodeRole=roleNULL;
      nullState=nullNull;
      break;
      
    case mstrtestSuccessExit:
      statemachineRetryCount=0;
      masterTestState=mstrtestNull;
//      nodeRole=roleMASTER;
//      masterState=mstrNull;
      nodeRole=roleNULL;
      nullState=nullNull;
      break;
  }
}
