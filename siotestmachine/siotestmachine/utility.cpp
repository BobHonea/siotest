#include <Arduino.h>
#include <Time.h>

typedef struct {
	unsigned long initialMS;
	unsigned long lastMS;
	unsigned long delayMS;
	bool delayOver;
} millisTimer;

typedef struct {
	unsigned long initialUS;
	unsigned long lastUS;
	unsigned long delayUS;
	bool delayOver;
} microsTimer;

// MAXMSM is MAXLONG
// MAX PERIOD is MAXMSM+1
#define MAXLONG 0xffffffff
#define MAXMS MAXLONG+1
#define MAXUS MAXMS
millisTimer msTimer;
microsTimer usTimer;

#ifdef TEST_MS_DELAY

void msDelayWatch(void) {
  printf("delay(%lums)  last(%lums)  initial(%lums)  over?(%d)\n",
          msTimer.delayMS, msTimer.lastMS, msTimer.initialMS, msTimer.delayOver);
}

bool msDelayWatchTest() {

  initMSdelayWatch(1000);
  msDelayWatch();
  delay(800);
  bool over800=MSdelayOver();
  delay(220);
  bool over1020=MSdelayOver();
  msDelayWatch();

  initMSdelayWatch(4000);
  msDelayWatch();
  delay(3800);
  bool over3800=MSdelayOver();
  msDelay(4020);
  bool over4020=MSdelayOver();
  msDelayWatch();

  dprintf("MS Timer Watch:");

  if (!over800 && ! over3800 && over1020 && over4020) {
    dprintf(" SUCCESS\n");
  } else {
    dprintf(" FAIL\n");
    fatal("Millisecond Timer Watch Uncalibrated");
  }

  return true;
}


#endif // TEST_MS_DELAY

#ifdef TEST_US_DELAY
void usDelayWatch(void) {
  printf("delay(%luus)  last(%luus)  initial(%luus)  over?(%d)\n", \
          usTimer.delayUS, usTimer.lastUS, usTimer.initialUS, usTimer.delayOver);
}


bool usDelayWatchTest() {

  initUSdelayWatch(400);
  usDelayWatch();
  delay(200);
  bool over200=USdelayOver();
  delay(402);
  bool over402=USdelayOver();
  usDelayWatch();


  initUSdelayWatch(100);
  usDelayWatch();
  delay(40);
  bool over40=USdelayOver();
  delay(62);
  bool over102=USdelayOver();
  usDelayWatch();

  dprintf("US Timer Watch:");


  dprintf("\n>40?%d   >102?%d  >200?%d   >402?%d", (int)over40, (int)over102, (int)over200, (int)over402);

  if (!over40 && over102 && !over200 && over402) {
    dprintf(" SUCCESS/n");
  } else {
    dprintf(" FAIL/n");
    fatal("Microsecond Timer Watch Uncalibrated");
  }

  return true;
}

#endif // TEST_US_DELAY

void initMSdelayWatch(unsigned long delayMS) {
	msTimer.initialMS = msTimer.lastMS = millis();
	msTimer.delayMS = delayMS;
	msTimer.delayOver = false;
}

void initUSdelayWatch(unsigned long delayUS) {
	usTimer.initialUS = usTimer.lastUS = micros();
	usTimer.delayUS = delayUS;
	usTimer.delayOver = false;
}

bool MSdelayOver(void) {
	unsigned long now = millis();
	unsigned long delay;

	if (!msTimer.delayOver) {
		if (now < msTimer.initialMS) {
			delay = MAXMS - msTimer.initialMS + now;
		} else {
			delay = now - msTimer.initialMS;
		}

		msTimer.lastMS = now;
		if (delay >= msTimer.delayMS) {
			msTimer.delayOver = true;
		}
	}
	return msTimer.delayOver;
}

bool USdelayOver(void) {
	unsigned long now = micros();
	unsigned long delay;

	if (!usTimer.delayOver) {
		if (now < usTimer.initialUS) {
			delay = MAXUS - usTimer.initialUS + now;
		} else {
			delay = now - usTimer.initialUS;
		}

		usTimer.lastUS = now;
		if (delay >= usTimer.delayUS) {
			usTimer.delayOver = true;
		}
	}
	return usTimer.delayOver;
}

bool msTimeoutOnBoolean(bool *waitBool, bool waitOnValue, unsigned long delayMS,
		unsigned long sampleIntervalMS) {
	initMSdelayWatch(delayMS);
	while (!MSdelayOver()) {
		if (waitOnValue == *waitBool) {
			return false;
		}
		delay(sampleIntervalMS);
	}
	return true;
}

bool usTimeoutOnBoolean(bool *waitBool, bool waitOnValue, unsigned long delayUS,
		unsigned long sampleIntervalUS) {
	initUSdelayWatch(delayUS);
	// microsecond delay inaccurate above 16384 us
	// limit delay sample interval

	unsigned long usInterval = sampleIntervalUS;
	unsigned long thisUSinterval;

	while (!USdelayOver()) {
		if (waitOnValue == *waitBool) {
			return false;
		}

		thisUSinterval = (usInterval > 8192) ? 8192 : usInterval;
		usInterval -= thisUSinterval;
		delay(usInterval);
	}
	return true;
}

bool pinDownBool(void) {
	pinMode(9, INPUT);
	bool result = digitalRead(9);
	return result;
}

void seedRandomGenerator(void) {
	static bool seedGenerated = false;

	if (!seedGenerated) {
		unsigned long randSeed = analogRead(0);
		randomSeed(randSeed);
		seedGenerated = true;
	}
}

#define MAXRAND 100
#define MIDRAND 49

bool randomBool(void) {
	seedRandomGenerator();

	int randCount = random(MAXRAND);

	if (randCount > MIDRAND) {
		return true;
	} else {
		return false;
	}
}
