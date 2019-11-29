

#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

//#define BUFSIZE 32    // general i2c receive buffer
#define PKGCOUNT 8    // count of canonical exchange packets
#define PKGSIZE 4     // size of exchange packet payloads
#define PKTSIZE PKGSIZE+1

typedef struct {
  byte  count;
  byte  data[PKGSIZE];
} DataPacket;

typedef union {
  DataPacket  packet;
  byte        byteArray[PKTSIZE];
} UPacket;

byte patternedPayload[PKGCOUNT][PKGSIZE] = {
  { 0x81, 0x81, 0x81, 0x81 },
  { 0x42, 0x42, 0x42, 0x42 },
  { 0x24, 0x24, 0x24, 0x24 },
  { 0x18, 0x18, 0x18, 0x18 },
  { 0xff, 0xff, 0xff, 0xff },
  { 0xc3, 0xc3, 0xc3, 0xc3 },
  { 0xf7, 0xf7, 0xf7, 0xf7 },
  { 0x00, 0x00, 0x00, 0x00 }
};

byte randomPayload[PKGCOUNT][PKGSIZE]={};

int packageIndex = 0;

static int payloadNdx=0;

void resetPayloadIndex(void) {
  payloadNdx=0;
}

int nextPackageIndex(void) {
  //return(++payloadNdx%PKGCOUNT);  
  return payloadNdx;
}

byte * nextPatternedPayload(void) {
  return patternedPayload[nextPackageIndex()];
}

byte * nextRandomPayload(void) {
  return randomPayload[nextPackageIndex()];
}

void initRandomPackets(void) {
  seedRandomGenerator();
  char spbuff[20];
  
  printf("RandomPayloads\n");
  
#ifdef PRINTPKTIO
  for (int i=0;i<PKGCOUNT;i++) {
    for (int j=0;j<PKTSIZE;j++) {
      randomPayload[i][j]=random(0x100);


    }
  
    sprintf(spbuff,"rndPkt[%d]: ",i);    
    displayBytes(spbuff,randomPayload[i], sizeof(randomPayload[i]));
  }
#endif
}

#endif //_PAYLOAD_H_
