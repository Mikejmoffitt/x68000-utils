/*

// TODO: Page 224 of Inside has a good example of the raster copy

X68000 CRT Controller Helper Functions (crtc)
c. Michael Moffitt 2021

x68k_crtc_init_default does not touch display timings, and only resets scroll for
all layers. R20 and R21 are set up with what I would hope are sane defaults,
but this only affects the shadow copies, which will only be committed if those
registers are touched.

*/
#ifndef _X68K_CRTC_H
#define _X68K_CRTC_H

#include <stdint.h>

// VRAM memory mapping
#define GVRAM_BASE ((uint8_t *)0xC00000)
#define TVRAM_BASE ((uint8_t *)0xE00000)
#define CRTC_BASE ((uint16_t *)0xE80000)

// Struct representing CRTC configuration registers.

/*

Calculations for R00 - R07, translated from InsideX68000:

R00 = ((Hsync period * H total dots) / (H active dots * 8)) - 1
R01 = ((Hsync pulse * H total dots) / (H active dots * 8)) - 1
R02 = (((Hsync pulse + H back porch) * (H total dots)) / (H active dots)) - 5
R03 = (((Hsync period + H front porch) * (H total dots)) / (H active dots)) - 5
R04 = (Vsync period / Hsync period) - 1
R05 = (Vsync pulse / Hsync period) - 1
R06 = ((Vsync pulse + V back porch) / Hsync period) - 1
R07 = ((Vsync period - V front porch) / Hysnc period) - 1

Sample config table from Inside X68000:

    [       High-resolution       ] [   Low resolution    ]
    768x512 512x512 512x256 256x256 512x512 512x256 256x256
R00     $89     $5B     $5B     $2D     $4B     $4B     $25
R01     $0E     $09     $09     $04     $03     $03     $01
R02     $1C     $11     $11     $06     $05     $05     $00
R03     $7C     $51     $51     $26     $45     $45     $20
R04    $237    $237    $237    $237    $103    $103    $103
R05     $05     $05     $05     $05     $02     $02     $02
R06     $28     $28     $28     $28     $10     $10     $10
R07    $228    $228    $228    $228    $100    $100    $100
R08     $1B     $1B     $1B     $1B     $2C     $2C     $24

*/

typedef struct X68kCrtcConfig
{
	// R00 - R08 at 0xE80000
	uint16_t htotal;        // 1 bit = 8px
	uint16_t hsync_length;
	uint16_t hdisp_start;
	uint16_t hdisp_end;
	uint16_t vtotal;        // 1 bit = 1 line
	uint16_t vsync_length;  // "
	uint16_t vdisp_start;   // "
	uint16_t vdisp_end;     // "
	uint16_t ext_h_adjust;

	// R09 - R19 are not represented here (scroll registers).

	// R20 at 0xE80028. Bits 8-10 (color, plane size) should match the video
	// controller's top 3 bits (0xE82400).
	uint16_t flags;
	/*
	15                0
	.... .zcc ...f vvhh
	      ||     | | \__ Horizontal disp:  00 = 256 dots
	      ||     | |                       01 = 512 dots
	      ||     | |                       10 = invalid config?
	      ||     | |                       11 = 768 dots (50MHz dotclock?) no good for PCG
	      ||     | |
	      ||     |  \___ Vertical disp:    00 = 256 dots
	      ||     |                         01 = 512 dots
	      ||     |                         10 = invalid config
	      ||     |                         11 = invalid config
	      ||     |
	      ||      \_____ Frequency:        0 = 15.98KHz
	      ||                               1 = 31.5KHz (is this the dot clock?)
	      ||
	      |\____________ Color depth:      00 = 16 colors
	      |                                01 = 256 colors
	      |                                10 = invalid config
	      |                                11 = 65536 colors
	      |
	       \____________ Plane size:       0 = 512  dots
	                                       1 = 1024 dots
	*/
} X68kCrtcConfig;

// Apply an X68kCrtcConfig the hardware registers.
// Scroll positions will be initialized to zero.
void x68k_crtc_init(const X68kCrtcConfig *c);

// R09: Raster number for raster interrupt
void x68k_crtc_set_raster_interrupt(uint16_t v);

// R10 - R19: Scroll registers ===============================================

// R10: Text layer X scroll
void x68k_crtc_set_text_xscroll(uint16_t v);

// R11: Text layer Y scroll
void x68k_crtc_set_text_yscroll(uint16_t v);

// R12: Graphic layer 0 X scroll
void x68k_crtc_set_gp0_xscroll(uint16_t v);

// R13: Graphic layer 0 Y scroll
void x68k_crtc_set_gp0_yscroll(uint16_t v);

// R14: Graphic layer 1 X scroll
void x68k_crtc_set_gp1_xscroll(uint16_t v);

// R15: Graphic layer 1 Y scroll
void x68k_crtc_set_gp1_yscroll(uint16_t v);

// R16: Graphic layer 2 X scroll
void x68k_crtc_set_gp2_xscroll(uint16_t v);

// R17: Graphic layer 2 Y scroll
void x68k_crtc_set_gp2_yscroll(uint16_t v);

// R18: Graphic layer 3 X scroll
void x68k_crtc_set_gp3_xscroll(uint16_t v);

// R19: Graphic layer 3 Y scroll
void x68k_crtc_set_gp3_yscroll(uint16_t v);

// TODO: Raster copy stuff (R21-R23)

// CRTC control port
void x68k_crtc_set_control(uint8_t v);

#endif // XCRTC_H
