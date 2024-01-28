/*

MIT License

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_CONSOLE16_H
#define _HAGL_PICO_VGABOARD_PALETTES_CONSOLE16_H

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

#define CO16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define CO16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x2d, 0x00, 0x6e)
#define CO16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x26, 0x0a, 0x34)
#define CO16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x34)
#define CO16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6e, 0x00, 0x85)
#define CO16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x29, 0x29, 0xff)
#define CO16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x03, 0x2b)
#define CO16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x08, 0xff)
#define CO16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x70, 0x62)
#define CO16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x3c, 0x80, 0xdb)
#define CO16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7d, 0x7d, 0xa3)
#define CO16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x8f, 0x00)
#define CO16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0a, 0xff, 0x0a)
#define CO16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0d, 0xff, 0xff)
#define CO16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x0d)
#define CO16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

#define CO16_BLACK        0
#define CO16_DARK_BLUE    1
#define CO16_DARK_VIOLET  2
#define CO16_DARK_RED     3
#define CO16_VIOLET       4
#define CO16_BLUE         5
#define CO16_RED          6
#define CO16_MAGENTA      7
#define CO16_GREENISH     8
#define CO16_LIGHT_BLUE   9
#define CO16_GREYISH     10
#define CO16_ORANGE      11
#define CO16_GREEN       12
#define CO16_SKY_BLUE    13
#define CO16_YELLOW      14
#define CO16_WHITE       15

extern const BGAR5515 pico_vgaboard_palette_4bpp_co16[16];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_CONSOLE16_H */

/* EOF */
