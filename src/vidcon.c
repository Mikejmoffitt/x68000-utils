#include "vidcon.h"

// Register copies
static uint16_t regs[3];

// Set up a sane default configuration
void vidcon_init_default(void)
{
	regs[0] = 0x0000; // 512x512, 16-color
	regs[1] = 0x06E4; // Sprites > Text > GP0 > GP1 > GP2 > GP3
	regs[2] = 0x007F; // Enable all graphics layers.
}

// Update hardware registers from shadow copies
void vidcon_commit_regs(void)
{
	volatile uint16_t *r0 = (volatile uint16_t *)0xE82400;
	volatile uint16_t *r1 = (volatile uint16_t *)0xE82500;
	volatile uint16_t *r2 = (volatile uint16_t *)0xE82600;

	*r0 = regs[0];
	*r1 = regs[1];
	*r2 = regs[2];
}

// R0 config: screen

// Set screen size between 512px (0) and 1024px(1)
void vidcon_set_screen_size(uint8_t screen_size)
{
	regs[0] &= ~(0x0004);
	regs[0] |= screen_size ? 0x0004 : 0x0000;
}

// Set color mode between 16(0), 256(1), and 65536(3)
void vidcon_set_color_mode(uint8_t color_mode)
{
	regs[0] &= ~(0x0003);
	regs[0] |= color_mode & 0x0003;
}

// R1 config: priorities

// Set priorities of the graphics layers, where 0 is the topmost
void vidcon_set_prio_sprites(uint8_t priority)
{
	regs[1] &= ~(0x3000);
	regs[1] |= (priority & 0x0003) << 12;
}

void vidcon_set_prio_text(uint8_t priority)
{
	regs[1] &= ~(0x0C00);
	regs[1] |= (priority & 0x0003) << 10;
}
void vidcon_set_prio_graphics(uint8_t priority)
{
	regs[1] &= ~(0x0300);
	regs[1] |= (priority & 0x0003) << 8;
}

// Set priorities within GVRAM-based layers
void vidcon_set_prio_gp0(uint8_t priority)
{
	regs[1] &= ~(0x0003);
	regs[1] |= (priority & 0x0003);
}

void vidcon_set_prio_gp1(uint8_t priority)
{
	regs[1] &= ~(0x000C);
	regs[1] |= (priority & 0x0003) << 2;
}

void vidcon_set_prio_gp2(uint8_t priority)
{
	regs[1] &= ~(0x0030);
	regs[1] |= (priority & 0x0003) << 4;
}

void vidcon_set_prio_gp3(uint8_t priority)
{
	regs[1] &= ~(0x00C0);
	regs[1] |= (priority & 0x0003) << 6;
}

// R2 config: misc. settings, enable masks

// Do not display image even when superimposing
void vidcon_set_video_cut(uint8_t cut_en)
{
	regs[2] &= ~(0x8000);
	regs[2] |= (cut_en ? 0x8000 : 0x0000);
}

// Set translucent: text palette 0 (force translucent regardless of exon, etc)
void vidcon_set_ah(uint8_t ah_en)
{
	regs[2] &= ~(0x4000);
	regs[2] |= (ah_en ? 0x4000 : 0x0000);
}

// Set translucent: video image (used in color image unit)
void vidcon_set_vht(uint8_t vht_en)
{
	regs[2] &= ~(0x2000);
	regs[2] |= (vht_en ? 0x2000 : 0x0000);
}

// Enable special priority / translucent mode
void vidcon_set_exon(uint8_t exon_en)
{
	regs[2] &= ~(0x1000);
	regs[2] |= (exon_en ? 0x1000 : 0x0000);
}

// Special priority (0) or translucent mode (1) selection
void vidcon_set_hp(uint8_t hp)
{
	regs[2] &= ~(0x0800);
	regs[2] |= (hp ? 0x1000 : 0x0000);
}

// Sharp reserved(0) or something else(1)
void vidcon_set_bp(uint8_t bp)
{
	regs[2] &= ~(0x0400);
	regs[2] |= (bp ? 0x0400 : 0x0000);
}

// Set translucent: graphic screen
void vidcon_set_gg(uint8_t gg_en)
{
	regs[2] &= ~(0x0200);
	regs[2] |= (gg_en ? 0x0200 : 0x0000);
}

// Set translucent: text/sprite screen
void vidcon_set_gt(uint8_t gt_en)
{
	regs[2] &= ~(0x0100);
	regs[2] |= (gt_en ? 0x0100 : 0x0000);
}

// Enable border color display
void vidcon_border_enable(uint8_t en)
{
	regs[2] &= ~(0x0080);
	regs[2] |= (en ? 0x0080 : 0x0000);
}

// Enable sprite display
void vidcon_sprite_enable(uint8_t en)
{
	regs[2] &= ~(0x0040);
	regs[2] |= (en ? 0x0040 : 0x0000);
}

// Enable text plane display
void vidcon_text_enable(uint8_t en)
{
	regs[2] &= ~(0x0020);
	regs[2] |= (en ? 0x0020 : 0x0000);
}

// Enable graphic plane display
void vidcon_graphic_enable(uint8_t en)
{
	regs[2] &= ~(0x0010);
	regs[2] |= (en ? 0x0010 : 0x0000);
}

// Enable particular graphic layers
void vidcon_graphic_gp0_enable(uint8_t en)
{
	regs[2] &= ~(0x0001);
	regs[2] |= (en ? 0x0001 : 0x0000);
}
void vidcon_graphic_gp1_enable(uint8_t en)
{
	regs[2] &= ~(0x0002);
	regs[2] |= (en ? 0x0002 : 0x0000);
}
void vidcon_graphic_gp2_enable(uint8_t en)
{
	regs[2] &= ~(0x0004);
	regs[2] |= (en ? 0x0004 : 0x0000);
}
void vidcon_graphic_gp3_enable(uint8_t en)
{
	regs[2] &= ~(0x0008);
	regs[2] |= (en ? 0x0008 : 0x0000);
}
