#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BLOCKSIZE 16

typedef struct {
	void (*f)(char out[BLOCKSIZE]);
	unsigned int iv;
} block;

#include "config.h"

#define nblocks (sizeof(blocks)/sizeof(block))

unsigned int timers[nblocks] = {0};
static char line[nblocks][BLOCKSIZE] = {0};
static char lastline[nblocks][BLOCKSIZE] = {0};

int main(void)
{
	unsigned int min = 0;
	unsigned int slept = 0;
	size_t i = 0;
	int copied = 0;

	while(1) {
		slept = min;
		min = 1<<8;
		copied = 0;

		for (i = 0; i < nblocks; ++i) {
			if ((timers[i]-=slept) <= 0) {
				if (!copied) {
					memcpy(lastline, line, sizeof(line));
					copied = 1;
				}

				blocks[i].f(line[i]);
				timers[i] = blocks[i].iv;
			}

			if (timers[i] < min)
				min = timers[i];
		}

		if (copied && memcmp(line, lastline, sizeof(line))!=0) {
			for (i = 0; i < nblocks; ++i) {
				if (i != 0)
					write(STDOUT_FILENO, SEP, sizeof(SEP));
				write(STDOUT_FILENO, line[i], strlen(line[i]));
			}
			putchar('\n');
		}

		if (min) sleep(min);
	}
}
