#ifndef PCG_H
#define PCG_H

#include <stdint.h>

/*

Register notes:

Control:    0xEB0808
---- --9- ---- ---- D/C - turn on PCG to allow PCG register access
---- ---- --54 ---- BG1 TXsel - nametable mapping
---- ---- ---- 3--- BG1 ON
---- ---- ---- -21- BG0 TXsel - nametable mapping
---- ---- ---- ---0 BG0 ON
H-total:    0xEB080A
---- ---- 7654 3210 H total
H-disp:     0xEB080C
---- ---- --54 3210 H disp
V-disp:     0xEB080E
---- ---- 7654 3210 V disp
Mode:       0xEB0810
---- ---- ---4 ---- L/H        15KHz (0), 31Khz (1)
---- ---- ---- 32-- V-res      256-line mode (0), 512-line mode (>=1)
---- ---- ---- --10 H-res      256-dot mode (0), 512-dot mode (>=1)

Tile size (8x8 or 16x16) is influenced by HRES in 0xEB0810
    Display mode    Screen size     Layer size          Tile size
    register HRES
    bits
    0 0             256 x 256 dots  512 x 512 dots      8 x 8 dots
    0 1             512 x 512 dots  1024 x 1024 dots    16 x 16 dots

Tile format:
f--- ---- ---- ---- y flip
-e-- ---- ---- ---- x flip
--dc ---- ---- ---- unused
---- ba98 ---- ---- color
---- ---- 7654 4310 pattern #

Tile indexing:

In 16x16 tile mode (hres on) tiles are "as you expect".
16x16 tiles are indexed within PCG memory like so:

00 -> 01 -> 02 -> 03 -> 04 ...

16 -> 17 -> 18 -> 19 -> 20 ...

In 8x8 tile mode (hres off) tiles are in a zig-zag order. They still follow
addressing rules of the 16x16 mode in a sense. 
8x8 tiles are indexed within PCG memory like so:

00  02  03  05  08
| / | / | / | / |
01  03  05  07  ...

64  66  68
| / | / |
65  67  ...

*/

// Memory map
#define PCG_SPR_TABLE  0xEB0000
#define PCG_TILE_DATA  0xEB8000
#define PCG_BG0_NAME   0xEBC000
#define PCG_BG1_NAME   0xEBE000
#define PCG_BG0_XSCRL  0xEB0800
#define PCG_BG0_YSCRL  0xEB0802
#define PCG_BG1_XSCRL  0xEB0806
#define PCG_BG1_YSCRL  0xEB0808
#define PCG_CTRL       0xEB0808
#define PCG_HTOTAL     0xEB080A
#define PCG_HDISP      0xEB080C
#define PCG_VDISP      0xEB080E
#define PCG_MODE       0xEB0810

// Attributes to specify sprite and backdrop tiles
#define PCG_ATTR(y,x,c,p) (((y&1)<<15) | ((x&1)<<14) |\
                          ((c&0xF)<<8) | ((p&0xFF)))

// Struct representing a sprite
typedef struct
{
	uint16_t x; // Limited to 10 bits of precision
	uint16_t y; // "
	uint16_t attr; // Bitfield; see PCG_ATTR macro
	uint16_t prio; // Priority relative to BG
} spr_t;

/* Priority between sprites and PCG backgrounds:

Sprite offset +06 is priority, or "PRW" as Inside calls it.

PRW     Layering
----------------
0 0     Sprites are not drawn.
0 1     Sprites are behind both tile layers.
1 0     Sprites are behind BG0, but in front of BG1.
1 1     Sprites are above both BG layers.

*/

// Setup =====================================================================

// Set up with some sane defaults.
void pcg_init_default(void);

// Set to CPU(1) or display(0)
void pcg_set_access(uint8_t en);

// Change the mappings for BG1 and BG0 nametables
void pcg_set_bg1_txsel(uint8_t t);
void pcg_set_bg0_txsel(uint8_t t);

// Enable or disable BG layer display
void pcg_set_bg1_enable(uint8_t en);
void pcg_set_bg0_enable(uint8_t en);

// Configure display timings; should correspond with CRTC.
static inline void pcg_set_htotal(uint8_t t)
{
	volatile uint16_t *r = (volatile uint16_t *)PCG_HTOTAL;
	*r = t;
}

static inline void pcg_set_hdisp(uint8_t t)
{
	volatile uint16_t *r = (volatile uint16_t *)PCG_HDISP;
	*r = t;
}

static inline void pcg_set_vdisp(uint8_t t)
{
	volatile uint16_t *r = (volatile uint16_t *)PCG_VDISP;
	*r = t;
}

// TODO: What does L/H represent? Low/high something? Might be used
// to indicate interlaced mode.
void pcg_set_lh(uint8_t lh);

// Vertical and horizontal resolution; seems related to sprite and tile size
void pcg_set_vres(uint8_t v);
void pcg_set_hres(uint8_t h);

// Data manipulation =========================================================

// Sets a tile. Not recommended for drawing a background or much else beyond
// small changes or playing around, as it's slower than doing a large DMA.
static inline void pcg_set_bg0_tile(uint16_t x, uint16_t y, uint16_t attr)
{
	volatile uint16_t *nt = (volatile uint16_t *)PCG_BG0_NAME;
	nt += x;
	nt += (y << 6);
	*nt = attr;
}

static inline void pcg_set_bg1_tile(uint16_t x, uint16_t y, uint16_t attr)
{
	volatile uint16_t *nt = (volatile uint16_t *)PCG_BG1_NAME;
	nt += x;
	nt += (y << 6);
	*nt = attr;
}

// Scroll registers
static inline void pcg_set_bg0_xscroll(uint16_t x)
{
	volatile uint16_t *scr = (volatile uint16_t *)PCG_BG0_XSCRL;
	*scr = x;
}

static inline void pcg_set_bg1_xscroll(uint16_t x)
{
	volatile uint16_t *scr = (volatile uint16_t *)PCG_BG1_XSCRL;
	*scr = x;
}

static inline void pcg_set_bg0_yscroll(uint16_t y)
{
	volatile uint16_t *scr = (volatile uint16_t *)PCG_BG0_YSCRL;
	*scr = y;
}

static inline void pcg_set_bg1_yscroll(uint16_t y)
{
	volatile uint16_t *scr = (volatile uint16_t *)PCG_BG1_YSCRL;
	*scr = y;
}

// Does pointer math to get a handle to a sprite entry
static inline volatile spr_t *pcg_get_sprite(uint8_t idx)
{
	volatile spr_t *ret = (volatile spr_t *)PCG_SPR_TABLE;
	ret += idx;
	return ret;
}

// Semi-pointless sprite setter function
static inline void pcg_set_sprite(uint8_t idx, uint16_t x,
                                  int16_t y, int16_t attr, uint16_t prio)
{
	volatile spr_t *spr = pcg_get_sprite(idx);
	spr->x = x+16;
	spr->y = y+16;
	spr->attr = attr;
	spr->prio = prio;
}

// Clears all sprites by setting priority to %00 (invisible)
static inline void pcg_clear_sprites(void)
{
	volatile spr_t *spr = pcg_get_sprite(0);
	uint8_t i = 0;
	for (i = 0; i < 128; i++)
	{
		spr[i].prio = 0x00;
	}
}

#endif
