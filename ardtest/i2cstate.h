
#ifndef I2CSTATEHEADER
#define I2CSTATEHEADER




typedef enum {
  roleNULL=0,
  roleTESTMASTER=1,
  roleTESTSLAVE=2,
  roleMASTER=3,
  roleSLAVE=4,
  roleMax=roleSLAVE
} ROLE;

char * nodeRoleString[]={
  "roleNull",
  "roleTestMaster",
  "roleTestSlave",
  "roleMaster",
  "roleSlave"
};

typedef enum {
  nullNull,
  nullMax=nullNull
} NULLSTATE;


char const * nullstateString[] {
  "nullNull"
};

/*
#define roleNull 0
#define roleTESTMASTER 1
#define roleTESTSLAVE 2
#define roleMASTER 3
#define roleSLAVE 4
*/

typedef enum {
  slvNull=0,
  slvSetup,
  slvReceiveWait,
  slvReceiveFail,
  slvReceiveSuccess,
  slvEcho,
  slvEchoWait,
  slvEchoFail,
  slvEchoSuccess,
  slvSuccess,
  slvMax=slvSuccess
} SLAVESTATE;

char const * slavestateString[]={
  "slvNull",
  "slvSetup",
  "slvReceiveWait",
  "slvReceiveFail",
  "slvReceiveSuccess",
  "slvEcho",
  "slvEchoWait",
  "slvEchoFail",
  "slvEchoSuccess",
  "slvSuccess",
  ""
};

typedef enum {
  mstrNull=0,
  mstrSetup,
  mstrSendWait,
  mstrReceiveWait,
  mstrFail,
  mstrRecieveSuccess,
  mstrSuccess,
  mstrMax=mstrSuccess,
} MASTERSTATE;

char const * masterstateString[]={
  "mstrNull",
  "mstrSetup",
  "mstrSendWait",
  "mstrReceiveWait",
  "mstrFail",
  "mstrRecieveSuccess",
  "mstrSuccess",
  ""
};

typedef enum {
  slvtestNull=0,
  slvtestSetup,
  slvtestReceiveWait,
  slvtestReceive,
  slvtestEchoWait,
  slvtestEchoSuccess,
  slvtestFail,
  slvtestFailExit,
  slvtestSuccess,
  slvtestSuccessExit,
  slvtestMax=slvtestSuccessExit
} SLAVETESTSTATE;

char const * slaveteststateString[]={
  "slvtestNull",
  "slvtestSetup",
  "slvtestReceiveWait",
  "slvtestReceive",
  "slvtestEchoWait",
  "slvtestEchoSuccess",
  "slvtestFail",
  "slvtestFailExit",
  "slvtestSuccess",
  "slvtestSuccessExit",
  ""
};

typedef enum {
  mstrtestNull=0,
  mstrtestSetup,
  mstrtestSend,
  mstrtestRequest,
  mstrtestReceiveEcho,
  mstrtestProcEcho,
  mstrtestFail,
  mstrtestFailExit,
  mstrtestReceiveSuccess,
  mstrtestSuccessExit,
  mstrtestMax=mstrtestSuccessExit,
} MASTERTESTSTATE;

char const * masterteststateString[]={
  "mstrtestNull",
  "mstrtestSetup",
  "mstrtestSend",
  "mstrtestRequest",
  "mstrtestReceiveEcho",
  "mstrtestProcEcho",
  "mstrtestFail",
  "mstrtestFailExit",
  "mstrtestReceiveSuccess",
  "mstrtestSuccessExit",
  ""
};

typedef enum {
  eotxSuccess,
  eotxBufferOverrun,
  eotxAddrNack,
  eotxDataNack,
  eotxOtherError,
  eotxMax=eotxOtherError
} eotxRESULT;

char const * eotxString[5] = {
  "eotxSuccess",
  "eotxBufOverrun",
  "eotxAddrNack",
  "eotxDataNack",
  "eotxError"
};


typedef union {
  NULLSTATE       null;
  SLAVESTATE      slave;
  MASTERSTATE     master;
  SLAVETESTSTATE  slavetest;
  MASTERTESTSTATE mastertest;
} ROLES;

#endif