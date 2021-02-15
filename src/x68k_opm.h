#ifndef _X68K_OPM_H
#define _X68K_OPM_H

#include <stdint.h>

#define OPM_BASE 0xE90000

#define OPM_ADDRESS (volatile uint8_t *)(OPM_BASE + 1)
#define OPM_DATA (volatile uint8_t *)(OPM_BASE + 3)
#define OPM_STATUS (volatile uint8_t *)(OPM_BASE + 3)

#define x68k_opm_status() (*OPM_STATUS)

#define x68k_opm_busy() ((*OPM_STATUS) & 0x80)
#define x68k_opm_timer_a() ((*OPM_STATUS) & 0x01)
#define x68k_opm_timer_b() ((*OPM_STATUS) & 0x02)

#define x68k_opm_write(address, data) do \
{ \
	while(x68k_opm_busy()) __asm__ volatile ("nop"); \
	*OPM_ADDRESS = (address); \
	while(x68k_opm_busy()) __asm__ volatile ("nop"); \
	*OPM_DATA = (data); \
} while(0);

typedef enum X68kOpmReg
{
	OPM_REG_TEST_MODE = 0x01,
	OPM_REG_KEY_ON = 0x08,
	OPM_REG_NOISE = 0x0F,
	OPM_REG_CLKA_UPPER = 0x10,
	OPM_REG_CLKA_LOWER = 0x11,
	OPM_REG_CLKB = 0x12,
	OPM_REG_TIMER_FLAGS = 0x14,
	OPM_REG_LFO_FREQ = 0x18,
	OPM_REG_LFO_DEPTH = 0x19,
	OPM_REG_CONTROL = 0x1B,
	// These registers are repeated 8 times each, for CH.A-CH.H.
	OPM_CH_PAN_FL_CON = 0x20,
	OPM_CH_OCT_NOTE = 0x28,
	OPM_CH_KF = 0x30,
	OPM_CH_PMS_AMS = 0x38,
	// These registers are repeated 8 times in an inner set for the channels,
	// and then externally four times for the four operators.
	// For example, 0x40 is CH.A OP1; 0x47 is CH.H OP1. 0x48 is CH.A OP2, etc.
	OPM_CH_DT1_MUL = 0x40,
	OPM_CH_TL = 0x60,
	OPM_CH_KS_AR = 0x80,
	OPM_CH_AME_D1R = 0xA0,
	OPM_CH_DT2_D2R = 0xC0,
	OPM_CH_D1L_RR = 0xE0,

	OPM_REG_MAX = 0xFF
} X68kOpmReg;

#define X68K_OPM_TIMER_FLAG_CSM       0x80
#define X68K_OPM_TIMER_FLAG_F_RESET_B 0x20
#define X68K_OPM_TIMER_FLAG_F_RESET_A 0x10
#define X68K_OPM_TIMER_FLAG_IRQ_EN_B  0x08
#define X68K_OPM_TIMER_FLAG_IRQ_EN_A  0x04
#define X68K_OPM_TIMER_FLAG_LOAD_B    0x02
#define X68K_OPM_TIMER_FLAG_LOAD_A    0x01
typedef enum X68kOpmLfoWave
{
	LFO_WAVE_SAW = 0x00,
	LFO_WAVE_RECTANGLE,
	LFO_WAVE_TRIANGLE,
	LFO_WAVE_RAND,
} X68kOpmLfoWave;

#define X68K_OPM_PAN_RIGHT_ENABLE 0x80
#define X68K_OPM_PAN_LEFT_ENABLE 0x40
#define X68K_OPM_PAN_BOTH_ENABLE (X68K_OPM_PAN_RIGHT_ENABLE | X68K_OPM_PAN_LEFT_ENABLE)

typedef enum X68kOpmNote
{
	OPM_NOTE_CS = 0x0,
	OPM_NOTE_D  = 0x1,
	OPM_NOTE_DS = 0x2,
	OPM_NOTE_E  = 0x4,
	OPM_NOTE_F  = 0x5,
	OPM_NOTE_FS = 0x6,
	OPM_NOTE_G  = 0x8,
	OPM_NOTE_GS = 0x9,
	OPM_NOTE_A  = 0xA,
	OPM_NOTE_AS = 0xC,
	OPM_NOTE_B  = 0xD,
	OPM_NOTE_C  = 0xE,
} X68kOpmNote;

static inline void x68k_opm_set_test_mode(uint8_t en)
{
	x68k_opm_write(OPM_REG_TEST_MODE, en ? 0x01 : 0x00);
}

static inline void x68k_opm_set_lfo_reset(uint8_t en)
{
	x68k_opm_write(OPM_REG_TEST_MODE, en ? 0x02 : 0x00);
}

// Channel: 0 - 7
// SN:     0 - F
static inline void x68k_opm_set_key_on(uint8_t channel, uint8_t sn)
{
	x68k_opm_write(OPM_REG_KEY_ON, channel | (sn << 3));
}

// fnoise(Hz) = 4MHZ / (32 * nfreq)
// Nfreq:   0 - 31
static inline void x68k_opm_set_noise(uint8_t en, uint8_t nfreq)
{
	x68k_opm_write(OPM_REG_NOISE, (en ? 0x80 : 0x00) | nfreq);
}

