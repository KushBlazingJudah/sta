#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>

#define BLOCKSIZE 16

static Display *dpy;
static int screen;
static Window root;

typedef struct {
	void (*f)(char out[BLOCKSIZE]);
	unsigned int iv;
} block;

#include "config.stax.h"

#define nblocks (sizeof(blocks)/sizeof(block))

unsigned int timers[nblocks] = {0};
static char line[nblocks][BLOCKSIZE] = {0};
static char lastline[nblocks][BLOCKSIZE] = {0};
static char buf[(nblocks * BLOCKSIZE) + (sizeof(SEP) * (nblocks - 1)) + 1] = {0};

void exit_handler(void)
{
	XCloseDisplay(dpy);
}

int main(void)
{
	unsigned int min = 0;
	unsigned int slept = 0;
	size_t i = 0;
	int copied = 0;

	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		fprintf(stderr, "failed to open display\n");
		return 1;
	}
	atexit(exit_handler);

	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

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
			char* ptr = (char*)buf;
			for (i = 0; i < nblocks; ++i) {
				if (i != 0)
					ptr = mempcpy(ptr, SEP, sizeof(SEP));
				ptr = mempcpy(ptr, line[i], strlen(line[i]));
			}

			XStoreName(dpy, root, (char*)buf);
			XFlush(dpy);
		}

		if (min) sleep(min);
	}
}
