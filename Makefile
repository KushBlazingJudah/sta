CFLAGS ?= -O2 -std=c99 -pedantic -g

all: sta stan stax

sta: sta.c
	$(CC) -o $@ $^ $(CFLAGS)

stan: stan.c
	$(CC) -o $@ $< `pkg-config --cflags --libs libnotify` $(CFLAGS)

stax: stax.c
	$(CC) -o $@ $< -lX11 $(CFLAGS)

clean:
	rm -f sta stan stax

.PHONY: clean
