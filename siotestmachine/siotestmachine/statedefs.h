#ifndef __STATEDEFS_H_
#define __STATEDEFS_H_

typedef enum {
	roleNULL = 0,
	roleTESTMASTER = 1,
	roleTESTSLAVE = 2,
	roleMASTER = 3,
	roleSLAVE = 4,
	roleMax = roleSLAVE
} ROLE;

typedef enum {
	nullNull, nullMax = nullNull
} NULLSTATE;

typedef enum {
	slvNull = 0,
	slvSetup,
	slvReceiveWait,
	slvReceiveFail,
	slvReceiveSuccess,
	slvEcho,
	slvEchoWait,
	slvEchoFail,
	slvEchoSuccess,
	slvSuccess,
	slvMax = slvSuccess
} SLAVESTATE;

typedef enum {
	mstrNull = 0,
	mstrSetup,
	mstrSendWait,
	mstrReceiveWait,
	mstrFail,
	mstrRecieveSuccess,
	mstrSuccess,
	mstrMax = mstrSuccess,
} MASTERSTATE;

typedef enum {
	slvtestNull = 0,
	slvtestSetup,
	slvtestReceiveWait,
	slvtestReceive,
	slvtestEchoWait,
	slvtestEchoSuccess,
	slvtestFail,
	slvtestFailExit,
	slvtestSuccess,
	slvtestSuccessExit,
	slvtestMax = slvtestSuccessExit
} SLAVETESTSTATE;

typedef enum {
	mstrtestNull = 0,
	mstrtestSetup,
	mstrtestSend,
	mstrtestRequest,
	mstrtestReceiveEcho,
	mstrtestProcEcho,
	mstrtestFail,
	mstrtestFailExit,
	mstrtestReceiveSuccess,
	mstrtestSuccessExit,
	mstrtestMax = mstrtestSuccessExit,
} MASTERTESTSTATE;

typedef enum {
	eotxSuccess,
	eotxBufferOverrun,
	eotxAddrNack,
	eotxDataNack,
	eotxOtherError,
	eotxMax = eotxOtherError
} eotxRESULT;

typedef union {
	NULLSTATE null;
	SLAVESTATE slave;
	MASTERSTATE master;
	SLAVETESTSTATE slavetest;
	MASTERTESTSTATE mastertest;
} ROLES;

#endif