// Period:  0 - 1023
// Ta(sec) = (64 * (1024 - period)) / CLK
static inline void x68k_opm_set_clka_period(uint16_t period)
{
	x68k_opm_write(OPM_REG_CLKA_UPPER, period >> 2);
	x68k_opm_write(OPM_REG_CLKA_LOWER, period & 0x03);
}

// Period:  0 - 255
// Ta(sec) = (1024 * (256 - period)) / CLK
static inline void x68k_opm_set_clkb_period(uint8_t period)
{
	x68k_opm_write(OPM_REG_CLKB, period);
}
static inline void x68k_opm_set_timer_flags(uint8_t flags)
{
	x68k_opm_write(OPM_REG_TIMER_FLAGS, flags);
}

static inline void x68k_opm_set_lfo_freq(uint8_t freq)
{
	x68k_opm_write(OPM_REG_LFO_FREQ, freq);
}

// Depth:  0 - 127
static inline void x68k_opm_set_lfo_am_depth(uint8_t depth)
{
	x68k_opm_write(OPM_REG_LFO_DEPTH, depth);
}

// Depth:  0 - 127
static inline void x68k_opm_set_lfo_pm_depth(uint8_t depth)
{
	x68k_opm_write(OPM_REG_LFO_DEPTH, 0x80 | depth);
}

static inline void x68k_opm_set_control(uint8_t ct1_adpcm_8mhz,
                                        uint8_t ct2_fdc_ready,
                                        X68kOpmLfoWave lfo_wave)
{
	x68k_opm_write(OPM_REG_CONTROL, (ct1_adpcm_8mhz ? 0x80 : 0x00) |
	                                (ct2_fdc_ready ? 0x40 : 0x00) |
	                                lfo_wave);
}
// Channel Enable: Use X68K_OPM_PAN_*_ENABLE bitfield
// channel: 0 - 7
// FL:      0 - 7
// con:     0 - 7
static inline void x68k_opm_set_lr_fl_con(uint8_t channel, uint8_t pan, uint8_t fl,
                                          uint8_t con)
{
	x68k_opm_write(OPM_CH_PAN_FL_CON + channel, pan | (fl << 3) | con);
}

// Channel: 0 - 7
// Octave:  0 - 7
// Note:    X68kOpmNote values (0 - F)
static inline void x68k_opm_set_oct_note(uint8_t channel, uint8_t octave,
                                         X68kOpmNote note)
{
	x68k_opm_write(OPM_CH_OCT_NOTE + channel, note | (octave << 4 ));
}

// Channel:  0 - 7
// Fraction: 0 - 63
static inline void x68k_opm_set_key_fraction(uint8_t channel, uint8_t fraction)
{
	x68k_opm_write(OPM_CH_KF + channel, fraction << 2);
}

// Channel: 0 - 7
// PMS:     0 - 7
// AMS:     0 - 3
static inline void x68k_opm_set_pms_ams(uint8_t channel, uint8_t pms, uint8_t ams)
{
	x68k_opm_write(OPM_CH_PMS_AMS + channel, (pms << 4) | ams);
}

// Channel: 0 - 7
// Op:      0 - 7
// DT1:     0 - 7
// Mul:     0 - 7
static inline void x68k_opm_set_d1t_mul(uint8_t channel, uint8_t op,
                                        uint8_t dt1, uint8_t mul)
{
	x68k_opm_write(OPM_CH_DT1_MUL + channel + (8 * op), (dt1 << 4) | mul);
}

// Channel: 0 - 7
// Op:      0 - 7
// TL:      0 - 127
static inline void x68k_opm_set_tl(uint8_t channel, uint8_t op, uint8_t tl)
{
	x68k_opm_write(OPM_CH_TL + channel + (8 * op), tl);
}
// Channel: 0 - 7
// Op:      0 - 7
// KS:      0 - 3
// Ar:      0 - 31
static inline void x68k_opm_set_ks_ar(uint8_t channel, uint8_t op,
                                      uint8_t ks, uint8_t ar)
{
	x68k_opm_write(OPM_CH_KS_AR + channel + (8 * op), ar | (ks << 6));
}

// Channel: 0 - 7
// Op:      0 - 7
// AME:     0 - 1
// D1R:     0 - 31
static inline void x68k_opm_set_ame_d1r(uint8_t channel, uint8_t op,
                                        uint8_t ame, uint8_t d1r)
{
	x68k_opm_write(OPM_CH_AME_D1R + channel + (8 * op), (ame ? 0x80 : 0x00) | d1r);
}

// Channel: 0 - 7
// Op:      0 - 7
// DT2:     0 - 3
// D2R:     0 - 31
static inline void x68k_opm_set_dt2_d2r(uint8_t channel, uint8_t op,
                                        uint8_t dt2, uint8_t d2r)
{
	x68k_opm_write(OPM_CH_DT2_D2R + channel + (8 * op), (dt2 << 6) | d2r);
}

// Channel: 0 - 7
// Op:      0 - 7
// D1L:     0 - 15
// RR:      0 - 15
static inline void x68k_opm_set_d1l_rr(uint8_t channel, uint8_t op,
                                       uint8_t d1l, uint8_t rr)
{
	x68k_opm_write(OPM_CH_D1L_RR + channel + (8 * op), rr | (d1l << 4));
}

#endif  // _OPM_H
