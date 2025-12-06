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

#ifndef _PALETTE_FANTASY16_H
#define _PALETTE_FANTASY16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/fantasy-16
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: Fantasy 16
;Description: Created by <a target="_blank" href="https://twitter.com/brettybooop">Brettybooop</a>.
;Colors: 16
*/

/* #8e6d34 */ #define FANTASY16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x8e, 0x6d, 0x34)
/* #513a18 */ #define FANTASY16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x51, 0x3a, 0x18)
/* #332710 */ #define FANTASY16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x33, 0x27, 0x10)
/* #14130c */ #define FANTASY16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x14, 0x13, 0x0c)
/* #461820 */ #define FANTASY16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x46, 0x18, 0x20)
/* #a63c1e */ #define FANTASY16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa6, 0x3c, 0x1e)
/* #d37b1e */ #define FANTASY16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd3, 0x7b, 0x1e)
/* #e7bc4f */ #define FANTASY16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xe7, 0xbc, 0x4f)
/* #eeeefa */ #define FANTASY16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xee, 0xee, 0xfa)
/* #d9d55b */ #define FANTASY16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd9, 0xd5, 0x5b)
/* #757320 */ #define FANTASY16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x75, 0x73, 0x20)
/* #14210f */ #define FANTASY16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x14, 0x21, 0x0f)
/* #040405 */ #define FANTASY16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x04, 0x04, 0x05)
/* #1c1b2f */ #define FANTASY16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x1c, 0x1b, 0x2f)
/* #435063 */ #define FANTASY16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x43, 0x50, 0x63)
/* #60a18f */ #define FANTASY16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x60, 0xa1, 0x8f)

extern const BGAR5515 palette_4bpp_fantasy16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_FANTASY16_H */

/* EOF */
