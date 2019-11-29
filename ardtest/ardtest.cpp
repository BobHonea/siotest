    
#include <Arduino.h>
#include <Wire.h>

#include "debug.h"
#include "i2cstate.h"
#include "utility.h"
#include "printpacket.h"
#include "packetio.h"
#include "i2c_io.h"

// Slave receives a packet with this structure
//  | bytcount | byte0 | byte1 | ... | byte-bytecount-1) |
// On receipt of a packet:
// The master does not drop the bus, but expects a loopback packet
// the slave sends the just-received packet to the master
//


// if random number is > maxrand/2 assume master status
// else, slave status
// master sends packet, waits for packet echo from slave
// slave waits for a packet, on receiving, echoes packet to master
// roles become fixed on packet+echo transaction success
//
// packet transmission succeeds if there a master and a slave
// packet transmission fails if there are two masters
// packet transmission fails if there are two slaves
// there can be nasty collisions if two masters, nevermind
// on failure, random restart will occur.
// there should be no damage to hardware.
// on failure, repeat randomization and assumption






int nodeRole = roleNULL;

SLAVESTATE slaveState = slvNull;
MASTERSTATE masterState = mstrNull;
SLAVETESTSTATE slaveTestState = slvtestNull;
MASTERTESTSTATE masterTestState = mstrtestNull;
NULLSTATE nullState = nullNull;

int statemachineRetryCount = 0;

#define MAXRETRIES 3





ROLES prevState={ .null=nullNull };
ROLE         prevRole=roleNULL;
//static ROLESTATE    prevState={0};


// include statemachine function definitions
//#include "master.h"
//#include "slave.h"
#include "testmaster.h"
#include "testslave.h"
#include "teststate.h"

#ifdef DEBUG
void doPreLoopTests(void) {
  printf("**Begin Tests**\n");
  //testBool();
  delayWatchTest();
  usDelayWatchTest();
  printAllStateStrings();
  printf("***End Tests***\n");

}
#endif



void setup(void) {
  initPrintf();
  
  printf("\n\n****************************");
  printf(  "\n*  i2c test version 0.6.0  *");
  printf(  "\n****************************\n");
  

  initRandomPackets();
  resetPayloadIndex();
  
 #ifdef DEBUG
  if (false) {
    doPreLoopTests();
    
  }
#endif
  
i2cInitHardware();


}



void loop(void) {




  if (true) {
    //spinMarquee();

    // slow state machine
    // halt after N loops
    static int loopcount=0;
    delay(40);
    loopcount++;
    if (loopcount == 200) {
      printf("stall\n");
      while (true) delay(1000);
    }
  }
  
  
  //reportState();

  switch (nodeRole) {

    case roleNULL:
      {
        // randomize test session role

        if (pinDownBool()) {
//        if (randomBool()) {
//        if (true) {
          nodeRole = roleTESTMASTER;
          printf("is master\n");
          initMasterTestSM();

        } else {
          nodeRole = roleTESTSLAVE;
          printf("is slave\n");
          initSlaveTestSM();
        }
      }

      break;


    case roleTESTMASTER:
      smTESTMASTER();
      break;

    case roleTESTSLAVE:
      smTESTSLAVE();
      break;


    case roleMASTER:
      smTESTMASTER();
      break;

    case roleSLAVE:
      smTESTSLAVE();
      break;
      
    default:
      fatal("unknown state");
      break;
  }
}
