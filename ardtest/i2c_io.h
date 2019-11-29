
#define DEFAULT_I2C_CLOCKRATE 100000L
#define DEFAULT_I2C_SLAVE_ADDRESS 0x55

byte slaveAddress = DEFAULT_I2C_SLAVE_ADDRESS;

i2cInitHardware(void) {
    Wire.setClock(DEFAULT_I2C_CLOCKRATE);
}

i2cInitSlaveSession(byte slaveAddress) {
  Wire.begin(slaveAddress);
}

i2cInitMasterSession(unsigned long clockRate) {
  Wire.begin();
}

i2cSessionEnd(void) {
  Wire.end();
}

bool receiveI2CPacket(int byteCount) {
  // this event receives a packet of data
  // package format  [bytecount][byte 0, byte 1, ... , byte (bytecount)]

  // presuming receiveEvent is entered when receive trasaction is complete
  // otherwise the Init() will destroy information
  receivePacketInit();
  rxpkgSizeReported=byteCount;
  
#ifdef PRINTPKTIO
  char spbuff[40];
#endif

  int bytesRxd;
  for (bytesRxd=0 ;Wire.available() > 0; bytesRxd++) {
    rxPkt.byteArray[bytesRxd]=Wire.read();
  }

  #ifdef PRINTPKTIO
  printf("rxPkt bytes: expect(%d) actual(%d) size(%d)\n",
            byteCount, bytesRxd, sizeof(rxPkt.byteArray));
  #endif
  
  if (bytesRxd==sizeof(rxPkt.byteArray)) {
    rxpkgComplete=true;
    pktRxCount++;
    rxpkgGood=true;

  } else {
    rxpkgComplete=false;
    pktRxBadCount++;
    rxpkgGood=false;
  }
  
#ifdef PRINTPKTIO
  sprintf(spbuff, "rxPkt (%s): ", rxpkgGood? "OK" : "ERR");
  displayBytes(spbuff, rxPkt.byteArray, bytesRxd);
#endif
  return rxpkgGood;
}
