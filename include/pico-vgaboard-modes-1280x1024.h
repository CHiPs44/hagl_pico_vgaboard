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

#ifndef _HAGL_PICO_VGABOARD_MODES_1280X1024_H
#define _HAGL_PICO_VGABOARD_MODES_1280X1024_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

// rumbledethumps: half clock rate, effective 2 xscale
// #define VGABOARD_1280X1024_PIXEL_CLOCK_HZ (108000000L / 2)
#define VGABOARD_1280X1024_PIXEL_CLOCK_HZ (109000000L / 2)
#define VGABOARD_1280X1024_SYS_CLOCK_KHZ  (4 * VGABOARD_1280X1024_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_1280X1024_FREQ_HZ        60

/**
 * @brief VGA timings for 1280X1024@60Hz
 * NB:  cf. http://tinyvga.com/vga-timing/1280X1024@60Hz
 *              h-sync and v-sync polarities are positive
 *      cf. http://martin.hinner.info/vga/timing.html:
 *              1280x1024@60	                                108.0   1280 1328 1440 1688 1024 1025 1028 1066 	+hsync 	+vsync
 *              1280x1024 @ 60Hz (VESA) hsync: 64.0kHz 		    108.0   1280 1328 1440 1688 1024 1025 1028 1066 	+hsync 	+vsync 
 *      cf. https://www.ibm.com/docs/en/power8?topic=display-supported-resolution-timing-charts
 *      cf. https://tomverbeure.github.io/video_timings_calculator
 *          Modelines
 *              CVT Modeline 	    Modeline "1280x1024_59.89"  109     1280 1360 1496 1712 1024 1027 1034 1063 -HSync +VSync
 *              CVT-RB Modeline 	Modeline "1280x1024_59.96"  91      1280 1328 1360 1440 1024 1027 1034 1054 +HSync -VSync
 *              CVT-RBv2 Modeline 	Modeline "1280x1024_60"     86.006  1280 1288 1320 1360 1024 1040 1048 1054 +HSync -VSync
 *              DMT Modeline 	    Modeline "1280x1024_60.02"  108     1280 1328 1440 1688 1024 1025 1028 1066 +HSync +VSync
 *              CEA-861 Modeline 	-
 */
const scanvideo_timing_t vga_timing_1280x1024_60_chips44 = {
    .clock_freq = VGABOARD_1280X1024_PIXEL_CLOCK_HZ,
    .h_active = 1280/2,
    .v_active = 1024,
    // .h_front_porch = 48/2,
    // .h_pulse = 112/2,
    // .h_total = 1688/2,
    // .h_sync_polarity = 1, 
    .h_front_porch = 80/2,
    .h_pulse = 136/2,
    .h_total = 1712/2,
    .h_sync_polarity = 0,
    // .v_front_porch = 1,
    // .v_pulse = 3,
    // .v_total = 1066,
    .v_front_porch = 3,
    .v_pulse = 7,
    .v_total = 1063,
    .v_sync_polarity = 1,
};

/** @brief scanvideo mode for 640x512@60Hz */
const scanvideo_mode_t vga_mode_640x512_60_chips44 = {
    .default_timing = &vga_timing_1280x1024_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1280/2,
    .height = 1024,
    .xscale = 2,
    .yscale = 2,
};

/** @brief scanvideo mode for 640x256@60Hz */
const scanvideo_mode_t vga_mode_640x256_60_chips44 = {
    .default_timing = &vga_timing_1280x1024_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1280/2,
    .height = 1024,
    .xscale = 2,
    .yscale = 4,
};

/** @brief scanvideo mode for 320x256@60Hz */
const scanvideo_mode_t vga_mode_320x256_60_chips44 = {
    .default_timing = &vga_timing_1280x1024_60_chips44,
    .pio_program = &video_24mhz_composable,
    .width = 1280/2,
    .height = 1024,
    .xscale = 4/2,
    .yscale = 4,
};

/** @brief 640x512@60Hz, 1bpp, monochrome */
const vgaboard_t vgaboard_640x512x1bpp = {
    .scanvideo_mode = &vga_mode_640x512_60_chips44,
    .freq_hz = VGABOARD_1280X1024_FREQ_HZ,
    .depth = 1,
    .palette = ((uint16_t *)(&vgaboard_palette_1bpp_default)),
    .sys_clock_khz = VGABOARD_1280X1024_SYS_CLOCK_KHZ,
};

/** @brief 640x256@60Hz, 2bpp, 4 colors */
const vgaboard_t vgaboard_640x256x2bpp = {
    .scanvideo_mode = &vga_mode_640x256_60_chips44,
    .freq_hz = VGABOARD_1280X1024_FREQ_HZ,
    .depth = 2,
    .palette = ((uint16_t *)(&vgaboard_palette_2bpp_default)),
    .sys_clock_khz = VGABOARD_1280X1024_SYS_CLOCK_KHZ,
};

/** @brief 320x256@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_320x256x4bpp = {
    .scanvideo_mode = &vga_mode_320x256_60_chips44,
    .freq_hz = VGABOARD_1280X1024_FREQ_HZ,
    .depth = 4,
    .palette = ((uint16_t *)(&vgaboard_palette_4bpp_default)),
    .sys_clock_khz = VGABOARD_1280X1024_SYS_CLOCK_KHZ,
};

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_1280X1024_H */
