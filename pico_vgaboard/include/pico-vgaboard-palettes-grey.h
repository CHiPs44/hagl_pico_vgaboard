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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_GREY_H
#define _HAGL_PICO_VGABOARD_PALETTES_GREY_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Black on white */
extern const uint16_t pico_vgaboard_palette_1bpp_paperwhite[2];

/** @brief White on black with two intermediate greys */
extern const uint16_t pico_vgaboard_palette_2bpp_grey[4];

#define GREY16_00 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x00u, 0x00u, 0x00u)
#define GREY16_01 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x02u, 0x02u, 0x02u)
#define GREY16_02 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x04u, 0x04u, 0x04u)
#define GREY16_03 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x06u, 0x06u, 0x06u)
#define GREY16_04 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x08u, 0x08u, 0x08u)
#define GREY16_05 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0au, 0x0au, 0x0au)
#define GREY16_06 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0cu, 0x0cu, 0x0cu)
#define GREY16_07 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0eu, 0x0eu, 0x0eu)
#define GREY16_08 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x10u, 0x10u, 0x10u)
#define GREY16_09 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x12u, 0x12u, 0x12u)
#define GREY16_10 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x14u, 0x14u, 0x14u)
#define GREY16_11 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x16u, 0x16u, 0x16u)
#define GREY16_12 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x18u, 0x18u, 0x18u)
#define GREY16_13 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1au, 0x1au, 0x1au)
#define GREY16_14 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1cu, 0x1cu, 0x1cu)
#define GREY16_15 PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x1eu, 0x1eu, 0x1eu)

/** @brief 16 shades of grey from black to white */
extern const uint16_t pico_vgaboard_palette_4bpp_grey[16];

/* There are only 32 shades of grey in RGB555... */
// /** @brief 256 shades of grey from black to white */
// extern const uint16_t pico_vgaboard_palette_8bpp_grey[256];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_GREY_H */
