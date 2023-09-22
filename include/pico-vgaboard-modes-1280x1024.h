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

#ifndef PICO_VGABOARD_MODES_1280X1024_H
#define PICO_VGABOARD_MODES_1280X1024_H

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

#define PICO_VGABOARD_1280X1024_FREQ_HZ        60
#define PICO_VGABOARD_1280X1024_PIXEL_CLOCK_HZ (108000000L)
#define PICO_VGABOARD_1280X1024_SYS_CLOCK_KHZ  (2 * PICO_VGABOARD_1280X1024_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1280X1024_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
// 108 * 3 = 324 MHz => KO on my Pico, even at 1.30V
// #define PICO_VGABOARD_1280X1024_SYS_CLOCK_KHZ  (3 * PICO_VGABOARD_1280X1024_PIXEL_CLOCK_HZ / 1000L)
// #define PICO_VGABOARD_1280X1024_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)

/** @brief Copy of SDK */
const scanvideo_timing_t vga_timing_1280x1024_60_pico =
{
    .clock_freq      = PICO_VGABOARD_1280X1024_PIXEL_CLOCK_HZ,
    .h_active        = 1280,
    .v_active        = 1024,
    .h_front_porch   = 48,
    .h_pulse         = 112,
    .h_total         = 1688,
    .h_sync_polarity = 0,
    .v_front_porch   = 1,
    .v_pulse         = 3,
    .v_total         = 1066,
    .v_sync_polarity = 0,
};

#define SCANVIDEO_MODE_1280X1024(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1280x1024_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1280 / (__xscale__),\
    .height         = 1024 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

// const scanvideo_mode_t pico_vga_mode_1280x1024_60_pico = SCANVIDEO_MODE_1280X1024(1, 1);
// const scanvideo_mode_t pico_vga_mode_1280x512_60_pico  = SCANVIDEO_MODE_1280X1024(1, 2);
// const scanvideo_mode_t pico_vga_mode_640x1024_60_pico  = SCANVIDEO_MODE_1280X1024(2, 1);
const scanvideo_mode_t pico_vga_mode_640x512_60_pico = SCANVIDEO_MODE_1280X1024(2, 2);
const scanvideo_mode_t pico_vga_mode_640x256_60_pico = SCANVIDEO_MODE_1280X1024(2, 4);
const scanvideo_mode_t pico_vga_mode_320x256_60_pico = SCANVIDEO_MODE_1280X1024(4, 4);
const scanvideo_mode_t pico_vga_mode_160x256_60_pico = SCANVIDEO_MODE_1280X1024(8, 4);

#define PICO_VGABOARD_1280x1024(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = PICO_VGABOARD_1280X1024_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = PICO_VGABOARD_1280X1024_SYS_CLOCK_KHZ,\
    .vreg_voltage = PICO_VGABOARD_1280X1024_VREG_VOLTAGE,\
}

/***************************/
/* 40960 BYTES FRAMEBUFFER */
/***************************/

/** @brief 640x512@60Hz, 1bpp, monochrome */
const pico_vgaboard_t pico_vgaboard_640x512x1bpp = PICO_VGABOARD_1280x1024(&pico_vga_mode_640x512_60_pico, 1, &pico_vgaboard_palette_1bpp_default);
/** @brief 640x256@60Hz, 2bpp, 4 colors */
const pico_vgaboard_t pico_vgaboard_640x256x2bpp = PICO_VGABOARD_1280x1024(&pico_vga_mode_640x256_60_pico, 2, &pico_vgaboard_palette_2bpp_default);
/** @brief 320x256@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_320x256x4bpp = PICO_VGABOARD_1280x1024(&pico_vga_mode_320x256_60_pico, 4, &pico_vgaboard_palette_4bpp_default);
/** @brief 160x256@60Hz, 8bpp, 256 colors */
const pico_vgaboard_t pico_vgaboard_160x256x8bpp = PICO_VGABOARD_1280x1024(&pico_vga_mode_160x256_60_pico, 4, &pico_vgaboard_palette_8bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1280X1024_H */
