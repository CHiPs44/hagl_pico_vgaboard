/*

MIT License

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

#include "pico/scanvideo.h"
#include "pico-vgaboard-framebuffer.h"

    const scanvideo_mode_t vga_mode_640x480_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 1,
        .yscale = 1,
    };

    const scanvideo_mode_t vga_mode_640x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 1,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_320x480_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 2,
        .yscale = 1,
    };

    const scanvideo_mode_t vga_mode_320x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 2,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_320x120_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 2,
        .yscale = 4,
    };

    const scanvideo_mode_t vga_mode_160x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 4,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_160x120_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 4,
        .yscale = 4,
    };

    /** @brief 640x480@60Hz, 1bpp, monochrome */
    const vgaboard_t vgaboard_640x480x1bpp = {
        .scanvideo_mode = &vga_mode_640x480_60_chips44,
        .freq_hz = 60,
        .depth = 1,
        .palette = ((uint16_t *)(&vgaboard_default_palette_1bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 640x240@60Hz, 2bpp, 4 colors */
    const vgaboard_t vgaboard_640x240x2bpp = {
        .scanvideo_mode = &vga_mode_640x240_60_chips44,
        .freq_hz = 60,
        .depth = 2,
        .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 320x480@60Hz, 2bpp, 4 colors */
    const vgaboard_t vgaboard_320x480x2bpp = {
        .scanvideo_mode = &vga_mode_320x480_60_chips44,
        .freq_hz = 60,
        .depth = 2,
        .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 320x240@60Hz, 4bpp, 16 colors */
    const vgaboard_t vgaboard_320x240x4bpp = {
        .scanvideo_mode = &vga_mode_320x240_60_chips44,
        .freq_hz = 60,
        .depth = 4,
        .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 320x120@60Hz, 8bpp, 256 colors */
    const vgaboard_t vgaboard_320x120x8bpp = {
        .scanvideo_mode = &vga_mode_320x120_60_chips44,
        .freq_hz = 60,
        .depth = 8,
        .palette = ((uint16_t *)(&vgaboard_default_palette_8bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 160x240@60Hz, 8bpp, 256 colors */
    const vgaboard_t vgaboard_160x240x8bpp = {
        .scanvideo_mode = &vga_mode_160x240_60_chips44,
        .freq_hz = 60,
        .depth = 8,
        .palette = ((uint16_t *)(&vgaboard_default_palette_8bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 160x120@60Hz, 16bpp, 65536 colors (really 15bbp, 32768 colors) */
    const vgaboard_t vgaboard_160x120x16bpp = {
        .scanvideo_mode = &vga_mode_160x120_60_chips44,
        .freq_hz = 60,
        .depth = 16,
        .palette = NULL,
        .sys_clock_khz = 250000L,
    };

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_640X480_H */
