/*

MIT License

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_GREY_H
#define _HAGL_PICO_VGABOARD_PALETTES_GREY_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief White on black */
uint16_t vgaboard_palette_1bpp_mono[2] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_WHITE,
};

/** @brief White on black with two intermediate greys */
uint16_t vgaboard_palette_2bpp_grey[4] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_DARK_GREY,
    /* 02 */ RGAB5515_LIGHT_GREY,
    /* 03 */ RGAB5515_WHITE,
};

#define GREY16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define GREY16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x10, 0x10, 0x10)
#define GREY16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x20, 0x20, 0x20)
#define GREY16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x30, 0x30, 0x30)
#define GREY16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x40, 0x40, 0x40)
#define GREY16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x50, 0x50, 0x50)
#define GREY16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x60, 0x60, 0x60)
#define GREY16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x70, 0x70, 0x70)
#define GREY16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x80)
#define GREY16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x90, 0x90, 0x90)
#define GREY16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0xa0, 0xa0)
#define GREY16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xb0, 0xb0, 0xb0)
#define GREY16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc0, 0xc0, 0xc0)
#define GREY16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd0, 0xd0, 0xd0)
#define GREY16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xe0, 0xe0, 0xe0)
#define GREY16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

const uint16_t vgaboard_grey_palette_4bpp[16] = {
    GREY16_00,    GREY16_01,    GREY16_02,    GREY16_03,
    GREY16_04,    GREY16_05,    GREY16_06,    GREY16_07,
    GREY16_08,    GREY16_09,    GREY16_10,    GREY16_11,
    GREY16_12,    GREY16_13,    GREY16_14,    GREY16_15,
};

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_GREY_H */
