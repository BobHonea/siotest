/*
 * utility.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef UTILITY_H_
#define UTILITY_H_

bool msTimeoutOnBoolean(bool *waitBool, bool waitOnValue, unsigned long delayMS,
		unsigned long sampleIntervalMS);
bool usTimeoutOnBoolean(bool *waitBool, bool waitOnValue, unsigned long delayUS,
		unsigned long sampleIntervalUS);

bool pinDownBool(void);
bool randomBool(void);
void seedRandomGenerator(void);

#ifdef TEST_US_DELAY
bool usDelayWatchTest();
#endif
#ifdef TEST_MS_DELAY
bool msDelayWatchTest();
#endif
#endif /* UTILITY_H_ */
