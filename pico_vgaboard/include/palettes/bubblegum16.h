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

#ifndef _PALETTE_BUBLEGUM16_H
#define _PALETTE_BUBLEGUM16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/bubblegum-16 */

/* #16171a */ #define BUBBLEGUM16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x16, 0x17, 0x1a)
/* #7f0622 */ #define BUBBLEGUM16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7f, 0x06, 0x22)
/* #d62411 */ #define BUBBLEGUM16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd6, 0x24, 0x11)
/* #ff8426 */ #define BUBBLEGUM16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x84, 0x26)
/* #ffd100 */ #define BUBBLEGUM16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xd1, 0x00)
/* #fafdff */ #define BUBBLEGUM16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xfa, 0xfd, 0xff)
/* #ff80a4 */ #define BUBBLEGUM16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xa4)
/* #ff2674 */ #define BUBBLEGUM16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x26, 0x74)
/* #94216a */ #define BUBBLEGUM16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x94, 0x21, 0x6a)
/* #430067 */ #define BUBBLEGUM16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x43, 0x00, 0x67)
/* #234975 */ #define BUBBLEGUM16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x23, 0x49, 0x75)
/* #68aed4 */ #define BUBBLEGUM16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x68, 0xae, 0xd4)
/* #bfff3c */ #define BUBBLEGUM16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xbf, 0xff, 0x3c)
/* #10d275 */ #define BUBBLEGUM16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x10, 0xd2, 0x75)
/* #007899 */ #define BUBBLEGUM16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x78, 0x99)
/* #002859 */ #define BUBBLEGUM16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x28, 0x59)

#define BUBBLEGUM16_BLACK        0
#define BUBBLEGUM16_DARK_RED     1
#define BUBBLEGUM16_RED          2
#define BUBBLEGUM16_ORANGE       3
#define BUBBLEGUM16_YELLOW       4
#define BUBBLEGUM16_WHITE        5
#define BUBBLEGUM16_LIGHT_PINK   6
#define BUBBLEGUM16_PINK         7
#define BUBBLEGUM16_PURPLE       8
#define BUBBLEGUM16_VIOLET       9
#define BUBBLEGUM16_BLUE        10
#define BUBBLEGUM16_SKY_BLUE    11
#define BUBBLEGUM16_LEMON_GREEN 12
#define BUBBLEGUM16_LIGHT_GREEN 13
#define BUBBLEGUM16_LIGHT_BLUE  14
#define BUBBLEGUM16_DARK_BLUE   15

extern const BGAR5515 palette_4bpp_bubblegum16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_BUBLEGUM16_H */

/* EOF */
