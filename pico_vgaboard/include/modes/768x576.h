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
#define PICO_VGABOARD_768X576_PIXEL_CLOCK_HZ (35000000UL)
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My Pico B0 reaches 280MHz at 1.20V! */
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
const scanvideo_timing_t scanvideo_timing_768x576_60 = {
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

#define SCANVIDEO_MODE_768X576_60(__xscale__, __yscale__) {\
    .default_timing = &scanvideo_timing_768x576_60,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 768 / (__xscale__),\
    .height         = 576 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_192x144_44 = SCANVIDEO_MODE_768X576_60(4, 4);
const scanvideo_mode_t pico_vga_mode_192x192_43 = SCANVIDEO_MODE_768X576_60(4, 3);
const scanvideo_mode_t pico_vga_mode_192x288_42 = SCANVIDEO_MODE_768X576_60(4, 2);
const scanvideo_mode_t pico_vga_mode_256x192_33 = SCANVIDEO_MODE_768X576_60(3, 3);
const scanvideo_mode_t pico_vga_mode_384x144_24 = SCANVIDEO_MODE_768X576_60(2, 4);
const scanvideo_mode_t pico_vga_mode_384x192_23 = SCANVIDEO_MODE_768X576_60(2, 3);
const scanvideo_mode_t pico_vga_mode_384x288_22 = SCANVIDEO_MODE_768X576_60(2, 2);
const scanvideo_mode_t pico_vga_mode_384x576_21 = SCANVIDEO_MODE_768X576_60(2, 1);
const scanvideo_mode_t pico_vga_mode_768x288_12 = SCANVIDEO_MODE_768X576_60(1, 2);
const scanvideo_mode_t pico_vga_mode_768x576_11 = SCANVIDEO_MODE_768X576_60(1, 1);

#define PICO_VGABOARD_768X576_60(__scanvideo_mode__, __width__, __height__) {\
    .scanvideo_mode = __scanvideo_mode__,\
    .width          = __width__,\
    .height         = __height__,\
    .freq_hz        = PICO_VGABOARD_768X576_FREQ_HZ,\
    .sys_clock_khz  = PICO_VGABOARD_768X576_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_768X576_VREG_VOLTAGE,\
}

const pico_vgaboard_t pico_vgaboard_192x144_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_192x144_44, 192, 144);
const pico_vgaboard_t pico_vgaboard_192x192_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_192x192_43, 192, 192);
const pico_vgaboard_t pico_vgaboard_192x288_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_192x288_42, 192, 288);
const pico_vgaboard_t pico_vgaboard_256x192_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_256x192_33, 256, 192);
const pico_vgaboard_t pico_vgaboard_384x144_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_384x144_24, 384, 144);
const pico_vgaboard_t pico_vgaboard_384x192_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_384x192_23, 384, 192);
const pico_vgaboard_t pico_vgaboard_384x288_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_384x288_22, 384, 288);
const pico_vgaboard_t pico_vgaboard_384x576_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_384x576_21, 384, 576);
const pico_vgaboard_t pico_vgaboard_768x288_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_768x288_12, 768, 288);
const pico_vgaboard_t pico_vgaboard_768x576_60 = PICO_VGABOARD_768X576_60(&pico_vga_mode_768x576_11, 768, 576);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_768X576_H */
