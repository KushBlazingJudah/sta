#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define BATNAME "BAT0"
#define SEP " | "

static void block_time(char out[BLOCKSIZE]);
static void block_battery(char out[BLOCKSIZE]);

block blocks[] = {
	{ block_battery, 60 },
	{ block_time, 10 },
};

static void block_time(char out[BLOCKSIZE])
{
	time_t t;
	struct tm *ti;

	t = time(NULL);
	ti = localtime(&t);

	strftime(out, BLOCKSIZE, "%b %d %H:%M\0", ti);
}

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

	out[off] = '%';
	out[off+1] = 0;
}
