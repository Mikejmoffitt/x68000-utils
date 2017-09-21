#include "pcg.h"

static uint16_t pcg_mode, pcg_ctrl;
static volatile uint16_t *pcg_mode_r = (volatile uint16_t *)PCG_MODE;
static volatile uint16_t *pcg_ctrl_r = (volatile uint16_t *)PCG_CTRL;

/*
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
---- ---- ---4 ---- L/H  				15KHz(0), 31Khz(1)
---- ---- ---- 32-- V-res
---- ---- ---- --10 H-res				If nonzero, enables 16x16 tiles

*/

// Set up with some sane defaults.
void pcg_init_default(void)
{
	pcg_mode = 0;
	pcg_ctrl = 0;
	pcg_set_access(0); // Start off with CPU access, not displaying
	pcg_set_bg1_txsel(1);
	pcg_set_bg0_txsel(0);
	pcg_set_bg0_xscroll(0);
	pcg_set_bg0_yscroll(0);
	pcg_set_bg1_xscroll(0);
	pcg_set_bg1_yscroll(0);
	pcg_set_bg0_enable(1);
	pcg_set_bg1_enable(1);
	pcg_set_lh(1);
	pcg_set_vres(0);
	pcg_set_hres(1);
	pcg_clear_sprites();
	pcg_set_access(1);
}

// Set to CPU(1) or display(0)
void pcg_set_access(uint8_t en)
{
	pcg_ctrl &= ~(0x0200);
	pcg_ctrl |= (en ? 0x0200 : 0x0000);
	*pcg_ctrl_r = pcg_ctrl;
}

// Change the mappings for BG1 and BG0 nametables
void pcg_set_bg1_txsel(uint8_t t)
{
	pcg_ctrl &= ~(0x0030);
	pcg_ctrl |= (t & 0x03) << 4;
	*pcg_ctrl_r = pcg_ctrl;
}
void pcg_set_bg0_txsel(uint8_t t)
{
	
	pcg_ctrl &= ~(0x0030);
	pcg_ctrl |= (t & 0x03) << 1;
	*pcg_ctrl_r = pcg_ctrl;
}

// Enable or disable BG layer display
void pcg_set_bg1_enable(uint8_t en)
{
	pcg_ctrl &= ~(0x0008);
	pcg_ctrl |= (en ? 0x08 : 0x000);
	*pcg_ctrl_r = pcg_ctrl;
}
void pcg_set_bg0_enable(uint8_t en)
{
	pcg_ctrl &= ~(0x0001);
	pcg_ctrl |= (en ? 0x001 : 0x000);
	*pcg_ctrl_r = pcg_ctrl;
}

// TODO: What does L/H represent? Low/high something? Might be used
// to indicate interlaced mode.
void pcg_set_lh(uint8_t lh)
{
	pcg_mode &= ~(0x0010);
	pcg_mode |= (lh ? 0x10 : 0x00);
	*pcg_mode_r = pcg_mode;
}

// Vertical and horizontal resolution; seems related to sprite size
void pcg_set_vres(uint8_t v)
{
	pcg_mode &= ~(0x0000C);
	pcg_mode |= ((v & 0x03) << 2);
	*pcg_mode_r = pcg_mode;
}
void pcg_set_hres(uint8_t h)
{
	pcg_mode &= ~(0x00003);
	pcg_mode |= (h & 0x03);
	*pcg_mode_r = pcg_mode;
}
