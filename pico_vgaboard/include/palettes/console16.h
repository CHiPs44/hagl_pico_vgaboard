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

#ifndef _PALETTE_CONSOLE16_H
#define _PALETTE_CONSOLE16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
    Console 16 by adamPhoebe
    cf. https://lospec.com/palette-list/console16 => GPL file

    Reordered by luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b
              or luminance =   2126 * r +   7152 * g +    722 * b

    #	R	G	B	Lum		Hex
    ---	---	---	---	------	------
    0	0	0	0	      0	000000
    1	45	0	110	 175090	2d006e
    2	38	10	52	 189852	260a34
    3	128	0	52	 309672	800034
    4	110	0	133	 329886	6e0085
    5	41	41	255	 564508	2929ff
    6	255	3	43	 594632	ff032b
    7	255	8	255	 783456	ff08ff
    8	0	112	98	 871780	007062
    9	60	128	219	1201134	3c80db
    10	125	125	163	1277436	7d7da3
    11	255	143	0	1564866	ff8f00
    12	10	255	10	1852240	0aff0a
    13	13	255	255	2035508	0dffff
    14	255	255	13	2375276	ffff0d
    15	255	255	255	2550000	ffffff

*/

/* #000000 */ #define CONSOLE16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
/* #2d006e */ #define CONSOLE16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x2d, 0x00, 0x6e)
/* #260a34 */ #define CONSOLE16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x26, 0x0a, 0x34)
/* #800034 */ #define CONSOLE16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x34)
/* #6e0085 */ #define CONSOLE16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6e, 0x00, 0x85)
/* #2929ff */ #define CONSOLE16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x29, 0x29, 0xff)
/* #ff032b */ #define CONSOLE16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x03, 0x2b)
/* #ff08ff */ #define CONSOLE16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x08, 0xff)
/* #007062 */ #define CONSOLE16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x70, 0x62)
/* #3c80db */ #define CONSOLE16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x3c, 0x80, 0xdb)
/* #7d7da3 */ #define CONSOLE16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7d, 0x7d, 0xa3)
/* #ff8f00 */ #define CONSOLE16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x8f, 0x00)
/* #0aff0a */ #define CONSOLE16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0a, 0xff, 0x0a)
/* #0dffff */ #define CONSOLE16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0d, 0xff, 0xff)
/* #ffff0d */ #define CONSOLE16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x0d)
/* #ffffff */ #define CONSOLE16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

#define CONSOLE16_BLACK        0
#define CONSOLE16_DARK_BLUE    1
#define CONSOLE16_DARK_VIOLET  2
#define CONSOLE16_DARK_RED     3
#define CONSOLE16_VIOLET       4
#define CONSOLE16_BLUE         5
#define CONSOLE16_RED          6
#define CONSOLE16_MAGENTA      7
#define CONSOLE16_GREENISH     8
#define CONSOLE16_LIGHT_BLUE   9
#define CONSOLE16_GREYISH     10
#define CONSOLE16_ORANGE      11
#define CONSOLE16_GREEN       12
#define CONSOLE16_SKY_BLUE    13
#define CONSOLE16_YELLOW      14
#define CONSOLE16_WHITE       15

extern const BGAR5515 palette_4bpp_console16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_CONSOLE16_H */

/* EOF */
