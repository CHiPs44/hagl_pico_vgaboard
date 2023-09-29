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

#ifndef PICO_VGABOARD_MODES_1680X1050_H
#define PICO_VGABOARD_MODES_1680X1050_H

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

#define PICO_VGABOARD_1680X1050_FREQ_HZ        60

// /* 147.14 MHz => KO */
// #define PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ (147140000L)
// #define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (2 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
// #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (0)

// /** @brief cf. http://tinyvga.com/vga-timing/1680x1050@60Hz => KO */
// const scanvideo_timing_t vga_timing_1680x1050_60_pico =
// {
//     .clock_freq      = PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ,
//     .h_active        = 1680,
//     .h_front_porch   = 104,
//     .h_pulse         = 184,
//     .h_total         = 2256,
//     .h_sync_polarity = 0,
//     .v_active        = 1050,
//     .v_front_porch   = 1,
//     .v_pulse         = 3,
//     .v_total         = 1087,
//     .v_sync_polarity = 1,
// };

/* 146.25 MHz => normally OK with vcocalc, even 2x => 146 MHz *is* OK */
#define PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ (146000000L)
#define PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ  (2 * PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
// #define PICO_VGABOARD_1680X1050_VREG_VOLTAGE   (0)

/** @brief cf. https://glenwing.github.io/docs/VESA-DMT-1.13.pdf pp. 79 */
const scanvideo_timing_t vga_timing_1680x1050_60_pico =
{
    .clock_freq      = PICO_VGABOARD_1680X1050_PIXEL_CLOCK_HZ,
    .h_active        = 1680,
    .h_front_porch   = 104,
    .h_pulse         = 176,
    .h_total         = 2240,
    .h_sync_polarity = 0,
    .v_active        = 1050,
    .v_front_porch   = 3,
    .v_pulse         = 6,
    .v_total         = 1089,
    .v_sync_polarity = 1,
};

/* 146.25 MHz / 2 => 73.125 MHz => 4 * 73 */
#define PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ (73000000L)
#define PICO_VGABOARD_840X525_SYS_CLOCK_KHZ  (4 * PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_840X525_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)

const scanvideo_timing_t vga_timing_840x525_60_pico =
{
    .clock_freq      = PICO_VGABOARD_840X525_PIXEL_CLOCK_HZ,
    .h_active        = 1680 / 2,
    .h_front_porch   = 104 / 2,
    .h_pulse         = 176 / 2,
    .h_total         = 2240 / 2,
    .h_sync_polarity = 0,
    .v_active        = 1050,
    .v_front_porch   = 3,
    .v_pulse         = 6,
    .v_total         = 1089,
    .v_sync_polarity = 1,
};

#define SCANVIDEO_MODE_1680X1050(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1680x1050_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1680 / (__xscale__),\
    .height         = 1050 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

#define SCANVIDEO_MODE_840X525(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_840x525_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = (1680 / 2) / (__xscale__),\
    .height         = (1050) / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_840_525_60_pico_1 = SCANVIDEO_MODE_1680X1050(2, 2);
const scanvideo_mode_t pico_vga_mode_840_525_60_pico_2 = SCANVIDEO_MODE_840X525  (1, 2);

#define PICO_VGABOARD_1680x1050(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_1680X1050_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_1680X1050_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_1680X1050_VREG_VOLTAGE,\
}

#define PICO_VGABOARD_840x525(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz        = PICO_VGABOARD_1680X1050_FREQ_HZ,\
    .depth          = (__depth__),\
    .palette        = ((uint16_t *)(__palette__)),\
    .sys_clock_khz  = PICO_VGABOARD_840X525_SYS_CLOCK_KHZ,\
    .vreg_voltage   = PICO_VGABOARD_840X525_VREG_VOLTAGE,\
}

/***************************/
/* ????? BYTES FRAMEBUFFER */
/***************************/

/** @brief 840x525@60Hz, 4bpp, xxx, 1680x1050 based */
const pico_vgaboard_t pico_vgaboard_840x525x4bpp_1 = PICO_VGABOARD_1680x1050(&pico_vga_mode_840_525_60_pico_1, 4, &pico_vgaboard_palette_1bpp_default);

/** @brief 840x525@60Hz, 4bpp, xxx, 840x525 based */
const pico_vgaboard_t pico_vgaboard_840x525x4bpp_2 = PICO_VGABOARD_840x525  (&pico_vga_mode_840_525_60_pico_2, 4, &pico_vgaboard_palette_1bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1680X1050_H */
