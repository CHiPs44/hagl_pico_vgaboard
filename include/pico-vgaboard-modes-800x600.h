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

#ifndef _HAGL_PICO_VGABOARD_800x600_MODES_800X600_H
#define _HAGL_PICO_VGABOARD_800x600_MODES_800X600_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/* cf. http://tinyvga.com/vga-timing/800x600@60Hz */

#define VGABOARD_800X600_PIXEL_CLOCK_HZ 40000000L
#define VGABOARD_800X600_SYS_CLOCK_KHZ  (6 * VGABOARD_800X600_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_800X600_FREQ_HZ        60

const scanvideo_timing_t vga_timing_800x600_60_chips44 = {
    .clock_freq = VGABOARD_800X600_PIXEL_CLOCK_HZ,
    .h_active = 800,
    .v_active = 600,
    .h_front_porch = 40,
    .h_pulse = 128,
    .h_total = 1056,
    .h_sync_polarity = 1,
    .v_front_porch = 1,
    .v_pulse = 4,
    .v_total = 628,
    .v_sync_polarity = 1,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

#define SCANVIDEO_MODE_800x600(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_800x600_60_chips44,\
    .pio_program = &video_24mhz_composable,\
    .width = 800,\
    .height = 600,\
    .xscale = (__xscale__),\
    .yscale = (__yscale__),\
}

const scanvideo_mode_t vga_mode_800x600_60_chips44 = SCANVIDEO_MODE_800x600(1, 1);
const scanvideo_mode_t vga_mode_800x300_60_chips44 = SCANVIDEO_MODE_800x600(1, 2);
const scanvideo_mode_t vga_mode_400x600_60_chips44 = SCANVIDEO_MODE_800x600(2, 1);
const scanvideo_mode_t vga_mode_400x300_60_chips44 = SCANVIDEO_MODE_800x600(2, 2);
const scanvideo_mode_t vga_mode_400x150_60_chips44 = SCANVIDEO_MODE_800x600(2, 4);
const scanvideo_mode_t vga_mode_200x300_60_chips44 = SCANVIDEO_MODE_800x600(4, 2);
const scanvideo_mode_t vga_mode_200x150_60_chips44 = SCANVIDEO_MODE_800x600(4, 4);

#define VGABOARD_800x600(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = VGABOARD_800X600_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = VGABOARD_800X600_SYS_CLOCK_KHZ,\
}

/** @brief 800x600@60Hz, 1bpp, monochrome */
const vgaboard_t vgaboard_800x600x1bpp  = VGABOARD_800x600(&vga_mode_800x600_60_chips44,  1, &vgaboard_palette_1bpp_mono);
/** @brief 800x300@60Hz, 2bpp, 4 colors */
const vgaboard_t vgaboard_800x300x2bpp  = VGABOARD_800x600(&vga_mode_800x300_60_chips44,  2, &vgaboard_palette_2bpp_default);
/** @brief 400x600@60Hz, 2bpp, 4 colors */
const vgaboard_t vgaboard_400x600x2bpp  = VGABOARD_800x600(&vga_mode_400x600_60_chips44,  2, &vgaboard_palette_2bpp_default);
/** @brief 400x300@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_400x300x4bpp  = VGABOARD_800x600(&vga_mode_400x300_60_chips44,  4, &vgaboard_palette_4bpp_irgb);
/** @brief 400x150@60Hz, 8bpp, 256 colors */
const vgaboard_t vgaboard_400x150x8bpp  = VGABOARD_800x600(&vga_mode_400x150_60_chips44,  8, &vgaboard_palette_8bpp_default);
/** @brief 200x300@60Hz, 8bpp, 256 colors */
const vgaboard_t vgaboard_200x300x8bpp  = VGABOARD_800x600(&vga_mode_200x300_60_chips44,  8, &vgaboard_palette_8bpp_default);
/** @brief 200x150@60Hz, 16bpp, 32768 colors + 1 bit alpha - RGAB5515 */
const vgaboard_t vgaboard_200x150x16bpp = VGABOARD_800x600(&vga_mode_200x150_60_chips44, 16, &vgaboard_palette_16bpp_empty);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_800x600_MODES_800X600_H */