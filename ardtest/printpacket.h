#ifndef __PRINTPACKET_H_
#define __PRINTPACKET_H_

char * sprintfByteArray(char *bufptr, byte *byteArray, int arrayLen, char * charfmt) {
  int bufOffset=0;
  
  for (int i=0, bufOffset=0;i<arrayLen;i++) {
    bufOffset+=sprintf(bufptr+bufOffset, charfmt, (byte) byteArray[i]);
  }
  
  return bufOffset;
}


void displayBytes(char * label, byte * bytes, int bytecount) {
  char  spbuff[40];
  int bytesFormatted=sprintf(spbuff, "%s", label);
  sprintfByteArray(spbuff+bytesFormatted, bytes, bytecount, " %02x ");
  printf("%s\n",spbuff);
}

extern volatile int pktRxCount;
extern volatile int pktRxBadCount;
extern int RxBadPacketsReported;
extern int RxPkgCountReported;

void resetPacketCount(void) {
  pktRxCount=0;
}

void resetBadRxPacketCount(void) {
  pktRxBadCount=0;
}


void signalPacketRx(void) {
  pktRxCount+=1;
  
  if (pktRxCount > RxPkgCountReported) {
    char loopBuf[16];
    RxPkgCountReported = pktRxCount;
    printf("rx %02d\n", RxPkgCountReported);
  }

  if (pktRxBadCount > RxBadPacketsReported) {
    char loopBuf[16];
    RxBadPacketsReported = pktRxBadCount;
    printf("bad %02d\n", RxBadPacketsReported);
  }
}

#endif //__PRINTPACKET_H_
