#include <Arduino.h>
#include "debug.h"

#if TESTS_DEFINED
void doPreLoopTests(void) {
  printf("**Begin Tests**\n");
#ifdef TEST_BOOL
  testBool();
#endif
#ifdef TEST_MS_WATCH
  msDelayWatchTest();
#endif
#ifdef TEST_US_WATCH
  usDelayWatchTest();
#endif
#ifdef TEST_STATE_STRINGS
  printAllStateStrings();
#endif
  printf("***End Tests***\n");
}
#endif // TESTS_DEFINED

void fatal(char const *reason) {
	printf("FATAL: %s\n", reason);
	// abort after time for message display
	delay(200);
	abort();
}

#ifdef DEBUG

#define MOD1 40
#define MOD2 4
#define MOD3 16

int spunNdx = 0;
int spinNdx = 0;
char const *spinChr[] = { "|", "/", "-", "\\" };

void spinMarquee(void) {
	spunNdx += 1;
	if (spunNdx % MOD1 == 0) {
		spunNdx = 0;
		spinNdx += 1;
		printf(spinChr[spinNdx % MOD2]);

		if (spinNdx % MOD3 == 0) {
			printf("!\n");
		}
	}
}

#endif //DEBUG

// Setup printf() by redirecting putchar to Serial.write(c)
// Function that printf and related will use to print

int serial_putchar(char c, FILE *f) {
	if (c == '\n')
		serial_putchar('\r', f);
	return Serial.write(c) == 1 ? 0 : 1;
}

void noprintf(char const *fmt, ...) {
}

FILE serial_stdout;

void initPrintf(void) {
	// configure Serial Print I/O
	Serial.begin(9600);
	// Set up stdout
	fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &serial_stdout;
}

