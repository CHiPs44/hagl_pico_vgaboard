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

#ifndef _PALETTE_PICO8_H
#define _PALETTE_PICO8_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/pico-8
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: PICO-8
;Description: The <a href="https://www.lexaloffle.com/pico-8.php">PICO-8</a> is a virtual video game console created by Lexaloffle Games.
;Colors: 16
*/

/* #000000 */ #define PICO8_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
/* #1d2b53 */ #define PICO8_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x1d, 0x2b, 0x53)
/* #7e2553 */ #define PICO8_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7e, 0x25, 0x53)
/* #008751 */ #define PICO8_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x87, 0x51)
/* #ab5236 */ #define PICO8_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xab, 0x52, 0x36)
/* #5f574f */ #define PICO8_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x5f, 0x57, 0x4f)
/* #c2c3c7 */ #define PICO8_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc2, 0xc3, 0xc7)
/* #fff1e8 */ #define PICO8_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xf1, 0xe8)
/* #ff004d */ #define PICO8_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x4d)
/* #ffa300 */ #define PICO8_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xa3, 0x00)
/* #ffec27 */ #define PICO8_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xec, 0x27)
/* #00e436 */ #define PICO8_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xe4, 0x36)
/* #29adff */ #define PICO8_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x29, 0xad, 0xff)
/* #83769c */ #define PICO8_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x83, 0x76, 0x9c)
/* #ff77a8 */ #define PICO8_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x77, 0xa8)
/* #ffccaa */ #define PICO8_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xcc, 0xaa)

extern const BGAR5515 palette_4bpp_pico8[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_PICO8_H */

/* EOF */
