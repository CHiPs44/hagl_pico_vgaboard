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

#ifndef _HAGL_PICO_VGABOARD_MODES_640X400_H
#define _HAGL_PICO_VGABOARD_MODES_640X400_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#define VGABOARD_640X400_PIXEL_CLOCK_HZ (25000000L)
#define VGABOARD_640X400_SYS_CLOCK_KHZ  (10 * VGABOARD_640X400_PIXEL_CLOCK_HZ / 1000L)
// 25000000L
// .clock_freq = 25175000,
// 11 * 25175 => 276925L
// 10 * 25175 => 251750L
#define VGABOARD_640X400_FREQ_HZ 70

    /** @brief VGA timings from http://tinyvga.com/vga-timing/640x400@70Hz */
    const scanvideo_timing_t vga_timing_640x400_70_chips44 = {
        .clock_freq = VGABOARD_640X400_PIXEL_CLOCK_HZ,
        .h_active = 640,
        .v_active = 400,
        .h_front_porch = 16,
        .h_pulse = 96,
        .h_total = 800,
        .h_sync_polarity = 1,
        .v_front_porch = 12,
        .v_pulse = 2,
        .v_total = 449,
        .v_sync_polarity = 1,
        .enable_clock = 0,
        .clock_polarity = 0,
        .enable_den = 0,
    };

    const scanvideo_mode_t vga_mode_640x400_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 1,
        .yscale = 1,
    };

    const scanvideo_mode_t vga_mode_640x200_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 1,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_320x400_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 2,
        .yscale = 1,
    };

    const scanvideo_mode_t vga_mode_320x200_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 2,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_320x100_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 2,
        .yscale = 4,
    };

    const scanvideo_mode_t vga_mode_160x200_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 4,
        .yscale = 2,
    };

    const scanvideo_mode_t vga_mode_160x100_70_chips44 = {
        .default_timing = &vga_timing_640x400_70_chips44,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 400,
        .xscale = 4,
        .yscale = 4,
    };

    /** @brief 640x400@70Hz, 1bpp, monochrome, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_640x400x1bpp = {
        .scanvideo_mode = &vga_mode_640x400_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 1,
        .palette = ((uint16_t *)(&vgaboard_palette_1bpp_mono)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 640x200@70Hz, 2bpp, 4 colors, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_640x200x2bpp = {
        .scanvideo_mode = &vga_mode_640x200_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 2,
        .palette = ((uint16_t *)(&vgaboard_palette_2bpp_default)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 320x400@70Hz, 2bpp, 4 colors, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_320x400x2bpp = {
        .scanvideo_mode = &vga_mode_320x400_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 2,
        .palette = ((uint16_t *)(&vgaboard_palette_2bpp_default)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 320x200@70Hz, 4bpp, 16 colors, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_320x200x4bpp = {
        .scanvideo_mode = &vga_mode_320x200_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 4,
        .palette = ((uint16_t *)(&vgaboard_palette_4bpp_irgb)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 320x400@70Hz, 4bpp, 16 colors, 64000 bytes framebuffer */
    const vgaboard_t vgaboard_320x400x4bpp = {
        .scanvideo_mode = &vga_mode_320x400_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 4,
        .palette = ((uint16_t *)(&vgaboard_palette_4bpp_irgb)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 320x100@70Hz, 8bpp, 256 colors, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_320x100x8bpp = {
        .scanvideo_mode = &vga_mode_320x100_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 8,
        .palette = ((uint16_t *)(&vgaboard_palette_8bpp_default)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 160x200@70Hz, 8bpp, 256 colors, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_160x200x8bpp = {
        .scanvideo_mode = &vga_mode_160x200_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 8,
        .palette = ((uint16_t *)(&vgaboard_palette_8bpp_default)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 160x100@70Hz, 16bpp, 32768 colors + 1 bit alpha - RGAB5515, 32000 bytes framebuffer */
    const vgaboard_t vgaboard_160x100x16bpp = {
        .scanvideo_mode = &vga_mode_160x100_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 16,
        .palette = NULL,
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 640x200@70Hz, 4bpp, 16 colors, 64000 bytes framebuffer */
    const vgaboard_t vgaboard_640x200x4bpp = {
        .scanvideo_mode = &vga_mode_640x200_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 4,
        .palette = ((uint16_t *)(&vgaboard_palette_4bpp_irgb)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

    /** @brief 320x200@70Hz, 8bpp, 256 colors, 64000 bytes framebuffer */
    const vgaboard_t vgaboard_320x200x8bpp_64k = {
        .scanvideo_mode = &vga_mode_320x200_70_chips44,
        .freq_hz = VGABOARD_640X400_FREQ_HZ,
        .depth = 8,
        .palette = ((uint16_t *)(&vgaboard_palette_8bpp_default)),
        .sys_clock_khz = VGABOARD_640X400_SYS_CLOCK_KHZ,
    };

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_640X400_H */
