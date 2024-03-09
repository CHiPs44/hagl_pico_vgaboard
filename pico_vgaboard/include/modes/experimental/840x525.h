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

#ifndef PICO_VGABOARD_MODES_840X525_H
#define PICO_VGABOARD_MODES_840X525_H

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

#define PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ (73500000L)
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My B1 Pico reaches 294 MHz! */
#define PICO_VGABOARD_840X525_SYS_CLOCK_KHZ (4 * PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_840X525_VREG_VOLTAGE  (VREG_VOLTAGE_1_20)
#else
/* Should be good for any Pico */
#define PICO_VGABOARD_840X525_SYS_CLOCK_KHZ (3 * PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_840X525_VREG_VOLTAGE  (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_840X525_SYS_CLOCK_KHZ (3 * PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_840X525_VREG_VOLTAGE  (0)
#endif
#define PICO_VGABOARD_840X525_FREQ_HZ 60
const scanvideo_timing_t vga_timing_840x525_60_pico =
{
    .clock_freq      = PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ,
    .h_active        = 1680 / 2,
    .h_front_porch   = 104 / 2,
    .h_pulse         = 184 / 2,
    .h_total         = 2256 / 2,
    .h_sync_polarity = 1, // NEGATIVE
    .v_active        = 1050 / 2,
    .v_front_porch   = 1,
    .v_pulse         = 3 / 2,
    .v_total         = 1087 / 2,
    .v_sync_polarity = 1, // POSITIVE
};

#define SCANVIDEO_MODE_840X525(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_840x525_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = (1680 / 2) / (__xscale__),\
    .height         = (1050 / 2) / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_840_525_60_pico_2 = SCANVIDEO_MODE_840X525  (1, 2);
const scanvideo_mode_t pico_vga_mode_420_525_60_pico_2 = SCANVIDEO_MODE_840X525  (2, 1);

#define PICO_VGABOARD_840x525(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_840X525_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_840X525_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_840X525_VREG_VOLTAGE,\
}

/***************************/
/* 55125 BYTES FRAMEBUFFER */
/***************************/

/** @brief 840x525@60Hz, 4bpp, 55125 bytes, 840x525 based */
const pico_vgaboard_t pico_vgaboard_840x525x1bpp_2 = PICO_VGABOARD_840x525(&pico_vga_mode_840_525_60_pico_2, 1, &palette_1bpp_default);

/***************************/
/* 73500 BYTES FRAMEBUFFER */
/***************************/

/** @brief 560x525@60Hz, 1bpp, 73500 bytes, 840x525 based */
const pico_vgaboard_t pico_vgaboard_560x525x2bpp_2 = PICO_VGABOARD_840x525(&pico_vga_mode_840_525_60_pico_2, 2, &palette_2bpp_default);

/***************************/
/* 220500 BYTES FRAMEBUFFER */
/***************************/

/** @brief 840x525@60Hz, 4bpp, 220500 bytes, 840x525 based */
const pico_vgaboard_t pico_vgaboard_840x525x4bpp_2 = PICO_VGABOARD_840x525(&pico_vga_mode_840_525_60_pico_2, 4, &palette_4bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_840X525_H */
