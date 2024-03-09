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

#ifndef PICO_VGABOARD_MODES_1280X720_H
#define PICO_VGABOARD_MODES_1280X720_H

/* clang-format off */

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

// cf. https://projectf.io/posts/video-timings-vga-720p-1080p/#hd-1280x720-60-hz

// Spec says 74.250 MHz, vcocalc says 3 or 4 times 74.000 is feasible
#define PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ 74000000L
#define PICO_VGABOARD_1280X720_FREQ_HZ        60
#if !PICO_NO_HARDWARE
#if ALLOW_VREG_VOLTAGE_OVERRIDE
/* My Pico B0 does not reach 325 MHz and is flaky at 296, even at 1.30V! */
/* My Pico B1 does reach 296 MHz at 1.30V */
#define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (4L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (VREG_VOLTAGE_MAX)
#else
#define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (3L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1280X720_VREG_VOLTAGE   (VREG_VOLTAGE_DEFAULT)
#endif
#else
#define PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ  (3L * PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ / 1000L)
#define PICO_VGABOARD_1280X720_VREG_VOLTAGE   0
#endif

const scanvideo_timing_t vga_timing_1280x720_60_pico = {
    .clock_freq      = PICO_VGABOARD_1280X720_PIXEL_CLOCK_HZ,
    .h_active        = 1280,
    .v_active        = 720,
    .h_front_porch   = 110,
    .h_pulse         = 40,
    .h_total         = 1650,
    .h_sync_polarity = 1,
    .v_front_porch   = 5,
    .v_pulse         = 5,
    .v_total         = 750,
    .v_sync_polarity = 1,
    .enable_clock    = 0,
    .clock_polarity  = 0,
    .enable_den      = 0,
};

#define SCANVIDEO_MODE_1280X720(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1280x720_60_pico,\
    .pio_program    = &video_24mhz_composable,\
    .width          = 1280 / (__xscale__),\
    .height         = 720 / (__yscale__),\
    .xscale         = (__xscale__),\
    .yscale         = (__yscale__),\
}

const scanvideo_mode_t pico_vga_mode_1280x720_60_pico = SCANVIDEO_MODE_1280X720(1, 1);
const scanvideo_mode_t pico_vga_mode_1280x360_60_pico = SCANVIDEO_MODE_1280X720(1, 2);
const scanvideo_mode_t pico_vga_mode_640x720_60_pico  = SCANVIDEO_MODE_1280X720(2, 1);
const scanvideo_mode_t pico_vga_mode_640x360_60_pico  = SCANVIDEO_MODE_1280X720(2, 2);
const scanvideo_mode_t pico_vga_mode_640x180_60_pico  = SCANVIDEO_MODE_1280X720(2, 4);
const scanvideo_mode_t pico_vga_mode_320x360_60_pico  = SCANVIDEO_MODE_1280X720(4, 2);
const scanvideo_mode_t pico_vga_mode_320x180_60_pico  = SCANVIDEO_MODE_1280X720(4, 4);
const scanvideo_mode_t pico_vga_mode_160x180_60_pico  = SCANVIDEO_MODE_1280X720(8, 4);

#define PICO_VGABOARD_1280x720(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = PICO_VGABOARD_1280X720_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = PICO_VGABOARD_1280X720_SYS_CLOCK_KHZ,\
    .vreg_voltage = PICO_VGABOARD_1280X720_VREG_VOLTAGE,\
}

/*
NAME		SCALEX	SCALEY	WIDTH	HEIGHT	RATIO	PIXELS	COLORS	BPP	VRAM
-----------	-------	-------	-------	-------	-------	-------	-------	---	------
1280x720	1		1		1280	720		16:9	921600	2		1	115200
1280x720	1		2		1280	360		32:9	460800	2		1	57600
1280x720	2		1		640		720		8:9		460800	2		1	57600
1280x720	2		2		640		360		16:9	230400	4		2	57600
1280x720	2		4		640		180		32:9	115200	16		4	57600
1280x720	4		2		320		360		8:9		115200	16		4	57600
1280x720	4		4		320		180		16:9	57600	256		8	57600
1280x720	8		4		160		180		8:9		28800	65536	16	57600
*/

/***************************/
/* 28800 BYTES FRAMEBUFFER */
/***************************/

/** @brief 640x360@60Hz, 1bpp, 2 colors */
const pico_vgaboard_t pico_vgaboard_640x360x1bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_640x360_60_pico ,  1, &palette_1bpp_default);

/** @brief 320x180@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_320x180x4bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_320x180_60_pico ,  4, &palette_4bpp_default);

/***************************/
/* 57600 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1280x360@60Hz, 1bpp, monochrome */
const pico_vgaboard_t pico_vgaboard_1280x360x1bpp  = PICO_VGABOARD_1280x720(&pico_vga_mode_1280x360_60_pico,  1, &palette_1bpp_default);

/** @brief 640x720@60Hz, 1bpp, monochrome */
const pico_vgaboard_t pico_vgaboard_640x720x1bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_640x720_60_pico ,  1, &palette_1bpp_default);

/** @brief 640x360@60Hz, 2bpp, 4 colors */
const pico_vgaboard_t pico_vgaboard_640x360x2bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_640x360_60_pico ,  2, &palette_2bpp_default);

/** @brief 640x180@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_640x180x4bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_640x180_60_pico ,  4, &palette_4bpp_default);

/** @brief 320x360@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_320x360x4bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_320x360_60_pico ,  4, &palette_4bpp_default);

/** @brief 320x180@60Hz, 8bpp, 256 colors */
const pico_vgaboard_t pico_vgaboard_320x180x8bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_320x180_60_pico ,  8, &palette_8bpp_default);

/** @brief 160x180@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515 */
const pico_vgaboard_t pico_vgaboard_160x180x16bpp  = PICO_VGABOARD_1280x720(&pico_vga_mode_160x180_60_pico , 16, &palette_16bpp_empty );

// /***************************/
// /* 76800 BYTES FRAMEBUFFER */
// /***************************/

/** @brief 640x360@60Hz, 4bpp, 16 colors */
const pico_vgaboard_t pico_vgaboard_640x360x4bpp   = PICO_VGABOARD_1280x720(&pico_vga_mode_640x360_60_pico ,  4, &palette_4bpp_default);

/****************************/
/* 115200 BYTES FRAMEBUFFER */
/****************************/

/** @brief 1280x720@60Hz, 1bpp, monochrome => DISABLED */
const pico_vgaboard_t pico_vgaboard_1280x720x1bpp_115200  = PICO_VGABOARD_1280x720(&pico_vga_mode_1280x720_60_pico,  1, &palette_1bpp_default);

/** @brief 1280x720@60Hz, 1bpp, monochrome => DISABLED */
const pico_vgaboard_t pico_vgaboard_1280x720x4bpp = PICO_VGABOARD_1280x720(&pico_vga_mode_1280x720_60_pico,  4, &palette_4bpp_default);

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* PICO_VGABOARD_MODES_1280X720_H */
