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

#ifndef _PICO_VGABOARD_PALETTE_BG16_H
#define _PICO_VGABOARD_PALETTE_BG16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/bubblegum-16 */

#define BG16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x16, 0x17, 0x1a)
#define BG16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7f, 0x06, 0x22)
#define BG16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd6, 0x24, 0x11)
#define BG16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x84, 0x26)
#define BG16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xd1, 0x00)
#define BG16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xfa, 0xfd, 0xff)
#define BG16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xa4)
#define BG16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x26, 0x74)
#define BG16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x94, 0x21, 0x6a)
#define BG16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x43, 0x00, 0x67)
#define BG16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x23, 0x49, 0x75)
#define BG16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x68, 0xae, 0xd4)
#define BG16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xbf, 0xff, 0x3c)
#define BG16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x10, 0xd2, 0x75)
#define BG16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x78, 0x99)
#define BG16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x28, 0x59)

#define BG16_BLACK        0
#define BG16_DARK_RED     1
#define BG16_RED          2
#define BG16_ORANGE       3
#define BG16_YELLOW       4
#define BG16_WHITE        5
#define BG16_LIGHT_PINK   6
#define BG16_PINK         7
#define BG16_PURPLE       8
#define BG16_VIOLET       9
#define BG16_BLUE        10
#define BG16_SKY_BLUE    11
#define BG16_LEMON_GREEN 12
#define BG16_LIGHT_GREEN 13
#define BG16_LIGHT_BLUE  14
#define BG16_DARK_BLUE   15

extern const BGAR5515 pico_vgaboard_palette_4bpp_bg16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_PALETTE_BG16_H */

/* EOF */
