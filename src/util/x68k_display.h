// Utility functions for display mode management.
#ifndef X68K_DISPLAY_H
#define X68K_DISPLAY_H

#include <stdint.h>

#include "x68000/x68k_pcg.h"
#include "x68000/x68k_crtc.h"
#include "x68000/x68k_vidcon.h"

typedef struct X68kDisplayMode
{
	X68kCrtcConfig crtc;
	X68kPcgConfig pcg;
	X68kVidconConfig vidcon;
} X68kDisplayMode;

typedef struct X68kDisplay
{
	const X68kDisplayMode **modes;
	int num_modes;
	int current_mode;
} X68kDisplay;

// Initialize with a list of display modes. Mode 0 is applied to the video
// chipset.
void x68k_display_init(X68kDisplay *d, const X68kDisplayMode **modes,
                       int num_modes);

// Get the current display mode information.
const X68kDisplayMode *x68k_display_get_mode(const X68kDisplay *d);

// Go to the next display mode.
void x68k_display_cycle_mode(X68kDisplay *d);

#endif  // X68K_DISPLAY.H
