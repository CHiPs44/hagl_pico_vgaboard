/*

MIT License

Copyright (c) 2021-2024 CHiPs44 <chips44@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:

https://github.com/CHiPs44/hagl_pico_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef PICO_VGABOARD_MODES_1680X1050_H
#define PICO_VGABOARD_MODES_1680X1050_H

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/*
    cf. http://tinyvga.com/vga-timing/1680x1050@60Hz
    Other & different sources:
    cf. https://glenwing.github.io/docs/VESA-DMT-1.13.pdf pp. 79
    Modeline "1680x1050"x60.0  146.25  1680 1784 1960 2240  1050 1053 1059 1089 -hsync +vsync (65.3 kHz e)
    From my LG L204WT EDID:
        ...
        Detailed Timing Descriptors:
            DTD 1:  1680x1050   59.954 Hz   8:5    65.290 kHz 146.250 MHz (434 mm x 270 mm)
                        Hfront  104 Hsync 176 Hback 280 Hpol N
                        Vfront    3 Vsync   6 Vback  30 Vpol P
        Display Range Limits:
            Monitor ranges (GTF): 56-75 Hz V, 28-83 kHz H, max dotclock 150 MHz
            Display Product Name: 'L204WT'
        ...
*/

/* 147.14 MHz => 294.28 KO with vcocalc => 294 OK => 147 MHz */
#define PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ (147000000L)
#if !PICO_NO_HARDWARE
    #if PICO_RP2350
        /* RP2350 seems to be as overclockable as RP2040! */
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (3 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)
        #else
            #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (2 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #else
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            /* My B1 Pico reaches 294 MHz at 1.20V! */
            #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (2 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
        #else
            #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (1 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #endif
#else
    #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (1 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
    #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#define PICO_VGABOARD_1680X1050_FREQ_HZ        60

/** @brief cf. http://tinyvga.com/vga-timing/1680x1050@60Hz */
const scanvideo_timing_t vga_timing_1680x1050_60_pico =
{
    .clock_freq      = PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ,
    .h_active        = 1680,
    .h_front_porch   = 104,
    .h_pulse         = 184,
    .h_total         = 2256,
    .h_sync_polarity = 1, // NEGATIVE
    .v_active        = 1050,
    .v_front_porch   = 1,
    .v_pulse         = 3,
    .v_total         = 1087,
    .v_sync_polarity = 0, // POSITIVE
};

#define SCANVIDEO_MODE_1680X1050(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1680x1050_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1680 / (__xscale__),\
    .height         = 1050 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_840_525_60_pico_1 = SCANVIDEO_MODE_1680X1050(2, 2);
const scanvideo_mode_t pico_vga_mode_840_350_60_pico_1 = SCANVIDEO_MODE_1680X1050(2, 3);
const scanvideo_mode_t pico_vga_mode_560_525_60_pico_1 = SCANVIDEO_MODE_1680X1050(3, 2);
const scanvideo_mode_t pico_vga_mode_560_350_60_pico_1 = SCANVIDEO_MODE_1680X1050(3, 3);
const scanvideo_mode_t pico_vga_mode_336_210_60_pico_1 = SCANVIDEO_MODE_1680X1050(5, 5);
const scanvideo_mode_t pico_vga_mode_560_175_60_pico_1 = SCANVIDEO_MODE_1680X1050(3, 6);
const scanvideo_mode_t pico_vga_mode_280_350_60_pico_1 = SCANVIDEO_MODE_1680X1050(6, 3);

#define PICO_VGABOARD_1680x1050(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_1680X1050_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1680X1050_VREG_VOLTAGE,\
}



/***************************/
/* 24500 BYTES FRAMEBUFFER */
/***************************/

/** @brief 560x350@60Hz, 1bpp, 24500 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x350x1bpp = PICO_VGABOARD_1680x1050(&pico_vga_mode_560_350_60_pico_1, 1, &palette_1bpp_default);

/***************************/
/* 35280 BYTES FRAMEBUFFER */
/***************************/

/** @brief 336x210@60Hz, 4bpp, 35280 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_336x210x4bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_336_210_60_pico_1, 4, &palette_4bpp_default);

/***************************/
/* 36750 BYTES FRAMEBUFFER */
/***************************/

/** @brief 560x525@60Hz, 1bpp, 36750 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x525x1bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 1, &palette_1bpp_default);

/***************************/
/* 55125 BYTES FRAMEBUFFER */
/***************************/

/** @brief 840x525@60Hz, 1bpp, 55125 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_840x525x1bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 1, &palette_1bpp_default);

/***************************/
/* 70560 BYTES FRAMEBUFFER */
/***************************/

/** @brief 336x210@60Hz, 8bpp, 70560 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_336x210x8bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_336_210_60_pico_1, 8, &palette_8bpp_default);

/***************************/
/* 73500 BYTES FRAMEBUFFER */
/***************************/

/** @brief 560x525@60Hz, 1bpp, 73500 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x525x2bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 2, &palette_2bpp_default);

/***************************/
/* 98000 BYTES FRAMEBUFFER */
/***************************/

/** @brief 280x350@60Hz, 8bpp, 98000 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_280x350x8bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_280_350_60_pico_1, 8, &palette_8bpp_default);
/** @brief 560x175@60Hz, 8bpp, 98000 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x175x8bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_560_175_60_pico_1, 8, &palette_8bpp_default);
/** @brief 560x350@60Hz, 4bpp, 98000 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x350x4bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_560_350_60_pico_1, 4, &palette_4bpp_default);

/****************************/
/* 110250 BYTES FRAMEBUFFER */
/****************************/

/** @brief 840x525@60Hz, 2bpp, 110250 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_840x525x2bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 2, &palette_2bpp_default);

/****************************/
/* 196000 BYTES FRAMEBUFFER */
/****************************/

/** @brief 560x350@60Hz, 8bpp, 196000 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_560x350x8bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_560_350_60_pico_1, 8, &palette_8bpp_default);

/***************************/
/* 220500 BYTES FRAMEBUFFER */
/***************************/

/** @brief 840x525@60Hz, 4bpp, 220500 bytes, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_840x525x4bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 4, &palette_4bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1680X1050_H */
