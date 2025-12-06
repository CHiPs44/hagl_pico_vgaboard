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

#ifndef _PALETTE_RGB121_H
#define _PALETTE_RGB121_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/4-bit-rgb
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: 4-Bit RGB
;Description: 1 red bit, 2 green bits, 1 blue bit.  Similar in concept to the Uzebox palette, which also uses bit combinations for the RGB color channels, in that case adding up to 8 bits in total, rather than 4.
;Colors: 16
*/

/* #000000 */ #define RGB121_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
/* #005500 */ #define RGB121_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x55, 0x00)
/* #00aa00 */ #define RGB121_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xaa, 0x00)
/* #00ff00 */ #define RGB121_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00)
/* #0000ff */ #define RGB121_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff)
/* #0055ff */ #define RGB121_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x55, 0xff)
/* #00aaff */ #define RGB121_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xaa, 0xff)
/* #00ffff */ #define RGB121_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff)
/* #ff0000 */ #define RGB121_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00)
/* #ff5500 */ #define RGB121_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x55, 0x00)
/* #ffaa00 */ #define RGB121_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xaa, 0x00)
/* #ffff00 */ #define RGB121_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00)
/* #ff00ff */ #define RGB121_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff)
/* #ff55ff */ #define RGB121_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x55, 0xff)
/* #ffaaff */ #define RGB121_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xaa, 0xff)
/* #ffffff */ #define RGB121_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

extern const BGAR5515 palette_4bpp_rgb121[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_RGB121_H */

/* EOF */
