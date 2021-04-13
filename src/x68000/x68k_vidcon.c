#include "x68000/x68k_vidcon.h"

void x68k_vidcon_init(const X68kVidconConfig *c)
{
	volatile uint16_t *r0 = (volatile uint16_t *)0xE82400;
	volatile uint16_t *r1 = (volatile uint16_t *)0xE82500;
	volatile uint16_t *r2 = (volatile uint16_t *)0xE82600;

	*r0 = c->screen;
	*r1 = c->prio;
	*r2 = c->flags;
}
