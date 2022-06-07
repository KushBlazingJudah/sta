# sta

sta is a suite of programs similar in idea to dwmblocks, but slightly better.

Executing a program every *n* seconds kinda sucks in the grand scheme of things,
so when the time comes for the block to update, it'll call a function which
writes out its block.

There are two variants of sta, all of which share the same base code but do
something slightly different:

- stan (uses libnotify)
  - Sends a notification when blocks have changed.
  - Accepts the same configuration, but with extra parameters, namely `title`
    and `timeout` (in milliseconds)
- stax (sets the root window's title in X)
  - Accepts the same configuration as sta.
  - Offered solely for convenience.

They can all be configured in their respective `config.h` files.
Configurations between `sta` and `stax` are interchangeable, but going to `stan`
they'll work but not in the way you expect.

## Configuration

For `sta` and `stax`, this is pretty simple.
Define your blocks, give it an interval, write the function and send its output
to `out`.

```
static void block_battery(char out[BLOCKSIZE]);
static void block_time(char out[BLOCKSIZE]);

block blocks[] = {
	{ block_battery, 60 }, // time is in seconds
	{ block_time, 10 },
};

static void block_battery(char out[BLOCKSIZE])
{
  ...
}

static void block_time(char out[BLOCKSIZE])
{
  ...
}
```

`stan` follows the same principle but has a bigger `BLOCKSIZE` by default and
will display a notification if the first byte of `out` is not `0`.

## Building

Normal `sta` has zero dependencies, and can be built with `make sta`.

`stan` depends on `libnotify`, and can be built with `make stan`.

`stax` depends on `libX11`, and can be built with `make stax`.

All three of these can be built at once by simply running `make`.
