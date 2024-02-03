/*

MIT License

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

#ifndef PICO_VGABOARD_MODES_1024X768_H
#define PICO_VGABOARD_MODES_1024X768_H

/* clang-format off */

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_1024X768_FREQ_HZ        60
#define PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ 65000000L
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My Pico does not reach 325 MHz, even at 1.30V! */
// #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (5 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
// #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
#define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#else
#define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1024X768_VREG_VOLTAGE   0
#endif

/**
 * @brief VGA timings for 1024x768@60Hz
 * cf. https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466 (from @kilograham)
 * NB: according to http://tinyvga.com/vga-timing/1024x768@60Hz, h-sync and v-sync polarities are negative
 */
const scanvideo_timing_t vga_timing_1024x768_60_pico = {
    .clock_freq      = PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ,
    .h_active        = 1024,
    .v_active        = 768,
    .h_front_porch   = 24,
    .h_pulse         = 136,
    .h_total         = 1344,
    .h_sync_polarity = 0,
    .v_front_porch   = 3,
    .v_pulse         = 6,
    .v_total         = 806,
    .v_sync_polarity = 0,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1024x768(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1024x768_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1024 / (__xscale__),\
    .height         = 768 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1024x768_60_pico = SCANVIDEO_MODE_1024x768(1, 1);
const scanvideo_mode_t pico_vga_mode_1024x384_60_pico = SCANVIDEO_MODE_1024x768(1, 2);
const scanvideo_mode_t pico_vga_mode_512x768_60_pico  = SCANVIDEO_MODE_1024x768(2, 1);
const scanvideo_mode_t pico_vga_mode_512x384_60_pico  = SCANVIDEO_MODE_1024x768(2, 2);
const scanvideo_mode_t pico_vga_mode_512x192_60_pico  = SCANVIDEO_MODE_1024x768(2, 4);
const scanvideo_mode_t pico_vga_mode_256x384_60_pico  = SCANVIDEO_MODE_1024x768(4, 2);
const scanvideo_mode_t pico_vga_mode_256x192_60_pico  = SCANVIDEO_MODE_1024x768(4, 4);

#define PICO_VGABOARD_1024x768(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_1024X768_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1024X768_VREG_VOLTAGE\
}

/***************************/
/* 24576 BYTES FRAMEBUFFER */
/***************************/

/** @brief 512x384@60Hz, 1bpp, 2 colors, 24576 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_512x384x1bpp_24576   = PICO_VGABOARD_1024x768(&pico_vga_mode_512x384_60_pico, 1, &pico_vgaboard_palette_1bpp_default);
/** @brief 256x192@60Hz, 4bpp, 16 colors, 24576 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_256x192x4bpp_24576_1 = PICO_VGABOARD_1024x768(&pico_vga_mode_256x192_60_pico, 4, &pico_vgaboard_palette_4bpp_default);

/***************************/
/* 49152 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1024x384@60Hz, 1bpp, monochrome, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_1024x384x1bpp  = PICO_VGABOARD_1024x768(&pico_vga_mode_1024x768_60_pico,  1, &pico_vgaboard_palette_1bpp_default);
/** @brief 512x768@60Hz, 1bpp, monochrome, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_512x768x1bpp   = PICO_VGABOARD_1024x768(&pico_vga_mode_512x768_60_pico ,  1, &pico_vgaboard_palette_1bpp_default);
/** @brief 512x384@60Hz, 2bpp, 4 colors, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_512x384x2bpp   = PICO_VGABOARD_1024x768(&pico_vga_mode_512x384_60_pico ,  2, &pico_vgaboard_palette_2bpp_default);
/** @brief 512x192@60Hz, 4bpp, 16 colors, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_512x192x4bpp   = PICO_VGABOARD_1024x768(&pico_vga_mode_512x192_60_pico ,  4, &pico_vgaboard_palette_4bpp_default);
/** @brief 256x384@60Hz, 4bpp, 16 colors, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_256x384x4bpp   = PICO_VGABOARD_1024x768(&pico_vga_mode_256x384_60_pico ,  4, &pico_vgaboard_palette_4bpp_default);
/** @brief 256x192@60Hz, 8bpp, 256 colors, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_256x192x8bpp_1 = PICO_VGABOARD_1024x768(&pico_vga_mode_256x192_60_pico ,  8, &pico_vgaboard_palette_8bpp_default);

/***************************/
/* 98304 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1024x768@60Hz, 1bpp, monochrome, 98304 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_1024x768x1bpp_98304 = PICO_VGABOARD_1024x768(&pico_vga_mode_1024x768_60_pico, 1, &pico_vgaboard_palette_1bpp_default);
/** @brief 512x384@60Hz, 4bpp, 16 colors, 98304 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_512x384x4bpp_98304  = PICO_VGABOARD_1024x768(&pico_vga_mode_512x384_60_pico , 4, &pico_vgaboard_palette_4bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1024X768_H */
