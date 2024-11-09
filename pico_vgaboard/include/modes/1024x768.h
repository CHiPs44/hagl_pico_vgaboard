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

#ifndef PICO_VGABOARD_MODES_1024X768_H
#define PICO_VGABOARD_MODES_1024X768_H

/* clang-format off */

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_1024X768_FREQ_HZ        60
#define PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ 65000000L
#if !PICO_NO_HARDWARE
    #if PICO_RP2350
        /* RP2350 seems to be as overclockable as RP2040! */
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (6 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)
        #else
            #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #else
        #if ALLOW_VREG_VOLTAGE_OVERRIDE
            /* My B1 Pico reaches 294 MHz at 1.20V! */
            #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
        #else
            #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
            #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
        #endif
    #endif
#else
    #define PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
    #define PICO_VGABOARD_1024X768_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif

/**
 * @brief VGA timings for 1024x768@60Hz
 * cf. https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466 (from @kilograham)
 * NB: according to http://tinyvga.com/vga-timing/1024x768@60Hz, h-sync and v-sync polarities are negative
 */
const scanvideo_timing_t vga_timing_1024x768_60_pico = {
    .clock_freq      = PICO_VGABOARD_1024X768_PIXEL_CLOCK_HZ,
    .h_active        = 1024,
    .v_active        = 768,
    .h_front_porch   = 24,
    .h_pulse         = 136,
    .h_total         = 1344,
    .h_sync_polarity = 0,
    .v_front_porch   = 3,
    .v_pulse         = 6,
    .v_total         = 806,
    .v_sync_polarity = 0,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1024x768(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1024x768_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1024 / (__xscale__),\
    .height         = 768 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1024x768_60_pico = SCANVIDEO_MODE_1024x768(1, 1);
const scanvideo_mode_t pico_vga_mode_1024x384_60_pico = SCANVIDEO_MODE_1024x768(1, 2);
const scanvideo_mode_t pico_vga_mode_512x768_60_pico  = SCANVIDEO_MODE_1024x768(2, 1);
const scanvideo_mode_t pico_vga_mode_512x384_60_pico  = SCANVIDEO_MODE_1024x768(2, 2);
const scanvideo_mode_t pico_vga_mode_512x256_60_pico  = SCANVIDEO_MODE_1024x768(2, 3);
const scanvideo_mode_t pico_vga_mode_512x192_60_pico  = SCANVIDEO_MODE_1024x768(2, 4);
const scanvideo_mode_t pico_vga_mode_256x384_60_pico  = SCANVIDEO_MODE_1024x768(4, 2);
const scanvideo_mode_t pico_vga_mode_256x192_60_pico  = SCANVIDEO_MODE_1024x768(4, 4);

#define PICO_VGABOARD_1024x768(__scanvideo_mode__, __width__, __height__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_1024X768_FREQ_HZ,\
    .sys_clock_khz  = PICO_VGABOARD_1024X768_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1024X768_VREG_VOLTAGE,\
    .width          = __width__,\
    .height         = __height__,\
}

const pico_vgaboard_t pico_vgaboard_256x192_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_256x192_60_pico ,  256, 192);
const pico_vgaboard_t pico_vgaboard_256x384_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_256x384_60_pico ,  256, 384);
const pico_vgaboard_t pico_vgaboard_512x192_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_512x192_60_pico ,  512, 192);
const pico_vgaboard_t pico_vgaboard_512x256_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_512x256_60_pico ,  512, 256);
const pico_vgaboard_t pico_vgaboard_512x384_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_512x384_60_pico ,  512, 384);
const pico_vgaboard_t pico_vgaboard_512x768_60  = PICO_VGABOARD_1024x768(&pico_vga_mode_512x768_60_pico ,  512, 768);
const pico_vgaboard_t pico_vgaboard_1024x384_60 = PICO_VGABOARD_1024x768(&pico_vga_mode_1024x384_60_pico, 1024, 384);
const pico_vgaboard_t pico_vgaboard_1024x768_60 = PICO_VGABOARD_1024x768(&pico_vga_mode_1024x768_60_pico, 1024, 768);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1024X768_H */
