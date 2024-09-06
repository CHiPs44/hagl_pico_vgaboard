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

#ifndef _PALETTE_NIGHT16_H
#define _PALETTE_NIGHT16_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://lospec.com/palette-list/night-16
;paint.net Palette File
;Downloaded from Lospec.com/palette-list
;Palette Name: Night 16
;Description: 3rd place winner of the <a href="https://pixeljoint.com/forum/forum_posts.asp?TID=22582&PN=4">PixelJoint 16 color palette competition</a> (2015). Created by <a href="http://pixeljoint.com/p/26118.htm">Night</a>.
;Colors: 16
FF0f0f1e
FFfff8bc
FF0c2133
FF48586d
FF79a0b0
FFb0ce9d
FF657f49
FF3f4536
FFb99d6a
FFffdd91
FFdd945b
FF9a5142
FF644b48
FF333033
FF767088
FFc5a3b3
*/

#define NIGHT16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0f, 0x0f, 0x1e)
#define NIGHT16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xf8, 0xbc)
#define NIGHT16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x0c, 0x21, 0x33)
#define NIGHT16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x48, 0x58, 0x6d)
#define NIGHT16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x79, 0xa0, 0xb0)
#define NIGHT16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xb0, 0xce, 0x9d)
#define NIGHT16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x65, 0x7f, 0x49)
#define NIGHT16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x3f, 0x45, 0x36)
#define NIGHT16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xb9, 0x9d, 0x6a)
#define NIGHT16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xdd, 0x91)
#define NIGHT16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xdd, 0x94, 0x5b)
#define NIGHT16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x9a, 0x51, 0x42)
#define NIGHT16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x64, 0x4b, 0x48)
#define NIGHT16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x33, 0x30, 0x33)
#define NIGHT16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x76, 0x70, 0x88)
#define NIGHT16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc5, 0xa3, 0xb3)

extern const BGAR5515 palette_4bpp_night16[16];

#ifdef __cplusplus
}
#endif

#endif /* _PALETTE_NIGHT16_H */

/* EOF */
