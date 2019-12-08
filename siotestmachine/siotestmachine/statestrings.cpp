/*
 * statestrings.cpp
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

char const *nodeRoleString[] = { "roleNull", "roleTestMaster", "roleTestSlave",
		"roleMaster", "roleSlave" };

char const *nullstateString[] { "nullNull" };

char const *slavestateString[] = { "slvNull", "slvSetup", "slvReceiveWait",
		"slvReceiveFail", "slvReceiveSuccess", "slvEcho", "slvEchoWait",
		"slvEchoFail", "slvEchoSuccess", "slvSuccess", "" };

char const *masterstateString[] = { "mstrNull", "mstrSetup", "mstrSendWait",
		"mstrReceiveWait", "mstrFail", "mstrRecieveSuccess", "mstrSuccess", "" };

char const *slaveteststateString[] = { "slvtestNull", "slvtestSetup",
		"slvtestReceiveWait", "slvtestReceive", "slvtestEchoWait",
		"slvtestEchoSuccess", "slvtestFail", "slvtestFailExit",
		"slvtestSuccess", "slvtestSuccessExit", "" };

char const *masterteststateString[] = { "mstrtestNull", "mstrtestSetup",
		"mstrtestSend", "mstrtestRequest", "mstrtestReceiveEcho",
		"mstrtestProcEcho", "mstrtestFail", "mstrtestFailExit",
		"mstrtestReceiveSuccess", "mstrtestSuccessExit", "" };

char const *eotxString[] = { "eotxSuccess", "eotxBufOverrun", "eotxAddrNack",
		"eotxDataNack", "eotxError" };

