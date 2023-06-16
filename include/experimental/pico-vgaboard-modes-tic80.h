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

#ifndef _HAGL_PICO_VGABOARD_MODES_TIC80_H
#define _HAGL_PICO_VGABOARD_MODES_TIC80_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

/*
    240x136 * 2
        => 480x272
            => 512x384 with 16 pixels left margin and 56 pixels top margin
            => 640x400 with 80 pixels left margin and 64 pixels top margin
            => 640x480 with 80 pixels left margin and 104 pixels top margin
    240x136 * 3
        => 720x408
            => 768x576 with 24 pixels left margin and 84 pixels top margin
            => 800x600 with 40 pixels left margin and 96 pixels top margin
    240x136 * 4
        => 960x544
            (does not fit in real 16:9 960x540)
            => 1024*576 with 32 pixels left margin and 16 pixels top margin
    240x136 * 5
        => 1200x680
            => 1280*720 with 40 pixels left margin and 20 pixels top margin
*/

/** @brief 240x136@XXHz, 4bpp, 16 colors, 16320 bytes framebuffer */
const vgaboard_t vgaboard_256x192x8bpp_49152_2 = VGABOARD_768X576(&vga_mode_256x192_60_33_pico,  8, &vgaboard_palette_8bpp_default);

/***************************/
/* 55296 BYTES FRAMEBUFFER */
/***************************/

/** @brief 768x576@60Hz, 1bpp, monochrome, 55296 bytes framebuffer */
const vgaboard_t vgaboard_768x576x1bpp  = VGABOARD_768X576(&vga_mode_768x576_60_11_pico,  1, &vgaboard_palette_1bpp_default);
/** @brief 768x288@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_768x288x2bpp  = VGABOARD_768X576(&vga_mode_768x288_60_12_pico,  2, &vgaboard_palette_2bpp_default);
/** @brief 384x576@60Hz, 2bpp, 4 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x576x2bpp  = VGABOARD_768X576(&vga_mode_384x576_60_21_pico,  2, &vgaboard_palette_2bpp_default);
/** @brief 384x288@60Hz, 4bpp, 16 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x288x4bpp  = VGABOARD_768X576(&vga_mode_384x288_60_22_pico,  4, &vgaboard_palette_4bpp_default);
/** @brief 384x144@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_384x144x8bpp  = VGABOARD_768X576(&vga_mode_384x144_60_24_pico,  8, &vgaboard_palette_8bpp_default);
/** @brief 192x288@60Hz, 8bpp, 256 colors, 55296 bytes framebuffer */
const vgaboard_t vgaboard_192x288x8bpp  = VGABOARD_768X576(&vga_mode_192x288_60_42_pico,  8, &vgaboard_palette_8bpp_default);
/** @brief 192x144@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 55296 bytes framebuffer */
const vgaboard_t vgaboard_192x144x16bpp = VGABOARD_768X576(&vga_mode_192x144_60_44_pico, 16, &vgaboard_palette_16bpp_empty);

/****************************/
/* 110592 BYTES FRAMEBUFFER */
/****************************/

/** @brief 192x288@60Hz, 16bpp, 32768 colors + 1 bit alpha - BGAR5515, 110592 bytes framebuffer */
const vgaboard_t vgaboard_192x288x16bpp_110592 = VGABOARD_768X576(&vga_mode_192x288_60_42_pico, 16, &vgaboard_palette_16bpp_empty);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_TIC80_H */
