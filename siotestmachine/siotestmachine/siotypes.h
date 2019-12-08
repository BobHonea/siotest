/*
 * siotypes.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef SIOTYPES_H_
#define SIOTYPES_H_

#include "statedefs.h"

typedef unsigned char byte;
extern ROLE nodeRole;
extern SLAVESTATE slaveState;
extern MASTERSTATE masterState;
extern SLAVETESTSTATE slaveTestState;
extern MASTERTESTSTATE masterTestState;
extern NULLSTATE nullState;
extern int statemachineRetryCount;
#define MAXRETRIES 3

#endif /* SIOTYPES_H_ */
