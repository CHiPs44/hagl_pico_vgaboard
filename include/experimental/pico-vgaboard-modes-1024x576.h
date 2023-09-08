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

#ifndef _HAGL_PICO_VGABOARD_MODES_1024X576_H
#define _HAGL_PICO_VGABOARD_MODES_1024X576_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/*
    cf. https://www.improwis.com/tables/video.webt

                                                                                pixel     front       back   front       back
                                                Htotal Vtotal     hsync   vsync   clock     porch hsync porch  porch vsync porch
        mode       name      res                pixels lines      kHz  pol pol     MHz       pix   pix   pix   lines lines lines
    ====                     1024x576           // 16:9 square pixel size of 720x570 source size, D1/DV PAL
    === WSVGA, PAL 16:9      1024x576 16:9      // 16:9 square pixel; 7-10" screens; ultramobile PCs, netbooks, tablets; full 1024pix XGA width, full PAL height, for 625/50 video; quadruples to 2048x1152
    t   SDTV PAL   WSVGA     1024x576i@25               625              - -      19.67                                              16:9 square pixel
    a   SailorSat 15kHz cust 1024x576i@25.008   1328    626      15.6551 - -      20.790       48   104  152      4    4   42        soft-15kHz, 15kHz custom, SailorSat's arcade emulator modes [ref]
    t   EDTV PAL             1024x576p@50               625              - -      39.33                                              16:9 square pixel
    c   CVT 0.59M9-R         1024x576@59.82     1184    593      35.47   + -      42.00        48    32   80      3    5    9        x86 extramodes
    d   LM133LF5L01(scaled)  1024x576@59.82     1184    593      35.4730 + -      42.000       48    32   80      3    5    9        from EDID
==> c   CVT 0.59M9           1024x576@59.90     1296    599      35.88   - +      46.50        40    96  136      3    5   15        x86 extramodes
    d   LM133LF5L01(scaled)  1024x576@59.899    1296    599      35.8796 - +      46.500       40    96  136      3    5   15        from EDID
    d   LM133LF5L01(scaled)  1024x576@59.953    1376    597      71.5843 - + DS   98.500       68   108  176      1    3   17        from EDID
    d   LM133LF5L01(scaled)  1024x576@59.963    1104    592      70.9964 + - DS   78.380       24    16   40      1    3   12        from EDID
*/

#define VGABOARD_1024X576_PIXEL_CLOCK_HZ 46500000L
#define VGABOARD_1024X576_SYS_CLOCK_KHZ  (6 * VGABOARD_1024X576_PIXEL_CLOCK_HZ / 1000L)
#define VGABOARD_1024X576_VREG_VOLTAGE   (VREG_VOLTAGE_1_20)
#define VGABOARD_1024X576_FREQ_HZ 60

/**
 * @brief VGA timings for 1024x576@60Hz
 */
const scanvideo_timing_t vga_timing_1024x576_60_pico = {
    .clock_freq = VGABOARD_1024X576_PIXEL_CLOCK_HZ,
    .h_active = 1024,
    .v_active = 576,
    .h_front_porch = 40,
    .h_pulse = 96,
    .h_total = 1296, // = 1024+40+96+136
    .h_sync_polarity = 0,
    .v_front_porch = 3,
    .v_pulse = 5,
    .v_total = 599, // = 576+3+5+15
    .v_sync_polarity = 1,
    .enable_clock = 0,
    .clock_polarity = 0,
    .enable_den = 0,
};

#define SCANVIDEO_MODE_1024x576(__xscale__, __yscale__) {\
    .default_timing = &vga_timing_1024x576_60_pico,\
    .pio_program = &video_24mhz_composable,\
    .width = 1024,\
    .height = 576,\
    .xscale = (__xscale__),\
    .yscale = (__yscale__),\
}

const scanvideo_mode_t vga_mode_1024x576_60_pico = SCANVIDEO_MODE_1024x576(1, 1);
const scanvideo_mode_t vga_mode_1024x288_60_pico = SCANVIDEO_MODE_1024x576(1, 2);
const scanvideo_mode_t vga_mode_512x576_60_pico  = SCANVIDEO_MODE_1024x576(2, 1);
const scanvideo_mode_t vga_mode_512x288_60_pico  = SCANVIDEO_MODE_1024x576(2, 2);
const scanvideo_mode_t vga_mode_512x144_60_pico  = SCANVIDEO_MODE_1024x576(2, 4);
const scanvideo_mode_t vga_mode_256x288_60_pico  = SCANVIDEO_MODE_1024x576(4, 2);
const scanvideo_mode_t vga_mode_256x144_60_pico  = SCANVIDEO_MODE_1024x576(4, 4);
const scanvideo_mode_t vga_mode_128x72_60_pico   = SCANVIDEO_MODE_1024x576(8, 8);

