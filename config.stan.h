#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define BATNAME "BAT0"

static void block_battery(char out[BLOCKSIZE]);
static int level = 0;

block blocks[] = {
	{ .f = block_battery, .iv = 120, .title = "Battery level" },
};

static void block_battery(char out[BLOCKSIZE])
{
	int fd;
	size_t off;

	if ((fd = open("/sys/class/power_supply/"BATNAME"/capacity", O_RDONLY)) == -1) {
		perror("open");
		return;
	}

	if ((off = read(fd, (char*)out, BLOCKSIZE-2)-1) < 0) { // % and \0
		perror("fread");
		close(fd);
		return;
	}

	close(fd);

	level = atoi((char*)out);
	if (level <= 10)
		snprintf((char*)out, BLOCKSIZE, "Battery is at %d%%.", level);
	else
		out[0] = 0; // Nothing to display

}
