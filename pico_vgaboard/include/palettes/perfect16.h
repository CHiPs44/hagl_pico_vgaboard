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

#ifndef _PALETTE_PERFECT16_H
#define _PALETTE_PERFECT16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/the-perfect-palette-pocket
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: The Perfect Palette pocket
;Description: A reduced and rearranged 4 bit version of The Perfect Palette
;Colors: 16
FFff97ce
FFf32341
FF7e2550
FFffd5a7
FFfb8404
FF944e2b
FFffee68
FF50c555
FF178055
FF96ffff
FF00aafb
FF494297
FFa742ee
FF272727
FF919191
FFf9f9f9
*/

#define PERFECT16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x97, 0xce)
#define PERFECT16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xf3, 0x23, 0x41)
#define PERFECT16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7e, 0x25, 0x50)
#define PERFECT16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xd5, 0xa7)
#define PERFECT16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xfb, 0x84, 0x04)
#define PERFECT16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x94, 0x4e, 0x2b)
#define PERFECT16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xee, 0x68)
#define PERFECT16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x50, 0xc5, 0x55)
#define PERFECT16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x17, 0x80, 0x55)
#define PERFECT16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x96, 0xff, 0xff)
#define PERFECT16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xaa, 0xfb)
#define PERFECT16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x49, 0x42, 0x97)
#define PERFECT16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa7, 0x42, 0xee)
#define PERFECT16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x27, 0x27, 0x27)
#define PERFECT16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x91, 0x91, 0x91)
#define PERFECT16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xf9, 0xf9, 0xf9)

extern const BGAR5515 palette_4bpp_perfect16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_PERFECT16_H */

/* EOF */
