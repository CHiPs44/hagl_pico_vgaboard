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

#ifndef PICO_VGABOARD_MODES_1600X900_H
#define PICO_VGABOARD_MODES_1600X900_H

/* clang-format off */

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

// cf. https://projectf.io/posts/video-timings-vga-720p-1080p/#hd-1600x900-60-hz

// Spec says 74.250 MHz, vcocalc says 3 or 4 times 74.000 is feasible
#define PICO_VGABOARD_1600X900_PIXEL_CLOCK_HZ 108000000L
#define PICO_VGABOARD_1600X900_FREQ_HZ        60
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My Pico B0 does not reach 325 MHz and is flaky at 296, even at 1.30V! */
/* My Pico B1 does reach 296 MHz at 1.30V */
#define PICO_VGABOARD_1600X900_SYS_CLOCK_KHZ  (2L * PICO_VGABOARD_1600X900_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1600X900_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
#else
#define PICO_VGABOARD_1600X900_SYS_CLOCK_KHZ  (2L * PICO_VGABOARD_1600X900_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1600X900_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_1600X900_SYS_CLOCK_KHZ  (2L * PICO_VGABOARD_1600X900_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1600X900_VREG_VOLTAGE   0
#endif

/*  1600x900 (0x6c) 108.000MHz +HSync +VSync
        h: width  1600 start 1624 end 1704 total 1800 skew    0 clock  60.00KHz
        v: height  900 start  901 end  904 total 1000           clock  60.00Hz
*/
const scanvideo_timing_t vga_timing_1600x900_60_pico = {
    .clock_freq      = PICO_VGABOARD_1600X900_PIXEL_CLOCK_HZ,
    .h_active        = 1600,
    .v_active        = 900,
    .h_front_porch   = 1624 - 1600,
    .h_pulse         = 1704 - 1624,
    .h_total         = 1800,
    .h_sync_polarity = 0,
    .v_front_porch   = 901 - 900,
    .v_pulse         = 904 - 901,
    .v_total         = 1000,
    .v_sync_polarity = 0,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1600X900(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1600x900_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1600 / (__xscale__),\
    .height         = 900 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1600x900_60_pico = SCANVIDEO_MODE_1600X900(1, 1);
const scanvideo_mode_t pico_vga_mode_800x450_60_pico  = SCANVIDEO_MODE_1600X900(2, 2);
const scanvideo_mode_t pico_vga_mode_400x225_60_pico  = SCANVIDEO_MODE_1600X900(4, 4);

#define PICO_VGABOARD_1600x900(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = PICO_VGABOARD_1600X900_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = PICO_VGABOARD_1600X900_SYS_CLOCK_KHZ,\
    .vreg_voltage = PICO_VGABOARD_1600X900_VREG_VOLTAGE,\
}

/***************************/
/* 45000 BYTES FRAMEBUFFER */
/***************************/

/** @brief 400x225@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_400x225x4bpp  = PICO_VGABOARD_1600x900(&pico_vga_mode_400x225_60_pico ,  4, &palette_4bpp_default);

/***************************/
/* 90000 BYTES FRAMEBUFFER */
/***************************/

/** @brief 400x225@60Hz, 8bpp, 256 colors */
const pico_vgaboard_t pico_vgaboard_400x225x8bpp  = PICO_VGABOARD_1600x900(&pico_vga_mode_400x225_60_pico ,  8, &palette_8bpp_default);

/** @brief 800x450@60Hz, 2bpp, 4 colors */
const pico_vgaboard_t pico_vgaboard_800x450x2bpp  = PICO_VGABOARD_1600x900(&pico_vga_mode_800x450_60_pico ,  4, &palette_2bpp_default);

/****************************/
/* 180000 BYTES FRAMEBUFFER */
/****************************/

/** @brief 1600x900@60Hz, 1bpp, 2 colors */
const pico_vgaboard_t pico_vgaboard_1600x900x1bpp = PICO_VGABOARD_1600x900(&pico_vga_mode_1600x900_60_pico,  1, &palette_1bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1600X900_H */
