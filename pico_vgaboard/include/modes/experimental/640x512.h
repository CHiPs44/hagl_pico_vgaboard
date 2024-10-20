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

#ifndef PICO_VGABOARD_MODES_640X512_H
#define PICO_VGABOARD_MODES_640X512_H

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#else
#define VREG_VOLTAGE_DEFAULT 0
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#define PICO_VGABOARD_640X512_FREQ_HZ        60
#define PICO_VGABOARD_640X512_PIXEL_CLOCK_HZ (25500000L / 4L)
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
#define PICO_VGABOARD_640X512_SYS_CLOCK_KHZ  (12 * PICO_VGABOARD_640X512_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X512_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
#else
#define PICO_VGABOARD_640X512_SYS_CLOCK_KHZ  (10 * PICO_VGABOARD_640X512_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X512_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_640X512_SYS_CLOCK_KHZ  (10 * PICO_VGABOARD_640X512_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X512_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif

/** @brief CVT from https://tomverbeure.github.io/video_timings_calculator */
const scanvideo_timing_t vga_timing_640X512_60_pico_2 =
{
    .clock_freq      = PICO_VGABOARD_640X512_PIXEL_CLOCK_HZ,
    .h_active        = 640,
    .h_front_porch   = 16,
    .h_pulse         = 64,
    .h_total         = 800,
    .h_sync_polarity = 1,
    .v_active        = 512,
    .v_front_porch   = 3, 
    .v_pulse         = 7, 
    .v_total         = 533,
    .v_sync_polarity = 0,
};

#define SCANVIDEO_MODE_640X512(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_640X512_60_pico_2,\
    .pio_program    = &video_24mhz_composable,\
    .width          = (1280 / 2) / (__xscale__),\
    .height         = (1024 / 2) / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_640x512_60_pico_2 = SCANVIDEO_MODE_640X512(1, 1);
const scanvideo_mode_t pico_vga_mode_640x256_60_pico_2 = SCANVIDEO_MODE_640X512(1, 2);
const scanvideo_mode_t pico_vga_mode_320x512_60_pico_2 = SCANVIDEO_MODE_640X512(2, 1);
const scanvideo_mode_t pico_vga_mode_320x256_60_pico_2 = SCANVIDEO_MODE_640X512(2, 2);
const scanvideo_mode_t pico_vga_mode_160x256_60_pico_2 = SCANVIDEO_MODE_640X512(4, 2);
const scanvideo_mode_t pico_vga_mode_320x128_60_pico_2 = SCANVIDEO_MODE_640X512(2, 4);
const scanvideo_mode_t pico_vga_mode_160x128_60_pico_2 = SCANVIDEO_MODE_640X512(4, 4);

#define PICO_VGABOARD_640X512(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = PICO_VGABOARD_640X512_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = PICO_VGABOARD_640X512_SYS_CLOCK_KHZ,\
    .vreg_voltage = PICO_VGABOARD_640X512_VREG_VOLTAGE,\
}

/***************************/
/* 40960 BYTES FRAMEBUFFER */
/***************************/

/** @brief 640x512@60Hz, 1bpp, monochrome, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_640x512x1bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_640x512_60_pico_2,  1, &palette_1bpp_default);
/** @brief 640x256@60Hz, 2bpp, 4 colors, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_640x256x2bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_640x256_60_pico_2,  2, &palette_2bpp_default);
/** @brief 320x256@60Hz, 4bpp, 16 colors, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_320x256x4bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_320x256_60_pico_2,  4, &palette_4bpp_default);
/** @brief 320x128@60Hz, 8bpp, 256 colors, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_320x128x8bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_320x128_60_pico_2,  8, &palette_8bpp_default);
/** @brief 160x256@60Hz, 8bpp, 256 colors, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_160x256x8bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_160x256_60_pico_2,  8, &palette_8bpp_default);
/** @brief 160x128@60Hz, 16bpp, 32768 colors, 40960 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_160x128x8bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_160x128_60_pico_2, 16, &palette_16bpp_empty);

/***************************/
/* 81920 BYTES FRAMEBUFFER */
/***************************/

/** @brief 320x256@60Hz, 8bpp, 256 colors, 81920 bytes framebuffer */
const pico_vgaboard_t pico_vgaboard_320x256x8bpp_2 = PICO_VGABOARD_640X512(&pico_vga_mode_320x256_60_pico_2,  8, &palette_8bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_640X512_H */
