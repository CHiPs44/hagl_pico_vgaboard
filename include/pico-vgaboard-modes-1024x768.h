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

#ifndef _HAGL_PICO_VGABOARD_MODES_1024X768_H
#define _HAGL_PICO_VGABOARD_MODES_1024X768_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#define VGABOARD_1024X768_PIXEL_CLOCK_HZ 65000000L
#define VGABOARD_1024X768_SYS_CLOCK_KHZ  (4 * VGABOARD_1024X768_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_1024X768_FREQ_HZ        60

/**
 * @brief VGA timings for 1024x768@60Hz
 * cf. https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466 (from @kilograham)
 * NB: according to http://tinyvga.com/vga-timing/1024x768@60Hz, h-sync and v-sync polarities are negative
 */
const scanvideo_timing_t vga_timing_1024x768_60_chips44 = {
    .clock_freq = VGABOARD_1024X768_PIXEL_CLOCK_HZ,
    .h_active = 1024,
    .v_active = 768,
    .h_front_porch = 24,
    .h_pulse = 136,
    .h_total = 1344,
    .h_sync_polarity = 0,
    .v_front_porch = 3,
    .v_pulse = 6,
    .v_total = 806,
    // (kilograham) .v_sync_polarity = 1,
    .v_sync_polarity = 0,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

/** @brief scanvideo mode for 1024x384@60Hz */
const scanvideo_mode_t vga_mode_1024x384_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 1,
    .yscale = 2,
};

/** @brief scanvideo mode for 512x768@60Hz */
const scanvideo_mode_t vga_mode_512x768_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 2,
    .yscale = 1,
};

/** @brief scanvideo mode for 512x384@60Hz */
const scanvideo_mode_t vga_mode_512x384_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 2,
    .yscale = 2,
};

/** @brief scanvideo mode for 512x192@60Hz */
const scanvideo_mode_t vga_mode_512x192_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 2,
    .yscale = 4,
};

/** @brief scanvideo mode for 256x384@60Hz */
const scanvideo_mode_t vga_mode_256x384_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 4,
    .yscale = 2,
};

/** @brief scanvideo mode for 256x192@60Hz */
const scanvideo_mode_t vga_mode_256x192_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 4,
    .yscale = 4,
};

/** @brief scanvideo mode for 128x96@60Hz */
const scanvideo_mode_t vga_mode_128x96_60_chips44 = {
    .default_timing = &vga_timing_1024x768_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 8,
    .yscale = 8,
};

/** @brief 1024x384@60Hz, 1bpp, monochrome */
const vgaboard_t vgaboard_1024x384x1bpp = {
    .scanvideo_mode = &vga_mode_1024x384_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 1,
    .palette = ((uint16_t *)(&vgaboard_default_palette_1bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 512x384@60Hz, 2bpp, 4 colors */
const vgaboard_t vgaboard_512x384x2bpp = {
    .scanvideo_mode = &vga_mode_512x384_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 2,
    .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 512x192@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_512x192x4bpp = {
    .scanvideo_mode = &vga_mode_512x192_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 4,
    .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 256x384@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_256x384x4bpp = {
    .scanvideo_mode = &vga_mode_256x384_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 4,
    .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 256x192@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_256x192x4bpp = {
    .scanvideo_mode = &vga_mode_256x192_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 4,
    .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 256x192@60Hz, 8bpp, 256 colors */
const vgaboard_t vgaboard_256x192x8bpp = {
    .scanvideo_mode = &vga_mode_256x192_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 8,
    .palette = ((uint16_t *)(&vgaboard_default_palette_8bpp)),
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

/** @brief 128x96@60Hz, 16bpp, 65536 colors (really 15bbp, 32768 colors) */
const vgaboard_t vgaboard_128x96x16bpp = {
    .scanvideo_mode = &vga_mode_128x96_60_chips44,
    .freq_hz = VGABOARD_1024X768_FREQ_HZ,
    .depth = 16,
    .palette = NULL,
    .sys_clock_khz = VGABOARD_1024X768_SYS_CLOCK_KHZ,
};

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_1024X768_H */
