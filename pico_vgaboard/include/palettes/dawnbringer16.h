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

#ifndef _PALETTE_DAWNBRINGER16_H
#define _PALETTE_DAWNBRINGER16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/dawnbringer-16 */

/* #140c1c */ #define DAWNBRINGER16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x14, 0x0c, 0x1c)
/* #442434 */ #define DAWNBRINGER16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x44, 0x24, 0x34)
/* #30346d */ #define DAWNBRINGER16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x30, 0x34, 0x6d)
/* #4e4a4e */ #define DAWNBRINGER16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x4e, 0x4a, 0x4e)
/* #854c30 */ #define DAWNBRINGER16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x85, 0x4c, 0x30)
/* #346524 */ #define DAWNBRINGER16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x34, 0x65, 0x24)
/* #d04648 */ #define DAWNBRINGER16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd0, 0x46, 0x48)
/* #757161 */ #define DAWNBRINGER16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x75, 0x71, 0x61)
/* #597dce */ #define DAWNBRINGER16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x59, 0x7d, 0xce)
/* #d27d2c */ #define DAWNBRINGER16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd2, 0x7d, 0x2c)
/* #8595a1 */ #define DAWNBRINGER16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x85, 0x95, 0xa1)
/* #6daa2c */ #define DAWNBRINGER16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6d, 0xaa, 0x2c)
/* #d2aa99 */ #define DAWNBRINGER16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd2, 0xaa, 0x99)
/* #6dc2ca */ #define DAWNBRINGER16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6d, 0xc2, 0xca)
/* #dad45e */ #define DAWNBRINGER16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xda, 0xd4, 0x5e)
/* #deeed6 */ #define DAWNBRINGER16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xde, 0xee, 0xd6)

#define DAWNBRINGER16_BLACK         0
#define DAWNBRINGER16_VIOLET        1
#define DAWNBRINGER16_BLUE          2
#define DAWNBRINGER16_DARK_GREY     3
#define DAWNBRINGER16_BROWN         4
#define DAWNBRINGER16_GREEN         5
#define DAWNBRINGER16_PINK          6
#define DAWNBRINGER16_GREY          7
#define DAWNBRINGER16_LIGHT_BLUE    8
#define DAWNBRINGER16_ORANGE        9
#define DAWNBRINGER16_LIGHT_GREY   10
#define DAWNBRINGER16_LIGHT_GREEN  11
#define DAWNBRINGER16_LIGHT_PINK   12
#define DAWNBRINGER16_SKY_BLUE     13
#define DAWNBRINGER16_LIGHT_YELLOW 14
#define DAWNBRINGER16_WHITE        15

extern const BGAR5515 palette_4bpp_dawnbringer16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_DAWNBRINGER16_H */

/* EOF */
