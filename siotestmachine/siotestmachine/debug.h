/*
 * debug.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <Arduino.h>

#define DEBUG

#ifdef DEBUG
#define TERSEDEBUG

#define REPORT_STATE_NAMES	false
#define TEST_BOOL			false
#define	TEST_MS_WATCH		false
#define TEST_US_WATCH		false
#define TEST_STATE_STRINGS	false
#define TESTS_DEFINED	(TEST_BOOL || TEST_MS_WATCH || TEST_US_WATCH || TEST_STATE_STRINGS)

extern void spinMarquee(void);

#ifndef TERSEDEBUG
// verbose options
#undef REPORT_STATE_NAMES
#define REPORT_STATE_NAMES true
//#define dprintf(X, ...) printf(X,...)
#define dprintf printf
#define PRINTPKTIO

#else //..ifndef TERSEDEBUG
// terse options

#define dprintf  noprintf

#endif  //..ifndef TERSEDEBUG

extern void spinMarquee(void);
#endif //..DEBUG
extern void noprintf(char const *fmt, ...);
extern void fatal(char const *reason);
extern void initPrintf(void);

#endif /* DEBUG_H_ */
