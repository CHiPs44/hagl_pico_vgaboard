/*

MIT License

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

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

#ifndef _HAGL_PICO_VGABOARD_MODES_720X408_TIC80_H
#define _HAGL_PICO_VGABOARD_MODES_720X408_TIC80_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/* cf. http://tinyvga.com/vga-timing/768x576@60Hz */

#define VGABOARD_720X408_PIXEL_CLOCK_HZ (35000000L)
#define VGABOARD_720X408_SYS_CLOCK_KHZ  (6 * VGABOARD_768X576_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_720X408_FREQ_HZ 60

// 240x136 * 3 => 720x408 / 720X408

const scanvideo_timing_t vga_timing_720x408_60_tic80 = {
    .clock_freq = VGABOARD_768X576_PIXEL_CLOCK_HZ,
    .h_active = 768,
    .v_active = 576,
    .h_front_porch = 24,
    .h_pulse = 80,
    .h_total = 976,
    .h_sync_polarity = 0,
    .v_front_porch = 1,
    .v_pulse = 3,
    .v_total = 597,
    .v_sync_polarity = 1,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

#define SCANVIDEO_MODE_768X576_TIC80(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_768x576_60_chips44,\
    .pio_program = &video_24mhz_composable,\
    .width = 768,\
    .height = 576,\
    .xscale = (__xscale__),\
    .yscale = (__yscale__),\
}

const scanvideo_mode_t vga_mode_256x192_60_33_chips44 = SCANVIDEO_MODE_768X576_TIC80(3, 3);

#define VGABOARD_768X576(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = VGABOARD_768X576_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = VGABOARD_768X576_SYS_CLOCK_KHZ,\
}

/***************************/
/* 24576 BYTES FRAMEBUFFER */
/***************************/

/** @brief 256x192@60Hz, 4bpp, 16 colors, 27648 bytes framebuffer */
const vgaboard_t vgaboard_256x192x4bpp_24576_2 = VGABOARD_768X576(&vga_mode_256x192_60_33_chips44,  4, &vgaboard_palette_4bpp_default);

/***************************/
/* 49152 BYTES FRAMEBUFFER */
/***************************/

/** @brief 256x192@60Hz, 4bpp, 256 colors, 49152 bytes framebuffer */
const vgaboard_t vgaboard_256x192x8bpp_49152_2 = VGABOARD_768X576(&vga_mode_256x192_60_33_chips44,  8, &vgaboard_palette_8bpp_default);

/***************************/
/* 55296 BYTES FRAMEBUFFER */
/***************************/

/** @brief 768x576@60Hz, 1bpp, monochrome, 55296 bytes framebuffer */
const vgaboard_t vgaboard_768x576x1bpp  = VGABOARD_768X576(&vga_mode_768x576_60_11_chips44,  1, &vgaboard_palette_1bpp_default);
/** @brief 768x288@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_768x288x2bpp  = VGABOARD_768X576(&vga_mode_768x288_60_12_chips44,  2, &vgaboard_palette_2bpp_default);
/** @brief 384x576@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x576x2bpp  = VGABOARD_768X576(&vga_mode_384x576_60_21_chips44,  2, &vgaboard_palette_2bpp_default);
/** @brief 384x288@60Hz, 4bpp, 16 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x288x4bpp  = VGABOARD_768X576(&vga_mode_384x288_60_22_chips44,  4, &vgaboard_palette_4bpp_default);
/** @brief 384x144@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x144x8bpp  = VGABOARD_768X576(&vga_mode_384x144_60_24_chips44,  8, &vgaboard_palette_8bpp_default);
/** @brief 192x288@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_192x288x8bpp  = VGABOARD_768X576(&vga_mode_192x288_60_42_chips44,  8, &vgaboard_palette_8bpp_default);
/** @brief 192x144@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 55296 bytes framebuffer */
const vgaboard_t vgaboard_192x144x16bpp = VGABOARD_768X576(&vga_mode_192x144_60_44_chips44, 16, &vgaboard_palette_16bpp_empty);

/****************************/
/* 110592 BYTES FRAMEBUFFER */
/****************************/

/** @brief 192x288@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 110592 bytes framebuffer */
const vgaboard_t vgaboard_192x288x16bpp_110592 = VGABOARD_768X576(&vga_mode_192x288_60_42_chips44, 16, &vgaboard_palette_16bpp_empty);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_720X408_TIC80_H */
