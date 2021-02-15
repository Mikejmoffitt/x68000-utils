#include "x68000/x68k_pcg.h"

static uint16_t x68k_pcg_ctrl;
static volatile uint16_t *x68k_pcg_ctrl_r = (volatile uint16_t *)PCG_BG_CTRL;

static uint8_t s_spr_next = 0;
static uint8_t s_spr_count_prev = 0;

/*
Control:    0xEB0808
---- --9- ---- ---- D/C          turn on PCG to allow PCG register access
---- ---- --54 ---- BG1 TXsel    nametable mapping
---- ---- ---- 3--- BG1 ON
---- ---- ---- -21- BG0 TXsel    nametable mapping
---- ---- ---- ---0 BG0 ON
H-total:    0xEB080A
---- ---- 7654 3210 H total
H-disp:     0xEB080C
---- ---- --54 3210 H disp
V-disp:     0xEB080E
---- ---- 7654 3210 V disp
Mode:       0xEB0810
---- ---- ---4 ---- L/H          15KHz(0), 31Khz(1)
---- ---- ---- 32-- V-res
---- ---- ---- --10 H-res        If nonzero, enables 16x16 tiles

*/

void x68k_pcg_init(const X68kPcgConfig *c)
{
	volatile uint16_t *pcg_reg = (volatile uint16_t *)0xEB080A;
	pcg_reg[0] = c->htotal;
	pcg_reg[1] = c->hdisp;
	pcg_reg[2] = c->vdisp;
	pcg_reg[3] = c->flags;

	x68k_pcg_set_disp_en(0);
	x68k_pcg_set_bg0_txsel(0);
	x68k_pcg_set_bg1_txsel(1);
	x68k_pcg_set_bg0_enable(1);
	x68k_pcg_set_bg1_enable(1);
	x68k_pcg_set_bg0_xscroll(0);
	x68k_pcg_set_bg1_xscroll(0);
	x68k_pcg_set_bg0_yscroll(0);
	x68k_pcg_set_bg1_yscroll(0);
	x68k_pcg_clear_sprites();
	x68k_pcg_set_disp_en(1);
}

// Set to CPU(1) or display(0)
void x68k_pcg_set_disp_en(uint8_t en)
{
	x68k_pcg_ctrl &= ~(0x0200);
	x68k_pcg_ctrl |= (en ? 0x0200 : 0x0000);
	*x68k_pcg_ctrl_r = x68k_pcg_ctrl;
}

// Change the mappings for BG1 and BG0 nametables
void x68k_pcg_set_bg1_txsel(uint8_t t)
{
	x68k_pcg_ctrl &= ~(0x0030);
	x68k_pcg_ctrl |= (t & 0x03) << 4;
	*x68k_pcg_ctrl_r = x68k_pcg_ctrl;
}
void x68k_pcg_set_bg0_txsel(uint8_t t)
{
	
	x68k_pcg_ctrl &= ~(0x0030);
	x68k_pcg_ctrl |= (t & 0x03) << 1;
	*x68k_pcg_ctrl_r = x68k_pcg_ctrl;
}

// Enable or disable BG layer display
void x68k_pcg_set_bg1_enable(uint8_t en)
{
	x68k_pcg_ctrl &= ~(0x0008);
	x68k_pcg_ctrl |= (en ? 0x08 : 0x000);
	*x68k_pcg_ctrl_r = x68k_pcg_ctrl;
}
void x68k_pcg_set_bg0_enable(uint8_t en)
{
	x68k_pcg_ctrl &= ~(0x0001);
	x68k_pcg_ctrl |= (en ? 0x001 : 0x000);
	*x68k_pcg_ctrl_r = x68k_pcg_ctrl;
}

void x68k_pcg_add_sprite(int16_t x, int16_t y, uint16_t attr, uint16_t prio)
{
	while (s_spr_count_prev > 0)
	{
		s_spr_count_prev--;
		volatile X68kPcgSprite *spr = x68k_pcg_get_sprite(s_spr_count_prev);
		spr->x = 0;
	}
	if (s_spr_next >= 128) return;
	x68k_pcg_set_sprite(s_spr_next++, x, y, attr, prio);
}

void x68k_pcg_finish_sprites(void)
{
	s_spr_count_prev = s_spr_next;
	s_spr_next = 0;
}
