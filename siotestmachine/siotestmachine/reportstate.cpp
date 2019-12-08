//#include "i2cstate.h"
#include "debug.h"
#include "siotypes.h"
#include "statedefs.h"
#include "statestrings.h"


extern ROLES prevState;
extern ROLE prevRole;

#if TEST_STATE_STRINGS

void printAllStateStrings(void) {
  for (int i=0; (ROLE)i != roleMax; i++) {
    printf("%s\n",nodeRoleString[i]);
  }
  
  for (int i=0; (NULLSTATE) i<nullMax; i++) {
    printf("%s\n",nullstateString[i]);
  }
  
  for (int i=0; (MASTERSTATE) i<mstrMax; i++) {
    printf("%s\n",masterstateString[i]);
  }
  
  for (int i=0; (SLAVESTATE) i<slvMax; i++) {
    printf("%s\n",slavestateString[i]);
  }
  
  for (int i=0; (MASTERTESTSTATE) i<mstrtestMax; i++) {
    printf("%s\n",masterteststateString[i]);
  }

  for (int i=0; (SLAVETESTSTATE) i<slvtestMax; i++) {
    printf("%s\n",slaveteststateString[i]);
  }
}
#endif

#if REPORT_STATE_NAMES

void reportState(void) {
  static bool firstPass=true;
  bool   displayState=false;
  char const * displayStateString="undefined";
  
  if (firstPass || (nodeRole==roleNULL)) {
    firstPass=false;
    displayState=true;
    printf("====================");
  }
 

  
  if (prevRole != nodeRole) {
    prevRole=nodeRole;
    displayState=true;
  }
  

  switch (nodeRole) {
    case roleNULL:
      if (nullState!=prevState.null) {
        displayState=true;
      }
      

      if (displayState)
        displayStateString=nullstateString[nullState];
      break; 
    
    case roleSLAVE:
      if (slaveState != prevState.slave) { 
        prevState.slave=slaveState;
        displayState=true;
      }

      if (displayState)
        displayStateString=slavestateString[slaveState];
      break;
    
    case roleMASTER:
      if (masterState != prevState.master) {
        prevState.master=masterState;
        displayState=true;
      }
      
      if (displayState)
        displayStateString=masterstateString[masterState];

      break;
      
    case roleTESTSLAVE:
      if (slaveTestState != prevState.slavetest) {
        prevState.slavetest=slaveTestState;
        displayState=true;
      }

      if (displayState)
        displayStateString=slaveteststateString[slaveTestState];
      break;
      
    case roleTESTMASTER:
      if (masterTestState != prevState.mastertest) {
        prevState.mastertest=masterTestState;
        displayState=true;
      }

      if (displayState)
      displayStateString=masterteststateString[masterTestState];
      break;
      
    default:
      break;
  }

  if (displayState==true) {
    printf("%s\n", displayStateString);
  }
  
}

#endif
