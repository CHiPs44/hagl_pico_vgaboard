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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_SWEETIE16_H
#define _HAGL_PICO_VGABOARD_PALETTES_SWEETIE16_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
    cf. https://lospec.com/palette-list/sweetie-16
    cf. https://github.com/nesbox/TIC-80/blob/612fc62178e747a7cb793af0bcf127871cd0f3ba/demos/palette.lua#L15
    {name="SWEETIE-16", data="1a1c2c5d275db13e53ef7d57ffcd75a7f07038b76425717929366f3b5dc941a6f673eff7333c57566c8694b0c2f4f4f4"},
    /!\ It seems original Sweetie 16 and this TIC-80 palette are not in the exact same order from 12 to 15 /!\ 
    Correct order from the web site:
        ;paint.net Palette File
        ;Downloaded from Lospec.com/palette-list
        ;Palette Name: Sweetie 16
        ;Description: Palette created by <a href="http://grafxkid.tumblr.com/palettes" target="_blank">GrafxKid</a>.
        ;Colors: 16
        FF1a1c2c
        FF5d275d
        FFb13e53
        FFef7d57
        FFffcd75
        FFa7f070
        FF38b764
        FF257179
        FF29366f
        FF3b5dc9
        FF41a6f6
        FF73eff7
        FFf4f4f4
        FF94b0c2
        FF566c86
        FF333c57
*/

#define SWEETIE16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x1a, 0x1c, 0x2c)
#define SWEETIE16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x5d, 0x27, 0x5d)
#define SWEETIE16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xb1, 0x3e, 0x53)
#define SWEETIE16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xef, 0x7d, 0x57)
#define SWEETIE16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xcd, 0x75)
#define SWEETIE16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa7, 0xf0, 0x70)
#define SWEETIE16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x38, 0xb7, 0x64)
#define SWEETIE16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x25, 0x71, 0x79)
#define SWEETIE16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x29, 0x36, 0x6f)
#define SWEETIE16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x3b, 0x5d, 0xc9)
#define SWEETIE16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x41, 0xa6, 0xf6)
#define SWEETIE16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x73, 0xef, 0xf7)
#define SWEETIE16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xf4, 0xf4, 0xf4)
#define SWEETIE16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x94, 0xb0, 0xc2)
#define SWEETIE16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x56, 0x6c, 0x86)
#define SWEETIE16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x33, 0x3c, 0x57)

extern const uint16_t pico_vgaboard_palette_4bpp_sw16[16];

#define SWEETIE16_BLACK         0
#define SWEETIE16_VIOLET        1
#define SWEETIE16_RED           2
#define SWEETIE16_ORANGE        3
#define SWEETIE16_YELLOW        4
#define SWEETIE16_LIGHT_GREEN   5
#define SWEETIE16_GREEN         6
#define SWEETIE16_GREEN_BLUE    7
#define SWEETIE16_DARK_BLUE     8
#define SWEETIE16_BLUE          9
#define SWEETIE16_LIGHT_BLUE    10
#define SWEETIE16_LIGHT_CYAN    11
#define SWEETIE16_WHITE         12
#define SWEETIE16_LIGHT_GREY    13
#define SWEETIE16_GREY          14
#define SWEETIE16_BLUE_GREY     15

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_SWEETIE16_H */

/* EOF */
