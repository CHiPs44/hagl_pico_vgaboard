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
#include "pico-vgaboard.h"
#include "pico/scanvideo.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

#define PICO_VGABOARD_640X400_FREQ_HZ        70
/* should be 25175000 (25.175 MHz) */
#define PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ 25200000L
#define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (10 * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X400_VREG_VOLTAGE   0
// #define PICO_VGABOARD_640X400_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
/* My B0 Pico does not reach 300MHz, even at 1.30V...
#define PICO_VGABOARD_640X400_SYS_CLOCK_KHZ  (12 * PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_640X400_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)
*/

/** @brief cf. http://tinyvga.com/vga-timing/640x400@70Hz */
const scanvideo_timing_t vga_timing_640x400_70 = {
    .clock_freq      = PICO_VGABOARD_640X400_PIXEL_CLOCK_HZ,
    .h_active        = 640,
    .v_active        = 400,
    .h_front_porch   = 16,
    .h_pulse         = 96,
    .h_total         = 800,
    .h_sync_polarity = 1,
    .v_front_porch   = 12,
    .v_pulse         = 2,
    .v_total         = 449,
    .v_sync_polarity = 1,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_640X400(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_640x400_70,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 640 / (__xscale__),\
    .height         = 400 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t scanvideo_mode_640x400_70 = SCANVIDEO_MODE_640X400(1, 1);
const scanvideo_mode_t scanvideo_mode_640x200_70 = SCANVIDEO_MODE_640X400(1, 2);
const scanvideo_mode_t scanvideo_mode_320x400_70 = SCANVIDEO_MODE_640X400(2, 1);
const scanvideo_mode_t scanvideo_mode_320x200_70 = SCANVIDEO_MODE_640X400(2, 2);
const scanvideo_mode_t scanvideo_mode_320x100_70 = SCANVIDEO_MODE_640X400(2, 4);
const scanvideo_mode_t scanvideo_mode_160x200_70 = SCANVIDEO_MODE_640X400(4, 2);
const scanvideo_mode_t scanvideo_mode_160x100_70 = SCANVIDEO_MODE_640X400(4, 4);

#define PICO_VGABOARD_640x400(__scanvideo_mode__, __width__, __height__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_640X400_FREQ_HZ,\
    .sys_clock_khz  = PICO_VGABOARD_640X400_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_640X400_VREG_VOLTAGE,\
    .width          = __width__,\
    .height         = __height__,\
}

const pico_vgaboard_t pico_vgaboard_160x100_70 = PICO_VGABOARD_640x400(&scanvideo_mode_160x100_70, 160, 100);
const pico_vgaboard_t pico_vgaboard_160x200_70 = PICO_VGABOARD_640x400(&scanvideo_mode_160x200_70, 160, 200);
const pico_vgaboard_t pico_vgaboard_320x100_70 = PICO_VGABOARD_640x400(&scanvideo_mode_320x100_70, 320, 100);
const pico_vgaboard_t pico_vgaboard_320x200_70 = PICO_VGABOARD_640x400(&scanvideo_mode_320x200_70, 320, 200);
const pico_vgaboard_t pico_vgaboard_320x400_70 = PICO_VGABOARD_640x400(&scanvideo_mode_320x400_70, 320, 400);
const pico_vgaboard_t pico_vgaboard_640x200_70 = PICO_VGABOARD_640x400(&scanvideo_mode_640x200_70, 640, 200);
const pico_vgaboard_t pico_vgaboard_640x400_70 = PICO_VGABOARD_640x400(&scanvideo_mode_640x400_70, 640, 400);

    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* PICO_VGABOARD_MODES_640X400_H */
