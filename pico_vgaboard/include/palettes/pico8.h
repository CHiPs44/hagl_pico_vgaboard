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
FF000000
FF1D2B53
FF7E2553
FF008751
FFAB5236
FF5F574F
FFC2C3C7
FFFFF1E8
FFFF004D
FFFFA300
FFFFEC27
FF00E436
FF29ADFF
FF83769C
FFFF77A8
FFFFCCAA
*/

#define PICO8_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define PICO8_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x1D, 0x2B, 0x53)
#define PICO8_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7E, 0x25, 0x53)
#define PICO8_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x87, 0x51)
#define PICO8_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xAB, 0x52, 0x36)
#define PICO8_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x5F, 0x57, 0x4F)
#define PICO8_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xC2, 0xC3, 0xC7)
#define PICO8_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0xF1, 0xE8)
#define PICO8_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0x00, 0x4D)
#define PICO8_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0xA3, 0x00)
#define PICO8_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0xEC, 0x27)
#define PICO8_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xE4, 0x36)
#define PICO8_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x29, 0xAD, 0xFF)
#define PICO8_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x83, 0x76, 0x9C)
#define PICO8_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0x77, 0xA8)
#define PICO8_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xFF, 0xCC, 0xAA)

extern const BGAR5515 palette_4bpp_pico8[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_PICO8_H */

/* EOF */
