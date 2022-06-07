#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libnotify/notify.h>

#define BLOCKSIZE 32

typedef struct {
	void (*f)(char out[BLOCKSIZE]);
	unsigned int iv;

	char* title;
	char* icon;
	unsigned int timeout;
} block;

#include "config.stan.h"

#define nblocks (sizeof(blocks)/sizeof(block))

unsigned int timers[nblocks] = {0};
static char line[nblocks][BLOCKSIZE] = {0};

int main(void)
{
	unsigned int min = 0;
	unsigned int slept = 0;
	size_t i = 0;
	int copied = 0;

	notify_init("stan");
	atexit(notify_uninit);

	while(1) {
		slept = min;
		min = 1<<8;
		copied = 0;

		for (i = 0; i < nblocks; ++i) {
			if ((timers[i]-=slept) <= 0) {
				line[i][0] = 0;

				blocks[i].f(line[i]);
				timers[i] = blocks[i].iv;

				// Check to see if it has changed
				if (line[i][0]) {
					NotifyNotification *n = notify_notification_new(blocks[i].title, line[i], blocks[i].icon);
					notify_notification_set_timeout(n, blocks[i].timeout);
					notify_notification_show(n, NULL);
					g_object_unref(G_OBJECT(n));
				}
			}

			if (timers[i] < min)
				min = timers[i];
		}

		if (min) sleep(min);
	}
}
