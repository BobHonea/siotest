/*
 * printpacket.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Asus
 */

#ifndef PRINTPACKET_H_
#define PRINTPACKET_H_

extern void displayBytes(char const *label, byte *bytes, int bytecount);
extern int sprintfByteArray(char *bufptr, byte *byteArray, int arrayLen,
		char const *charfmt);

#endif /* PRINTPACKET_H_ */
