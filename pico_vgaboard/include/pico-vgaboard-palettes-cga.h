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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_CGA_H
#define _HAGL_PICO_VGABOARD_PALETTES_CGA_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://en.wikipedia.org/wiki/Color_Graphics_Adapter#Color_palette */

#define CGA_00_BLACK           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00) /* #000000 */
#define CGA_01_BLUE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xaa) /* #0000AA */
#define CGA_02_GREEN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xaa, 0x00) /* #00AA00 */
#define CGA_03_CYAN            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xaa, 0xaa) /* #00AAAA */
#define CGA_04_RED             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0x00, 0x00) /* #AA0000 */
#define CGA_05_MAGENTA         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0x00, 0xaa) /* #AA00AA */
#define CGA_06_DARK_YELLOW     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0x00) /* #AAAA00 */
#define CGA_06_BROWN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0x55, 0x00) /* #AA5500 */
#define CGA_07_LIGHT_GRAY      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa) /* #AAAAAA */
#define CGA_08_DARK_GRAY       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55) /* #555555 */
#define CGA_09_LIGHT_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0xff) /* #5555FF */
#define CGA_10_LIGHT_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0xff, 0x55) /* #55FF55 */
#define CGA_11_LIGHT_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0xff, 0xff) /* #55FFFF */
#define CGA_12_LIGHT_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x55, 0x55) /* #FF5555 */
#define CGA_13_LIGHT_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x55, 0xff) /* #FF55FF */
#define CGA_14_YELLOW          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x55) /* #FFFF55 */
#define CGA_15_WHITE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff) /* #FFFFFF */

/** @brief "Old fashioned" CGA palette, #6 as dark yellow */
extern const BGAR5515 pico_vgaboard_palette_4bpp_cga_old[16];

#define CGA_BLACK            0
#define CGA_BLUE             1
#define CGA_GREEN            2
#define CGA_CYAN             3
#define CGA_RED              4
#define CGA_MAGENTA          5
#define CGA_DARK_YELLOW      6
#define CGA_BROWN            6
#define CGA_LIGHT_GRAY       7
#define CGA_DARK_GRAY        8
#define CGA_LIGHT_BLUE       9
#define CGA_LIGHT_GREEN     10
#define CGA_LIGHT_CYAN      11
#define CGA_LIGHT_RED       12
#define CGA_LIGHT_MAGENTA   13
#define CGA_YELLOW          14
#define CGA_WHITE           15

/** @brief Standard / "modern" CGA palette, #6 as brown */
extern const BGAR5515 pico_vgaboard_palette_4bpp_cga[16];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_CGA_H */
