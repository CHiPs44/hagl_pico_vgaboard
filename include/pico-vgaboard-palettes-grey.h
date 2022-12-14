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

/* Same as default */
// /** @brief White on black */
// uint16_t vgaboard_palette_1bpp_oldschool[2] = {
//     /* 00 */ BGAR5515_BLACK,
//     /* 01 */ BGAR5515_WHITE,
// };

/** @brief Black on white */
uint16_t vgaboard_palette_1bpp_paperwhite[2];

/** @brief White on black with two intermediate greys */
uint16_t vgaboard_palette_2bpp_grey[4];

#define GREY16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00, 0x00, 0x00)
#define GREY16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x02, 0x02, 0x10)
#define GREY16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x04, 0x04, 0x20)
#define GREY16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x06, 0x06, 0x30)
#define GREY16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x08, 0x08, 0x40)
#define GREY16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0a, 0x0a, 0x50)
#define GREY16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0c, 0x0c, 0x60)
#define GREY16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0e, 0x0e, 0x70)
#define GREY16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x10, 0x10, 0x10)
#define GREY16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x12, 0x12, 0x12)
#define GREY16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x14, 0x14, 0x14)
#define GREY16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x16, 0x16, 0x16)
#define GREY16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x18, 0x18, 0x18)
#define GREY16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1a, 0x1a, 0x1a)
#define GREY16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1c, 0x1c, 0x1c)
#define GREY16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1e, 0x1e, 0x1e)

/** @brief 16 shades of grey from black to white */
const uint16_t vgaboard_palette_4bpp_grey[16];

/** @brief 256 shades of grey from black to white */
const uint16_t vgaboard_palette_8bpp_grey[256];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_GREY_H */
