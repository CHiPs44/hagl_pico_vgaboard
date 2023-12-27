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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_C64_H
#define _HAGL_PICO_VGABOARD_PALETTES_C64_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://en.wikipedia.org/wiki/Commodore_64#Graphics */

#define C64_00_BLACK           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00) /* #000000 */
#define C64_01_WHITE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff) /* #FFFFFF */
#define C64_02_RED             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x9f, 0x4e, 0x44) /* #9F4E44 */
#define C64_03_CYAN            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6a, 0xbf, 0xc6) /* #6ABFC6 */
#define C64_04_PURPLE          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0x57, 0xa3) /* #A057A3 */
#define C64_05_GREEN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x5c, 0xab, 0x5e) /* #5CAB5E */
#define C64_06_BLUE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x50, 0x45, 0x9b) /* #50459B */
#define C64_07_YELLOW          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc9, 0xd4, 0x87) /* #C9D487 */
#define C64_08_ORANGE          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa1, 0x68, 0x3c) /* #A1683C */
#define C64_09_BROWN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x6d, 0x54, 0x12) /* #6D5412 */
#define C64_10_LIGHT_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xcb, 0x7e, 0x75) /* #CB7E75 */
#define C64_11_DARK_GRAY       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x62, 0x62, 0x62) /* #626262 */
#define C64_12_MID_GRAY        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x89, 0x89, 0x89) /* #898989 */
#define C64_13_LIGHT_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x9a, 0xe2, 0x9b) /* #9AE29B */
#define C64_14_LIGHT_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x88, 0x7e, 0xcb) /* #887ECB */
#define C64_15_LIGHT_GRAY      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xad, 0xad, 0xad) /* #ADADAD */

/** @brief C64 palette */
extern const BGAR5515 pico_vgaboard_palette_4bpp_c64[16];

#define C64_BLACK        0
#define C64_WHITE        1
#define C64_RED          2
#define C64_CYAN         3
#define C64_PURPLE       4
#define C64_GREEN        5
#define C64_BLUE         6
#define C64_YELLOW       7
#define C64_ORANGE       8
#define C64_BROWN        9
#define C64_LIGHT_RED   10
#define C64_DARK_GRAY   11
#define C64_MID_GRAY    12
#define C64_LIGHT_GREEN 13
#define C64_LIGHT_BLUE  14
#define C64_LIGHT_GRAY  15

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_C64_H */
