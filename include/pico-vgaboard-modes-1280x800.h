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

#ifndef _HAGL_PICO_VGABOARD_MODES_1280X800_H
#define _HAGL_PICO_VGABOARD_MODES_1280X800_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

// cf. http://tinyvga.com/vga-timing/1280x800@60Hz

// 83.46 MHz => 83.625 MHz via vcocalc => 83.6?
#define VGABOARD_1280X800_PIXEL_CLOCK_HZ 83460000L
#define VGABOARD_1280X800_SYS_CLOCK_KHZ  (3L * VGABOARD_1280X800_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_1280X800_VREG_VOLTAGE   VREG_VOLTAGE_DEFAULT
#define VGABOARD_1280X800_FREQ_HZ        60

const scanvideo_timing_t vga_timing_1280x800_60_pico = {
    .clock_freq = VGABOARD_1280X800_PIXEL_CLOCK_HZ,
    .h_active = 1280,
    .v_active = 800,
    .h_front_porch = 64,
    .h_pulse = 136,
    .h_total = 1680,
    .h_sync_polarity = 0,
    .v_front_porch = 1,
    .v_pulse = 3,
    .v_total = 828,
    .v_sync_polarity = 1,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

#define SCANVIDEO_MODE_1280X800(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1280x800_60_pico,\
    .pio_program = &video_24mhz_composable,\
    .width = 1280,\
    .height = 800,\
    .xscale = (__xscale__),\
    .yscale = (__yscale__),\
}

const scanvideo_mode_t vga_mode_1280x800_60_pico = SCANVIDEO_MODE_1280X800(1, 1);
const scanvideo_mode_t vga_mode_1280x400_60_pico = SCANVIDEO_MODE_1280X800(1, 2);
const scanvideo_mode_t vga_mode_640x800_60_pico  = SCANVIDEO_MODE_1280X800(2, 1);
const scanvideo_mode_t vga_mode_640x400_60_pico  = SCANVIDEO_MODE_1280X800(2, 2);
const scanvideo_mode_t vga_mode_640x200_60_pico  = SCANVIDEO_MODE_1280X800(2, 4);
const scanvideo_mode_t vga_mode_320x400_60_pico  = SCANVIDEO_MODE_1280X800(4, 2);
const scanvideo_mode_t vga_mode_320x200_60_pico  = SCANVIDEO_MODE_1280X800(4, 4);
const scanvideo_mode_t vga_mode_160x200_60_pico  = SCANVIDEO_MODE_1280X800(8, 4);

#define VGABOARD_1280x800(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = VGABOARD_1280X800_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = VGABOARD_1280X800_SYS_CLOCK_KHZ,\
    .vreg_voltage = VGABOARD_1280X800_VREG_VOLTAGE,\
}

/*
NAME		SCALEX	SCALEY	WIDTH	HEIGHT	RATIO	PIXELS	COLORS	BPP	VRAM
-----------	-------	-------	-------	-------	-------	-------	-------	---	------
1280x800	1		1		1280	800		16:9	1024000 2		1	128000
1280x800	1		2		1280	400		32:9	512000	2		1	64000
1280x800	2		1		640		800		8:9		512000	2		1	64000
1280x800	2		2		640		400		16:9	256000	4		2	64000
1280x800	2		4		640		200		32:9	128000	16		4	64000
1280x800	4		2		320		400		8:9		128000	16		4	32000
1280x800	4		4		320		200		16:9	64000	256		8	64000
1280x800	8		4		160		200		8:9		32000	65536	16	64000
*/


/***************************/
/* 32000 BYTES FRAMEBUFFER */
/***************************/

/** @brief 320x200@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_320x200x4bpp_2 = VGABOARD_1280x800(&vga_mode_320x200_60_pico ,  4, &vgaboard_palette_4bpp_default);

/***************************/
/* 64000 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1280x400@60Hz, 1bpp, monochrome */
const vgaboard_t vgaboard_1280x400x1bpp  = VGABOARD_1280x800(&vga_mode_1280x400_60_pico,  1, &vgaboard_palette_1bpp_default);
/** @brief 640x800@60Hz, 1bpp, monochrome */
const vgaboard_t vgaboard_640x800x1bpp   = VGABOARD_1280x800(&vga_mode_640x800_60_pico ,  1, &vgaboard_palette_1bpp_default);
/** @brief 640x400@60Hz, 2bpp, 4 colors */
const vgaboard_t vgaboard_640x400x2bpp   = VGABOARD_1280x800(&vga_mode_640x400_60_pico ,  2, &vgaboard_palette_2bpp_default   );
/** @brief 640x200@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_640x200x4bpp   = VGABOARD_1280x800(&vga_mode_640x200_60_pico ,  4, &vgaboard_palette_4bpp_default);
/** @brief 320x400@60Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_320x400x4bpp_2   = VGABOARD_1280x800(&vga_mode_320x400_60_pico ,  4, &vgaboard_palette_4bpp_default);
/** @brief 320x200@60Hz, 8bpp, 256 colors */
const vgaboard_t vgaboard_320x200x8bpp_2   = VGABOARD_1280x800(&vga_mode_320x200_60_pico ,  8, &vgaboard_palette_8bpp_default);
/** @brief 160x200@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515 */
const vgaboard_t vgaboard_160x200x16bpp  = VGABOARD_1280x800(&vga_mode_160x200_60_pico , 16, &vgaboard_palette_16bpp_empty );

/****************************/
/* 128000 BYTES FRAMEBUFFER */
/****************************/

/** @brief 1280x800@60Hz, 1bpp, monochrome => DISABLED */
const vgaboard_t vgaboard_1280x800x1bpp_128000  = VGABOARD_1280x800(&vga_mode_1280x800_60_pico,  1, &vgaboard_palette_1bpp_default);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_1280X800_H */
