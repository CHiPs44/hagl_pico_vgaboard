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

https://github.com/CHiPs44/hagl_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef PICO_VGABOARD_MODES_640X400_H
#define PICO_VGABOARD_MODES_640X400_H

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

/* should be 25,175,000 (25.175 MHz) */
#define PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ (25200000L)
#if !PICO_NO_HARDWARE
    #if PICO_RP2350
        /* RP2350 seems to be more overclockable than RP2040! */
        #define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (11L * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
        #define PICO_VGABOARD_640X400_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
    #else
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            #define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (11L * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_640X400_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
        #else
            #define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (11L * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_640X400_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #endif
#else
    #define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (10L * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
    #define PICO_VGABOARD_640X400_VREG_VOLTAGE   (0)
#endif

/** @brief cf. http://tinyvga.com/vga-timing/640x400@70Hz */
const scanvideo_timing_t scanvideo_timing_640x400_70 = {
    .clock_freq      = PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ,
    .h_active        = 640,
    .v_active        = 400,
    .h_front_porch   = 16,
    .h_pulse         = 96,
    .h_total         = 800,
    .h_sync_polarity = SCANVIDEO_POLARITY_NEGATIVE,
    .v_front_porch   = 12,
    .v_pulse         = 2,
    .v_total         = 449,
    .v_sync_polarity = SCANVIDEO_POLARITY_NEGATIVE,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_640X400_70(__xscale__, __yscale__) {\
    .default_timing = &scanvideo_timing_640x400_70,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 640 / (__xscale__),\
    .height         = 400 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t scanvideo_mode_640x400_11 = SCANVIDEO_MODE_640X400_70(1, 1);
const scanvideo_mode_t scanvideo_mode_640x200_12 = SCANVIDEO_MODE_640X400_70(1, 2);
const scanvideo_mode_t scanvideo_mode_320x400_21 = SCANVIDEO_MODE_640X400_70(2, 1);
const scanvideo_mode_t scanvideo_mode_320x200_22 = SCANVIDEO_MODE_640X400_70(2, 2);
const scanvideo_mode_t scanvideo_mode_320x100_24 = SCANVIDEO_MODE_640X400_70(2, 4);
const scanvideo_mode_t scanvideo_mode_160x200_42 = SCANVIDEO_MODE_640X400_70(4, 2);
const scanvideo_mode_t scanvideo_mode_160x100_44 = SCANVIDEO_MODE_640X400_70(4, 4);

#define PICO_VGABOARD_640x400_70(__scanvideo_mode__) {\
    .scanvideo_mode = __scanvideo_mode__,\
    .freq_hz        = 70,\
    .sys_clock_khz  = PICO_VGABOARD_640X400_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_640X400_VREG_VOLTAGE,\
}

const pico_vgaboard_t pico_vgaboard_160x100_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_160x100_44);
const pico_vgaboard_t pico_vgaboard_160x200_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_160x200_42);
const pico_vgaboard_t pico_vgaboard_320x100_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_320x100_24);
const pico_vgaboard_t pico_vgaboard_320x200_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_320x200_22);
const pico_vgaboard_t pico_vgaboard_320x400_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_320x400_21);
const pico_vgaboard_t pico_vgaboard_640x200_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_640x200_12);
const pico_vgaboard_t pico_vgaboard_640x400_70 = PICO_VGABOARD_640x400_70(&scanvideo_mode_640x400_11);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_640X400_H */
