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

#ifndef PICO_VGABOARD_MODES_1280X720_H
#define PICO_VGABOARD_MODES_1280X720_H

/* clang-format off */

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

// cf. https://projectf.io/posts/video-timings-vga-720p-1080p/#hd-1280x720-60-hz

// Spec says 74.250 MHz, vcocalc says 3 or 4 times 74.000 is feasible
#define PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ (74000000L)
#if !PICO_NO_HARDWARE
    #if PICO_RP2350
        /* RP2350 seems to be as overclockable as RP2040! */
        #define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (4L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
        #define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
    #else
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            /* My Pico B0 does not reach 325 MHz and is flaky at 296, even at 1.30V! */
            /* My Pico B1 does reach 296 MHz at 1.30V */
            #define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (4L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
        #else
            #define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (3L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #endif
#else
    #define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (3L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
    #define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (0)
#endif

const scanvideo_timing_t scanvideo_timing_1280x720_60 = {
    .clock_freq      = PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ,
    .h_active        = 1280,
    .v_active        = 720,
    .h_front_porch   = 110,
    .h_pulse         = 40,
    .h_total         = 1650,
    .h_sync_polarity = SCANVIDEO_POLARITY_NEGATIVE,
    .v_front_porch   = 5,
    .v_pulse         = 5,
    .v_total         = 750,
    .v_sync_polarity = SCANVIDEO_POLARITY_NEGATIVE,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1280X720(__xscale__, __yscale__) {\
    .default_timing = &scanvideo_timing_1280x720_60,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1280 / (__xscale__),\
    .height         = 720 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1280x720_11 = SCANVIDEO_MODE_1280X720(1, 1);
const scanvideo_mode_t pico_vga_mode_1280x360_12 = SCANVIDEO_MODE_1280X720(1, 2);
const scanvideo_mode_t pico_vga_mode_640x720_21  = SCANVIDEO_MODE_1280X720(2, 1);
const scanvideo_mode_t pico_vga_mode_640x360_22  = SCANVIDEO_MODE_1280X720(2, 2);
const scanvideo_mode_t pico_vga_mode_640x240_23  = SCANVIDEO_MODE_1280X720(2, 3);
const scanvideo_mode_t pico_vga_mode_640x180_24  = SCANVIDEO_MODE_1280X720(2, 4);
const scanvideo_mode_t pico_vga_mode_320x360_42  = SCANVIDEO_MODE_1280X720(4, 2);
const scanvideo_mode_t pico_vga_mode_320x180_44  = SCANVIDEO_MODE_1280X720(4, 4);
const scanvideo_mode_t pico_vga_mode_160x180_84  = SCANVIDEO_MODE_1280X720(8, 4);

#define PICO_VGABOARD_1280x720(__scanvideo_mode__) {\
    .scanvideo_mode = __scanvideo_mode__,\
    .freq_hz        = 60,\
    .sys_clock_khz  = PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1280X720_VREG_VOLTAGE,\
}

const pico_vgaboard_t pico_vgaboard_160x180_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_160x180_84 );
const pico_vgaboard_t pico_vgaboard_320x180_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_320x180_44 );
const pico_vgaboard_t pico_vgaboard_320x360_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_320x360_42 );
const pico_vgaboard_t pico_vgaboard_640x180_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_640x180_24 );
const pico_vgaboard_t pico_vgaboard_640x240_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_640x240_23 );
const pico_vgaboard_t pico_vgaboard_640x360_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_640x360_22 );
const pico_vgaboard_t pico_vgaboard_640x720_60  = PICO_VGABOARD_1280x720(&pico_vga_mode_640x720_21 );
const pico_vgaboard_t pico_vgaboard_1280x360_60 = PICO_VGABOARD_1280x720(&pico_vga_mode_1280x360_12);
const pico_vgaboard_t pico_vgaboard_1280x720_60 = PICO_VGABOARD_1280x720(&pico_vga_mode_1280x720_11);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1280X720_H */
