#include "x68000/x68k_crtc.h"


// Set up some sane defaults
// TODO: Verify on hardware
void x68k_crtc_init(const X68kCrtcConfig *c)
{
	volatile uint16_t *reg = CRTC_BASE;
	reg[0] = c->htotal;
	reg[1] = c->hsync_length;
	reg[2] = c->hdisp_start;
	reg[3] = c->hdisp_end;
	reg[4] = c->vtotal;
	reg[5] = c->vsync_length;
	reg[6] = c->vdisp_start;
	reg[7] = c->vdisp_end;
	reg[8] = c->ext_h_adjust;

	reg[20] = c->flags;
	// R22-R23 and the CTRL register are not touched here
}

// R09: Raster number for raster interrupt
void x68k_crtc_set_raster_interrupt(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80012;
	*reg = v;
}

// R10 - R19: Scroll registers ===============================================

// R10: Text layer X scroll
void x68k_crtc_set_text_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80014;
	*reg = v;
}

// R11: Text layer Y scroll
void x68k_crtc_set_text_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80016;
	*reg = v;
}

// R12: Graphic layer 0 X scroll
void x68k_crtc_set_gp0_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80018;
	*reg = v;
}

// R13: Graphic layer 0 Y scroll
void x68k_crtc_set_gp0_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001A;
	*reg = v;
}

// R14: Graphic layer 1 X scroll
void x68k_crtc_set_gp1_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001C;
	*reg = v;
}

// R15: Graphic layer 1 Y scroll
void x68k_crtc_set_gp1_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001E;
	*reg = v;
}

// R16: Graphic layer 2 X scroll
void x68k_crtc_set_gp2_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80020;
	*reg = v;
}

// R17: Graphic layer 2 Y scroll
void x68k_crtc_set_gp2_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80022;
	*reg = v;
}

// R18: Graphic layer 3 X scroll
void x68k_crtc_set_gp3_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80024;
	*reg = v;
}

// R19: Graphic layer 3 Y scroll
void x68k_crtc_set_gp3_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80026;
	*reg = v;
}

// CRTC control port
void x68k_crtc_set_control(uint8_t v)
{
	volatile uint8_t *reg = (uint8_t *)0xE80481;
	*reg = v;
}
