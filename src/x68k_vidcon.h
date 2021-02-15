/*

X68000 Video Controller Helper Functions (vidcon)
c. Michael Moffitt 2017

Provides some helper functions for configuring the video controller. The
configuration is stored in three words of shadow copies of the registers,
which are initially set with x68k_vidcon_init_default().

Afterwards, individual bits may be set or cleared with the clearly named
helper functions. These functions modify the shadow copies. To write the shadow
copies to the actual hardware registers, call x68k_vidcon_commit_regs().

*/

#ifndef X68K_VIDCON_H
#define X68K_VIDCON_H

#include <stdint.h>

// RGB palette entry macro
#define PAL_RGB5(r, g, b) ( (((r) & 0x1F) << 6) | (((g) & 0x1F) << 11) | (((b) & 0x1F) << 1) )
#define PAL_RGB4(r, g, b) ( (((r << 1) & 0x1F) << 6) | (((g << 1) & 0x1F) << 11) | (((b << 1) & 0x1F) << 1) )
#define PAL_RGB8(r, g, b) ( (((r >> 3) & 0x1F) << 6) | (((g >> 3) & 0x1F) << 11) | (((b >> 3) & 0x1F) << 1) )

/*

Example config:

screen = 0x0000;  // 512x512, 16-color
prio = 0x12E4;  // Text > PCG > (GP0 > GP1 > GP2 > GP3)
flags = 0x007F;  // Enable all layers.

*/

typedef struct X68kVidconConfig
{
	// R0: Screen
	// Bits 0-1: color depth (0 = 16, 1 = 256, 2 = invalid, 3 = 65536)
	// Bit 2: "Real screen mode" 0 = 512x512, 1 = 1024x1024
	// This should match CRTC R20 bits 8-10 shifted down to bits 0-2.
	uint16_t screen;

	// R1: Priotity
	// Ordering between layer groups, and between the graphics planes within
	// the graphics plane group.
	// 0 = topmost; 3 = bottom-most.
	// 15 -------------- 0
	// ..PP .... .... .... PCG priority 
	// .... TT.. .... .... Text priority
	// .... ..GG .... .... GP priority
	// .... .... 33.. ....  --> GP3 priority
	// .... .... ..22 ....  --> GP2 priority
	// .... .... .... 11..  --> GP1 priority
	// .... .... .... ..00  --> GP0 priority
	uint16_t prio;

	// R2: Flags
	// 15 -------------- 0
	// V... .... .... .... Video cut / don't show image even when superimposing
	// .A.. .... .... .... Ah: translucent text palette 0 regardless of exon ?
	// ..v. .... .... .... Vht: Video image (used in color image unit)
	// ...E .... .... .... Exon: Enable special priority / translucent mode
	// .... H... .... .... Hp: special priority (0) or translucent (1) select
	// .... .B.. .... .... Bp: Sharp reserved (0) or something else (1) ???
	// .... ..G. .... .... GG: Set translucent: graphics screen
	// .... ...T .... .... GT: Set translucent: text/sprite screen
	// .... .... b... .... border: enable border color display
	// .... .... .P.. .... Enable PCG display
	// .... .... ..t. .... Enable Text display
	// .... .... ...g .... Enable GP display
	// .... .... .... 3... GP3 enable
	// .... .... .... .2.. GP2 enable
	// .... .... .... ..1. GP1 enable
	// .... .... .... ...0 GP0 enable
	uint16_t flags;
} X68kVidconConfig;

void x68k_vidcon_init(const X68kVidconConfig *c);

// Graphics plane palette entries
static inline void x68k_vidcon_set_gp_color(uint8_t index, uint16_t val)
{
	volatile uint16_t *p = (volatile uint16_t *)0xE82000;
	p += index;
	*p = val;
}

// Text plane color entries
static inline void x68k_vidcon_set_text_color(uint8_t index, uint16_t val)
{
	volatile uint16_t *p = (volatile uint16_t *)0xE82200;
	p += index;
	*p = val;
}

// Sprite palette entries
static inline void x68k_vidcon_set_pcg_color(uint8_t index, uint16_t val)
{
	volatile uint16_t *p = (volatile uint16_t *)0xE82200;
	p += index;
	*p = val;
}

#endif // X68K_VIDCON_H
