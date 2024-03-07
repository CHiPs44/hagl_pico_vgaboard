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

#ifndef _PICO_VGABOARD_PALETTE_DB16_H
#define _PICO_VGABOARD_PALETTE_DB16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/dawnbringer-16
    140c1c
    442434
    30346d
    4e4a4e
    854c30
    346524
    d04648
    757161
    597dce
    d27d2c
    8595a1
    6daa2c
    d2aa99
    6dc2ca
    dad45e
    deeed6
*/

#define DB16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x14, 0x0c, 0x1c)
#define DB16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x44, 0x24, 0x34)
#define DB16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x30, 0x34, 0x6d)
#define DB16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x4e, 0x4a, 0x4e)
#define DB16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x85, 0x4c, 0x30)
#define DB16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x34, 0x65, 0x24)
#define DB16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd0, 0x46, 0x48)
#define DB16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x75, 0x71, 0x61)
#define DB16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x59, 0x7d, 0xce)
#define DB16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd2, 0x7d, 0x2c)
#define DB16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x85, 0x95, 0xa1)
#define DB16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6d, 0xaa, 0x2c)
#define DB16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd2, 0xaa, 0x99)
#define DB16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6d, 0xc2, 0xca)
#define DB16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xda, 0xd4, 0x5e)
#define DB16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xde, 0xee, 0xd6)

#define DB16_BLACK         0
#define DB16_VIOLET        1
#define DB16_BLUE          2
#define DB16_DARK_GREY     3
#define DB16_BROWN         4
#define DB16_GREEN         5
#define DB16_PINK          6
#define DB16_GREY          7
#define DB16_LIGHT_BLUE    8
#define DB16_ORANGE        9
#define DB16_LIGHT_GREY   10
#define DB16_LIGHT_GREEN  11
#define DB16_LIGHT_PINK   12
#define DB16_SKY_BLUE     13
#define DB16_LIGHT_YELLOW 14
#define DB16_WHITE        15

extern const BGAR5515 pico_vgaboard_palette_4bpp_db16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_PALETTE_DB16_H */

/* EOF */