#define VGABOARD_1024x576(__scanvideo_mode__, __depth__, __palette__) {\
    .scanvideo_mode = (__scanvideo_mode__),\
    .freq_hz = VGABOARD_1024X576_FREQ_HZ,\
    .depth = (__depth__),\
    .palette = ((uint16_t *)(__palette__)),\
    .sys_clock_khz = VGABOARD_1024X576_SYS_CLOCK_KHZ,\
    .vreg_voltage = VGABOARD_1024X576_VREG_VOLTAGE,\
}

/***************************/
/* 18432 BYTES FRAMEBUFFER */
/***************************/

/** @brief 512x288@60Hz, 1bpp, 2 colors, 18432 bytes framebuffer */
const vgaboard_t vgaboard_512x288x1bpp_18432   = VGABOARD_1024x576(&vga_mode_512x288_60_pico, 1, &vgaboard_palette_1bpp_default);
/** @brief 256x144@60Hz, 4bpp, 16 colors, 18432 bytes framebuffer */
const vgaboard_t vgaboard_256x144x4bpp_18432_1 = VGABOARD_1024x576(&vga_mode_256x144_60_pico, 4, &vgaboard_palette_4bpp_default);
/** @brief 128x96@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 18432 bytes framebuffer */
const vgaboard_t vgaboard_128x72x16bpp_18432   = VGABOARD_1024x576(&vga_mode_128x72_60_pico  , 16, &vgaboard_palette_16bpp_empty);

/***************************/
/* 36864 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1024x288@60Hz, 1bpp, monochrome, 36864 bytes framebuffer */
const vgaboard_t vgaboard_1024x288x1bpp  = VGABOARD_1024x576(&vga_mode_1024x576_60_pico,  1, &vgaboard_palette_1bpp_default);
/** @brief 512x576@60Hz, 1bpp, monochrome, 36864 bytes framebuffer */
const vgaboard_t vgaboard_512x576x1bpp   = VGABOARD_1024x576(&vga_mode_512x576_60_pico ,  1, &vgaboard_palette_1bpp_default);
/** @brief 512x288@60Hz, 2bpp, 4 colors, 36864 bytes framebuffer */
const vgaboard_t vgaboard_512x288x2bpp   = VGABOARD_1024x576(&vga_mode_512x288_60_pico ,  2, &vgaboard_palette_2bpp_default);
/** @brief 512x144@60Hz, 4bpp, 16 colors, 36864 bytes framebuffer */
const vgaboard_t vgaboard_512x144x4bpp   = VGABOARD_1024x576(&vga_mode_512x144_60_pico ,  4, &vgaboard_palette_4bpp_default);
/** @brief 256x288@60Hz, 4bpp, 16 colors, 36864 bytes framebuffer */
const vgaboard_t vgaboard_256x288x4bpp   = VGABOARD_1024x576(&vga_mode_256x288_60_pico ,  4, &vgaboard_palette_4bpp_default);
/** @brief 256x144@60Hz, 8bpp, 256 colors, 36864 bytes framebuffer */
const vgaboard_t vgaboard_256x144x8bpp   = VGABOARD_1024x576(&vga_mode_256x144_60_pico ,  8, &vgaboard_palette_8bpp_default);

/***************************/
/* 73728 BYTES FRAMEBUFFER */
/***************************/

/** @brief 1024x576@60Hz, 1bpp, monochrome, 73728 bytes framebuffer */
const vgaboard_t vgaboard_1024x576x1bpp_73728 = VGABOARD_1024x576(&vga_mode_1024x576_60_pico, 1, &vgaboard_palette_1bpp_default);
/** @brief 512x288@60Hz, 4bpp, 16 colors, 73728 bytes framebuffer */
const vgaboard_t vgaboard_512x288x4bpp_73728  = VGABOARD_1024x576(&vga_mode_512x288_60_pico , 4, &vgaboard_palette_4bpp_default);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_1024X576_H */
