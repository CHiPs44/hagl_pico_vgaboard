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

#ifndef PICO_VGABOARD_MODES_768X576_H
#define PICO_VGABOARD_MODES_768X576_H

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/* clang-format on */

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_768X576_FREQ_HZ        60
/* should be 34960000 (34.96 MHz) */
#define PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ (35000000L)
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My Pico reaches 280MHz at 1.20V! */
#define PICO_VGABOARD_768X576_SYS_CLOCK_KHZ  (8 * PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_768X576_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
#else
#define PICO_VGABOARD_768X576_SYS_CLOCK_KHZ  (6 * PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_768X576_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_768X576_SYS_CLOCK_KHZ  (6 * PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_768X576_VREG_VOLTAGE   0
#endif

/** @brief cf. http://tinyvga.com/vga-timing/768x576@60Hz */
const scanvideo_timing_t vga_timing_768x576_60_pico = {
    .clock_freq      = PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ,
    .h_active        = 768,
    .v_active        = 576,
    .h_front_porch   = 24,
    .h_pulse         = 80,
    .h_total         = 976,
    .h_sync_polarity = 0,
    .v_front_porch   = 1,
    .v_pulse         = 3,
    .v_total         = 597,
    .v_sync_polarity = 1,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_768X576(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_768x576_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 768 / (__xscale__),\
    .height         = 576 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_768x576_60_11_pico = SCANVIDEO_MODE_768X576(1, 1);
const scanvideo_mode_t pico_vga_mode_768x288_60_12_pico = SCANVIDEO_MODE_768X576(1, 2);
const scanvideo_mode_t pico_vga_mode_384x576_60_21_pico = SCANVIDEO_MODE_768X576(2, 1);
const scanvideo_mode_t pico_vga_mode_384x288_60_22_pico = SCANVIDEO_MODE_768X576(2, 2);
const scanvideo_mode_t pico_vga_mode_256x192_60_33_pico = SCANVIDEO_MODE_768X576(3, 3);
const scanvideo_mode_t pico_vga_mode_384x144_60_24_pico = SCANVIDEO_MODE_768X576(2, 4);
const scanvideo_mode_t pico_vga_mode_192x288_60_42_pico = SCANVIDEO_MODE_768X576(4, 2);
const scanvideo_mode_t pico_vga_mode_192x144_60_44_pico = SCANVIDEO_MODE_768X576(4, 4);

#define PICO_VGABOARD_768X576(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_768X576_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_768X576_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_768X576_VREG_VOLTAGE,\
}

/***************************/
/* 24576 BYTES FRAMEBUFFER */
/***************************/

/** @brief 256x192@60Hz, 4bpp, 16 colors, 27648 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_256x192x4bpp_24576_2 = PICO_VGABOARD_768X576(&pico_vga_mode_256x192_60_33_pico,  4, &pico_vgaboard_palette_4bpp_default);

/***************************/
/* 49152 BYTES FRAMEBUFFER */
/***************************/

/** @brief 256x192@60Hz, 4bpp, 256 colors, 49152 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_256x192x8bpp_49152_2 = PICO_VGABOARD_768X576(&pico_vga_mode_256x192_60_33_pico,  8, &pico_vgaboard_palette_8bpp_default);

/***************************/
/* 55296 BYTES FRAMEBUFFER */
/***************************/

/** @brief 768x576@60Hz, 1bpp, monochrome, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_768x576x1bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_768x576_60_11_pico,  1, &pico_vgaboard_palette_1bpp_default);
/** @brief 768x288@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_768x288x2bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_768x288_60_12_pico,  2, &pico_vgaboard_palette_2bpp_default);
/** @brief 384x576@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_384x576x2bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_384x576_60_21_pico,  2, &pico_vgaboard_palette_2bpp_default);
/** @brief 384x288@60Hz, 4bpp, 16 colors, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_384x288x4bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_384x288_60_22_pico,  4, &pico_vgaboard_palette_4bpp_default);
/** @brief 384x144@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_384x144x8bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_384x144_60_24_pico,  8, &pico_vgaboard_palette_8bpp_default);
/** @brief 192x288@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_192x288x8bpp  = PICO_VGABOARD_768X576(&pico_vga_mode_192x288_60_42_pico,  8, &pico_vgaboard_palette_8bpp_default);
/** @brief 192x144@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 55296 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_192x144x16bpp = PICO_VGABOARD_768X576(&pico_vga_mode_192x144_60_44_pico, 16, &pico_vgaboard_palette_16bpp_empty);

/****************************/
/* 110592 BYTES FRAMEBUFFER */
/****************************/

/** @brief 192x288@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 110592 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_192x288x16bpp = PICO_VGABOARD_768X576(&pico_vga_mode_192x288_60_42_pico, 16, &pico_vgaboard_palette_16bpp_empty);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_768X576_H */
