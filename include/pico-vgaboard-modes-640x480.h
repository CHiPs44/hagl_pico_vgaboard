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

#ifndef _HAGL_PICO_VGABOARD_MODES_640X480_H
#define _HAGL_PICO_VGABOARD_MODES_640X480_H

#ifdef __cplusplus
extern "C"
{
#endif

#if !PICO_NO_HARDWARE
#include "hardware/vreg.h"
#else
#define VREG_VOLTAGE_DEFAULT 11
#endif
#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#define VGABOARD_640X480_PIXEL_CLOCK_HZ 25000000L
// #define VGABOARD_640X480_PIXEL_CLOCK_HZ 25175000L
// #define VGABOARD_640X480_PIXEL_CLOCK_HZ 25200000L
// #define VGABOARD_640X480_SYS_CLOCK_KHZ  (8L * VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
// #define VGABOARD_640X480_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#define VGABOARD_640X480_SYS_CLOCK_KHZ  (10L * VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
#if !PICO_NO_HARDWARE
#define VGABOARD_640X480_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
#else
#define VGABOARD_640X480_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
// #define VGABOARD_640X480_SYS_CLOCK_KHZ  (12L * VGABOARD_640X480_PIXEL_CLOCK_HZ / 1000L)
// #define VGABOARD_640X480_VREG_VOLTAGE   (VREG_VOLTAGE_1_30)
#define VGABOARD_640X480_FREQ_HZ        60

/** @brief VGA timings from http://tinyvga.com/vga-timing/640x480@60Hz */
const scanvideo_timing_t vga_timing_640x480_60_pico = {
    .clock_freq = VGABOARD_640X480_PIXEL_CLOCK_HZ,
    .h_active = 640,
    .v_active = 480,
    .h_front_porch = 16,
    .h_pulse = 96,
    .h_total = 800,
    .h_sync_polarity = 1,
    .v_front_porch = 10,
    .v_pulse = 2,
    .v_total = 525,
    .v_sync_polarity = 1,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

#define SCANVIDEO_MODE_640X480(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_640x480_60_pico,\
    .pio_program = &video_24mhz_composable,\
    .width = 640,\
    .height = 480,\
    .xscale = (__xscale__),\
    .yscale = (__yscale__),\
}

const scanvideo_mode_t vga_mode_640x480_60_pico = SCANVIDEO_MODE_640X480(1, 1);
const scanvideo_mode_t vga_mode_640x240_60_pico = SCANVIDEO_MODE_640X480(1, 2);
const scanvideo_mode_t vga_mode_320x480_60_pico = SCANVIDEO_MODE_640X480(2, 1);
const scanvideo_mode_t vga_mode_320x240_60_pico = SCANVIDEO_MODE_640X480(2, 2);
const scanvideo_mode_t vga_mode_320x120_60_pico = SCANVIDEO_MODE_640X480(2, 4);
const scanvideo_mode_t vga_mode_160x240_60_pico = SCANVIDEO_MODE_640X480(4, 2);
const scanvideo_mode_t vga_mode_160x120_60_pico = SCANVIDEO_MODE_640X480(4, 4);

#define VGABOARD_640x480(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = VGABOARD_640X480_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = VGABOARD_640X480_SYS_CLOCK_KHZ,\
    .vreg_voltage = VGABOARD_640X480_VREG_VOLTAGE,\
}

/***************************/
/* 38400 BYTES FRAMEBUFFER */
/***************************/

/** @brief 640x480@60Hz, 1bpp, monochrome, 38400 bytes framebuffer */
const vgaboard_t vgaboard_640x480x1bpp  = VGABOARD_640x480(&vga_mode_640x480_60_pico, 1, &vgaboard_palette_1bpp_default);
/** @brief 640x240@60Hz, 2bpp, 4 colors, 38400 bytes framebuffer */
const vgaboard_t vgaboard_640x240x2bpp  = VGABOARD_640x480(&vga_mode_640x240_60_pico, 2, &vgaboard_palette_2bpp_default);
/** @brief 320x480@60Hz, 2bpp, 4 colors, 38400 bytes framebuffer */
const vgaboard_t vgaboard_320x480x2bpp  = VGABOARD_640x480(&vga_mode_320x480_60_pico, 2, &vgaboard_palette_2bpp_default);
/** @brief 320x240@60Hz, 4bpp, 16 colors, 38400 bytes framebuffer */
const vgaboard_t vgaboard_320x240x4bpp  = VGABOARD_640x480(&vga_mode_320x240_60_pico, 4, &vgaboard_palette_4bpp_default);
/** @brief 320x120@60Hz, 8bpp, 256 colors, 38400 bytes framebuffer */
const vgaboard_t vgaboard_320x120x8bpp  = VGABOARD_640x480(&vga_mode_320x120_60_pico, 8, &vgaboard_palette_8bpp_default);
/** @brief 160x240@60Hz, 8bpp, 256 colors, 38400 bytes framebuffer */
const vgaboard_t vgaboard_160x240x8bpp  = VGABOARD_640x480(&vga_mode_160x240_60_pico, 8, &vgaboard_palette_8bpp_default);
/** @brief 160x120@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 38400 bytes framebuffer */
const vgaboard_t vgaboard_160x120x16bpp = VGABOARD_640x480(&vga_mode_160x120_60_pico, 16, &vgaboard_palette_16bpp_empty);

/***************************/
/* 76800 BYTES FRAMEBUFFER */
/***************************/

/** @brief 320x240@60Hz, 8bpp, 256 colors, 76800 bytes framebuffer */
const vgaboard_t vgaboard_320x240x8bpp_76800 = VGABOARD_640x480(&vga_mode_320x240_60_pico, 8, &vgaboard_palette_8bpp_default);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_640X480_H */
