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

#ifndef PICO_VGABOARD_MODES_640X480_H
#define PICO_VGABOARD_MODES_640X480_H

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico-vgaboard.h"
#include "pico/scanvideo.h"

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_640X480_FREQ_HZ        60
/* should be 25175000 (25.175 MHz) */
#define PICO_VGABOARD_640X480_PIXEL_CLOCK_HZ 25200000L
// #if !PICO_NO_HARDWARE
// #if ALLOW_VREG_VOLTAGE_OVERRIDE
// /* My B0 Pico reaches 280MHz at 1.20V! */
// #define PICO_VGABOARD_640X480_SYS_CLOCK_KHZ  (11L * PICO_VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
// #define PICO_VGABOARD_640X480_VREG_VOLTAGE   VREG_VOLTAGE_1_30
// #else
// #define PICO_VGABOARD_640X480_SYS_CLOCK_KHZ  (10L * PICO_VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
// #define PICO_VGABOARD_640X480_VREG_VOLTAGE   0
// #endif
// #else
#define PICO_VGABOARD_640X480_SYS_CLOCK_KHZ  (10L * PICO_VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X480_VREG_VOLTAGE   0
// #endif

/** @brief cf. http://tinyvga.com/vga-timing/640x480@60Hz */
const scanvideo_timing_t vga_timing_640x480_60 = {
    .clock_freq      = PICO_VGABOARD_640X480_PIXEL_CLOCK_HZ,
    .h_active        = 640,
    .v_active        = 480,
    .h_front_porch   = 16,
    .h_pulse         = 96,
    .h_total         = 800,
    .h_sync_polarity = 1,
    .v_front_porch   = 10,
    .v_pulse         = 2,
    .v_total         = 525,
    .v_sync_polarity = 1,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_640X480(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_640x480_60,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 640 / (__xscale__),\
    .height         = 480 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t scanvideo_mode_640x480_60 = SCANVIDEO_MODE_640X480(1, 1);
const scanvideo_mode_t scanvideo_mode_640x240_60 = SCANVIDEO_MODE_640X480(1, 2);
const scanvideo_mode_t scanvideo_mode_320x480_60 = SCANVIDEO_MODE_640X480(2, 1);
const scanvideo_mode_t scanvideo_mode_320x240_60 = SCANVIDEO_MODE_640X480(2, 2);
const scanvideo_mode_t scanvideo_mode_320x160_60 = SCANVIDEO_MODE_640X480(2, 3);
const scanvideo_mode_t scanvideo_mode_320x120_60 = SCANVIDEO_MODE_640X480(2, 4);
const scanvideo_mode_t scanvideo_mode_160x240_60 = SCANVIDEO_MODE_640X480(4, 2);
const scanvideo_mode_t scanvideo_mode_160x120_60 = SCANVIDEO_MODE_640X480(4, 4);

#define PICO_VGABOARD_640x480(__scanvideo_mode__, __width__, __height__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_640X480_FREQ_HZ,\
    .sys_clock_khz  = PICO_VGABOARD_640X480_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_640X480_VREG_VOLTAGE,\
    .width          = __width__,\
    .height         = __height__,\
}

const pico_vgaboard_t pico_vgaboard_640x480_60 = PICO_VGABOARD_640x480(&scanvideo_mode_640x480_60, 640, 480);
const pico_vgaboard_t pico_vgaboard_640x240_60 = PICO_VGABOARD_640x480(&scanvideo_mode_640x240_60, 640, 240);
const pico_vgaboard_t pico_vgaboard_320x480_60 = PICO_VGABOARD_640x480(&scanvideo_mode_320x480_60, 320, 480);
const pico_vgaboard_t pico_vgaboard_320x240_60 = PICO_VGABOARD_640x480(&scanvideo_mode_320x240_60, 320, 240);
const pico_vgaboard_t pico_vgaboard_320x160_60 = PICO_VGABOARD_640x480(&scanvideo_mode_320x160_60, 320, 160);
const pico_vgaboard_t pico_vgaboard_320x120_60 = PICO_VGABOARD_640x480(&scanvideo_mode_320x120_60, 320, 120);
const pico_vgaboard_t pico_vgaboard_160x240_60 = PICO_VGABOARD_640x480(&scanvideo_mode_160x240_60, 160, 240);
const pico_vgaboard_t pico_vgaboard_160x120_60 = PICO_VGABOARD_640x480(&scanvideo_mode_160x120_60, 160, 120);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_640X480_H */
