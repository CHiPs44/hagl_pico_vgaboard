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

#ifndef _PALETTE_NA16_H
#define _PALETTE_NA16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/na16
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: NA16
;Description: <a target="_blank" href="https://twitter.com/hashtag/NA16,pixelart">#NA16</a>
;Colors: 16
FF8c8fae
FF584563
FF3e2137
FF9a6348
FFd79b7d
FFf5edba
FFc0c741
FF647d34
FFe4943a
FF9d303b
FFd26471
FF70377f
FF7ec4c1
FF34859d
FF17434b
FF1f0e1c
*/

#define NA16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x8c, 0x8f, 0xae)
#define NA16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x58, 0x45, 0x63)
#define NA16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x3e, 0x21, 0x37)
#define NA16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x9a, 0x63, 0x48)
#define NA16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd7, 0x9b, 0x7d)
#define NA16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xf5, 0xed, 0xba)
#define NA16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc0, 0xc7, 0x41)
#define NA16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x64, 0x7d, 0x34)
#define NA16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xe4, 0x94, 0x3a)
#define NA16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x9d, 0x30, 0x3b)
#define NA16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd2, 0x64, 0x71)
#define NA16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x70, 0x37, 0x7f)
#define NA16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7e, 0xc4, 0xc1)
#define NA16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x34, 0x85, 0x9d)
#define NA16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x17, 0x43, 0x4b)
#define NA16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x1f, 0x0e, 0x1c)

extern const BGAR5515 palette_4bpp_na16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_NA16_H */

/* EOF */
