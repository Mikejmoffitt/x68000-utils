#ifndef _X68K_JOY_H
#define _X68K_JOY_H

#include <stdint.h>

typedef enum X68kJoyBits
{
	KEY_UP = 0x01,
	KEY_DOWN = 0x02,
	KEY_LEFT = 0x04,
	KEY_RIGHT = 0x08,
	KEY_A = 0x10,
	KEY_B = 0x20,
} X68kJoyBits;

static inline X68kJoyBits x68k_joy_read(uint8_t id)
{
	volatile uint8_t *pad = (volatile uint8_t *)(0xE9A001 + (id ? 2 : 0));
	return *pad;
}

#endif // _X68K_JOY_H
