#ifndef _X68K_SYNC_H
#define _X68K_SYNC_H

/* MFP address */
#define MFP_BASE  0xE88000

#include <stdint.h>

struct MFP
{
	uint16_t gpdr;      /* General Purpose I/O line Data Register (used as interrupt sources on x68000) */
	uint16_t aer;       /* Active Edge Register (The signal edge the interrupt will be trigerred on) */
	uint16_t ddr;       /* Data Direction Register (only input on x68000) */
	uint16_t iera;      /* Interrupt Enable Register A */
	uint16_t ierb;      /* Interrupt Enable Register B */
	uint16_t ipra;      /* Interrupt Pending Register A */
	uint16_t iprb;      /* Interrupt Pending Register B */
	uint16_t isra;      /* Interrupt In-Service Register A */
	uint16_t isrb;      /* Interrupt In-Service Register B */
	uint16_t imra;      /* Interrupt Mask Register A */
	uint16_t imrb;      /* Interrupt Mask Register B */
	uint16_t vr;        /* Vector Register */
	uint16_t tacr;
	uint16_t tbcr;
	uint16_t tcdcr;
	uint16_t tadr;
	uint16_t tbdr;
	uint16_t tcdr;
	uint16_t tddr;
	uint16_t scr;
	uint16_t ucr;
	uint16_t rsr;
	uint16_t tsr;
	uint16_t udr;
};
#define mfp ((*(volatile struct MFP *)MFP_BASE))

#define GPIP_ALARM    (1 << 0)
#define GPIP_EXPON    (1 << 1)
#define GPIP_POWSW    (1 << 2)
#define GPIP_OPMIRQ   (1 << 3)
#define GPIP_VDISP    (1 << 4)
#define GPIP_VSYNC    (1 << 6)
#define GPIP_HSYNC    (1 << 7)

static void inline x68k_wait_for_vsync(void)
{
	while (mfp.gpdr & GPIP_VSYNC);
	while (!(mfp.gpdr & GPIP_VSYNC));
}

static void inline x68k_vbl_wait_for_vblank(void)
{
	/* CRTC under vdisp */
	while (mfp.gpdr & GPIP_VDISP);
}

static void inline x68k_vbl_wait_for_vdisp(void)
{
	/* CRTC under vblank */
	while (!(mfp.gpdr & GPIP_VDISP));
}

#endif // _X68K_SYNC_H
