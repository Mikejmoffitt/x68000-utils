#include "crtc.h"

// Shadow register copies for bitfields / combined registers
static uint16_t r20, r21, r22;

// Set up some sane defaults
// TODO: Verify on hardware
void crtc_init_default(void)
{
	crtc_set_text_xscroll(0);
	crtc_set_text_yscroll(0);
	crtc_set_gp0_xscroll(0);
	crtc_set_gp0_yscroll(0);
	crtc_set_gp1_xscroll(0);
	crtc_set_gp1_yscroll(0);
	crtc_set_gp2_xscroll(0);
	crtc_set_gp2_yscroll(0);
	crtc_set_gp3_xscroll(0);
	crtc_set_gp3_yscroll(0);

	r20 = 0x0015; // 31Khz 512x512, 16 color mode
	r21 = 0x00FF; // Disable simultaneous write stuff

	// R20-R23 and the CTRL register are not touched here
}

// R00 - R08: Display timings ================================================

// R00: Total horizontal period
void crtc_set_htotal(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80000;
	*reg = v;
}

// R01: HSync end position
void crtc_set_hsync_end(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80002;
	*reg = v;
}

// R02: H Display start
void crtc_set_hdisp_start(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80004;
	*reg = v;
}

// R03: H Display start
void crtc_set_hdisp_end(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80006;
	*reg = v;
}

// R04: Total vertical period
void crtc_set_vtotal(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80008;
	*reg = v;
}

// R05: VSync end position
void crtc_set_vsync_end(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8000A;
	*reg = v;
}

// R06: V Display start
void crtc_set_vdisp_start(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8000C;
	*reg = v;
}

// R07: V Display start
void crtc_set_vdisp_end(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8000E;
	*reg = v;
}

// R08 - R09: Misc ===========================================================

// R08: External sync horizontal phase fine adjustment
void crtc_set_horiz_adj(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80010;
	*reg = v;
}

// R09: Raster number for raster interrupt
void crtc_set_raster_interrupt(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80012;
	*reg = v;
}

// R10 - R19: Scroll registers ===============================================

// R10: Text layer X scroll
void crtc_set_text_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80014;
	*reg = v;
}

// R11: Text layer Y scroll
void crtc_set_text_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80016;
	*reg = v;
}

// R12: Graphic layer 0 X scroll
void crtc_set_gp0_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80018;
	*reg = v;
}

// R13: Graphic layer 0 Y scroll
void crtc_set_gp0_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001A;
	*reg = v;
}

// R14: Graphic layer 1 X scroll
void crtc_set_gp1_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001C;
	*reg = v;
}

// R15: Graphic layer 1 Y scroll
void crtc_set_gp1_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8001E;
	*reg = v;
}

// R16: Graphic layer 2 X scroll
void crtc_set_gp2_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80020;
	*reg = v;
}

// R17: Graphic layer 2 Y scroll
void crtc_set_gp2_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80022;
	*reg = v;
}

// R18: Graphic layer 3 X scroll
void crtc_set_gp3_xscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80024;
	*reg = v;
}

// R19: Graphic layer 3 Y scroll
void crtc_set_gp3_yscroll(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80026;
	*reg = v;
}

// R20 - R23: Misc 2 =========================================================

// Set T-VRAM usage to display(0) or buffer(1)
void crtc_set_tvram_usage(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x1000);
	r20 |= (v ? 0x1000 : 0x0000);
	*reg = r20;
}

// Set G-VRAM usage to display(0) or buffer(1)
void crtc_set_gvram_usage(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x0800);
	r20 |= (v ? 0x0800 : 0x0000);
	*reg = r20;
}

// Set screen size to 512px(0) or 1024px(1)
void crtc_set_screen_size(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x0400);
	r20 |= (v ? 0x0400 : 0x0000);
	*reg = r20;
}

// Set color mode between 16(0), 256(1), and 65536(3)
void crtc_set_color_mode(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x0300);
	r20 |= (v & 0x0003) << 8;
	*reg = r20;
}

// Set horizontal deflection frequency between 15.98KHz(0) and 31.50KHz(1)
void crtc_set_horizontal_freq(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x0010);
	r20 |= (v ? 0x0010 : 0x0000);
	*reg = r20;
}

// Set vertical display size between 256(0), 512(1), or 1024i(2) if Hf=31.5Khz
void crtc_set_display_height(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x000C);
	r20 |= (v & 0x0003) << 2;
	*reg = r20;
}

// Set horizontal display size between 256(0), 512(1), 7868(2), or 50MHz(3)
void crtc_set_display_width(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE80028;
	r20 &= ~(0x0003);
	r20 |= (v & 0x0003);
	*reg = r20;
}

// Text screen access mask
void crtc_set_text_screen_access_mask(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0200);
	r21 |= (v ? 0x0200 : 0x0000);
	*reg = r21;
}

// Text screen simultaneous access
void crtc_set_text_screen_access(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0100);
	r21 |= (v ? 0x0100 : 0x0000);
	*reg = r21;
}

// Text screen simultaneous access plane selection AP3
void crtc_set_text_screen_access_ap3(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0080);
	r21 |= (v ? 0x0080 : 0x0000);
	*reg = r21;
}

// Text screen simultaneous access plane selection AP2
void crtc_set_text_screen_access_ap2(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0040);
	r21 |= (v ? 0x0040 : 0x0000);
	*reg = r21;
}

// Text screen simultaneous access plane selection AP1
void crtc_set_text_screen_access_ap1(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0020);
	r21 |= (v ? 0x0020 : 0x0000);
	*reg = r21;
}

// Text screen simultaneous access plane selection AP0
void crtc_set_text_screen_access_ap0(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0010);
	r21 |= (v ? 0x0010 : 0x0000);
	*reg = r21;
}

// Text layer raster copy target plane / gvram clear target page
void crtc_set_cp3(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0008);
	r21 |= (v ? 0x0008 : 0x0000);
	*reg = r21;
}

// Text layer raster copy target plane / gvram clear target page
void crtc_set_cp2(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0004);
	r21 |= (v ? 0x0004 : 0x0000);
	*reg = r21;
}

// Text layer raster copy target plane / gvram clear target page
void crtc_set_cp1(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0002);
	r21 |= (v ? 0x0002 : 0x0000);
	*reg = r21;
}

// Text layer raster copy target plane / gvram clear target page
void crtc_set_cp0(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002A;
	r21 &= ~(0x0001);
	r21 |= (v ? 0x0001 : 0x0000);
	*reg = r21;
}

// Set source raster
void crtc_set_copy_source(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002C;
	r22 &= 0x00FF;
	r22 |= ((uint16_t)v) << 8;
	*reg = r22;
}

// Set destination raster
void crtc_set_copy_destination(uint8_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002C;
	r22 &= 0xFF00;
	r22 |= v;
	*reg = r22;
}

// Set text simultaneous mask pattern
void crtc_set_text_mask_pattern(uint16_t v)
{
	volatile uint16_t *reg = (uint16_t *)0xE8002E;
	*reg = v;
}

// CRTC control port
void crtc_set_control(uint8_t v)
{
	volatile uint8_t *reg = (uint8_t *)0xE80481;
	*reg = v;
}
