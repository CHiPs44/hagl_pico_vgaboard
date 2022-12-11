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

/* Q: would be PICO_SCANVIDEO_PIXEL_FROM_RGB5() instead of RGB8 more accurate? */

/* 16 colors IRGB palette */
/* Let's go for the 8 dark colors */
#define BGAR5515_BLACK          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define BGAR5515_DARK_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0x00, 0x00)
#define BGAR5515_DARK_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xa0, 0x00)
#define BGAR5515_DARK_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0xa0, 0x00)
#define BGAR5515_DARK_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xa0)
#define BGAR5515_DARK_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0x00, 0xa0)
#define BGAR5515_DARK_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xa0, 0xa0)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define BGAR5515_DARK_GREY      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
/* And then the 8 brighter ones */
#define BGAR5515_LIGHT_GREY     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
#define BGAR5515_RED            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00)
#define BGAR5515_GREEN          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00)
#define BGAR5515_YELLOW         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00)
#define BGAR5515_BLUE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff)
#define BGAR5515_MAGENTA        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff)
#define BGAR5515_CYAN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff)
#define BGAR5515_WHITE          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)
/*  Now lighter colors for other palettes */
#define BGAR5515_LIGHT_RED      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x80)
#define BGAR5515_LIGHT_GREEN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x80)
#define BGAR5515_LIGHT_YELLOW   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x80)
#define BGAR5515_LIGHT_BLUE     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0xff)
#define BGAR5515_LIGHT_MAGENTA  PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xff)
#define BGAR5515_LIGHT_CYAN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0xff)
/* And others, too... */
#define BGAR5515_DARK_ORANGE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x40, 0x00)
#define BGAR5515_ORANGE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x00)
#define BGAR5515_LIGHT_ORANGE   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xc0, 0x00)

/** @brief Default white on black */
uint16_t vgaboard_palette_1bpp_default[2] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_WHITE,
};

/** @brief Green CRT monitor green on black */
uint16_t vgaboard_palette_1bpp_green[2] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_LIGHT_GREEN,
};

/** @brief Amber CRT monitor orange on black */
uint16_t vgaboard_palette_1bpp_amber[2] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_LIGHT_ORANGE,
};

/** @brief Default white on black */
uint16_t vgaboard_palette_2bpp_default[4] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_BLUE,
    /* 02 */ BGAR5515_RED,
    /* 03 */ BGAR5515_WHITE,
};

/** @brief Green CRT monitor green on black */
uint16_t vgaboard_palette_2bpp_green[4] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00),
    /* 02 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xc0, 0x00),
    /* 03 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00),
};

/** @brief Amber CRT monitor orange on black */
uint16_t vgaboard_palette_2bpp_amber[4] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_DARK_ORANGE,
    /* 02 */ BGAR5515_ORANGE,
    /* 03 */ BGAR5515_LIGHT_ORANGE,
};

/** @brief IRGB 16 colors palette with dark and light grey in order of luminosity */
uint16_t vgaboard_palette_4bpp_default[16] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ BGAR5515_DARK_RED,
    /* 02 */ BGAR5515_DARK_GREEN,
    /* 03 */ BGAR5515_DARK_YELLOW,
    /* 04 */ BGAR5515_DARK_BLUE,
    /* 05 */ BGAR5515_DARK_MAGENTA,
    /* 06 */ BGAR5515_DARK_CYAN,
    /* 07 */ BGAR5515_DARK_GREY,
    /* 08 */ BGAR5515_LIGHT_GREY,
    /* 09 */ BGAR5515_RED,
    /* 10 */ BGAR5515_GREEN,
    /* 11 */ BGAR5515_YELLOW,
    /* 12 */ BGAR5515_BLUE,
    /* 13 */ BGAR5515_MAGENTA,
    /* 14 */ BGAR5515_CYAN,
    /* 15 */ BGAR5515_WHITE
};

// Empty palette for 16bpp modes
const uint16_t vgaboard_palette_16bpp_empty[0];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_H */
