#include "util/xt_display.h"
#include "x68000/x68k_vbl.h"
#include <iocs.h>

static void apply_mode(const X68kDisplayMode *mode)
{
	x68k_crtc_init(&mode->crtc);
	x68k_vidcon_init(&mode->vidcon);
	x68k_pcg_init(&mode->pcg);
}

// Initialize with a list of display modes. Mode 0 is applied to the video
// chipset.
void x68k_display_init(X68kDisplay *d, const X68kDisplayMode **modes,
                     int16_t num_modes)
{
	d->modes = modes;
	d->num_modes = num_modes;
	d->current_mode = 0;

	apply_mode(d->modes[0]);
}

const X68kDisplayMode *x68k_display_get_mode(const X68kDisplay *d)
{
	return d->modes[d->current_mode];
}

// Go to the next display mode.
void x68k_display_cycle_mode(X68kDisplay *d)
{
	d->current_mode++;
	if (d->current_mode >= d->num_modes)
	{
		d->current_mode = 0;
	}
	apply_mode(d->modes[d->current_mode]);
}
