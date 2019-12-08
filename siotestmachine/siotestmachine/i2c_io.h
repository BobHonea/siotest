/*
 * i2c_io.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef I2C_IO_H_
#define I2C_IO_H_
#include "payload.h"
#include "statedefs.h"
extern byte slaveAddress;
extern UPacket txPkt;

typedef void i2cOnReceiveHandler(int howMany);
typedef void i2cOnRequestHandler(void);

int i2cMasterRequestBytes( byte slaveAddress, int byteCount);
void i2cRegisterRequestHandler(i2cOnRequestHandler requestHandler);
void i2cRegisterReceiveHandler(i2cOnReceiveHandler receiveHandler);
void i2cInitHardware(void);
void i2cInitSlaveSession(byte slaveAddress);
void i2cInitMasterSession(void);
void i2cSessionEnd(void);
eotxRESULT i2cMasterSendPacket(byte slave_address, byte *data,
		int byte_count);
eotxRESULT i2cSlaveSendPacket(byte *data, int byteCount);
bool receiveI2CPacket(int byteCount);

#endif /* I2C_IO_H_ */
