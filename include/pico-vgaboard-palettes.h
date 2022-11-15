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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_H
#define _HAGL_PICO_VGABOARD_PALETTES_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RAM __not_in_flash("pico_vgaboard_framebuffer")

/* 16 colors IRGB palette */
/* Let's go for the 8 dark colors */
#define IRGB_BLACK PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define IRGB_DARK_RED PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7f, 0x00, 0x00)
#define IRGB_DARK_GREEN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x7f, 0x00)
#define IRGB_DARK_YELLOW PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7f, 0x7f, 0x00)
#define IRGB_DARK_BLUE PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x7f)
#define IRGB_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x7f, 0x00, 0x7f)
#define IRGB_DARK_CYAN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x7f, 0x7f)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define IRGB_DARK_GREY PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
/* And then the 8 brighter ones */
#define IRGB_LIGHT_GREY PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
#define IRGB_RED PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00)
#define IRGB_GREEN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00)
#define IRGB_YELLOW PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00)
#define IRGB_BLUE PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff)
#define IRGB_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff)
#define IRGB_CYAN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff)
#define IRGB_WHITE PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

/** @brief White on Black */
uint16_t RAM vgaboard_default_palette_1bpp[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_WHITE,
};

/** @brief Amstrad CPC Yellow on dark blue */
uint16_t RAM vgaboard_amstrad_cpc_palette_1bpp[] = {
    /* 00 */ IRGB_DARK_BLUE,
    /* 01 */ IRGB_YELLOW,
};

/** @brief Amstrad CPC-ish Yellow and dark blue */
uint16_t RAM vgaboard_default_palette_2bpp[] = {
    /* 00 */ IRGB_DARK_BLUE,
    /* 01 */ IRGB_DARK_YELLOW,
    /* 02 */ IRGB_BLUE,
    /* 03 */ IRGB_YELLOW,
};

/** @brief White on black with two intermediate greys */
uint16_t RAM vgaboard_grey_palette_2bpp[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_GREY,
    /* 02 */ IRGB_LIGHT_GREY,
    /* 03 */ IRGB_WHITE,
};

/** @brief default, IRGB 16 colors palette with dark and light grey in order of luminosity */
uint16_t RAM vgaboard_default_palette_4bpp[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_RED,
    /* 02 */ IRGB_DARK_GREEN,
    /* 03 */ IRGB_DARK_YELLOW,
    /* 04 */ IRGB_DARK_BLUE,
    /* 05 */ IRGB_DARK_MAGENTA,
    /* 06 */ IRGB_DARK_CYAN,
    /* 07 */ IRGB_DARK_GREY,
    /* 08 */ IRGB_LIGHT_GREY,
    /* 09 */ IRGB_RED,
    /* 10 */ IRGB_GREEN,
    /* 11 */ IRGB_YELLOW,
    /* 12 */ IRGB_BLUE,
    /* 13 */ IRGB_MAGENTA,
    /* 14 */ IRGB_CYAN,
    /* 15 */ IRGB_WHITE
};

uint16_t RAM vgaboard_grey_palette_4bpp[] = {
    /* 00 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00),
    /* 01 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x10, 0x10, 0x10),
    /* 02 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x20, 0x20, 0x20),
    /* 03 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x30, 0x30, 0x30),
    /* 04 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x40, 0x40, 0x40),
    /* 05 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x50, 0x50, 0x50),
    /* 06 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x60, 0x60, 0x60),
    /* 07 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x70, 0x70, 0x70),
    /* 08 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x80),
    /* 09 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x90, 0x90, 0x90),
    /* 10 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0xa0, 0xa0),
    /* 11 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xb0, 0xb0, 0xb0),
    /* 12 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xc0, 0xc0, 0xc0),
    /* 13 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xd0, 0xd0, 0xd0),
    /* 14 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xe0, 0xe0, 0xe0),
    /* 15 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)
};

// /* Specific to 8 bits depth / 256 colors mode */
// uint16_t RAM vgaboard_default_palette_8bpp[256];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_H */
