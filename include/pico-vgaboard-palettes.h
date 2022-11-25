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



/* 16 colors IRGB palette */
/* Let's go for the 8 dark colors */
#define RGAB5515_BLACK          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define RGAB5515_DARK_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0x00, 0x00)
#define RGAB5515_DARK_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xa0, 0x00)
#define RGAB5515_DARK_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0xa0, 0x00)
#define RGAB5515_DARK_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xa0)
#define RGAB5515_DARK_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xa0, 0x00, 0xa0)
#define RGAB5515_DARK_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xa0, 0xa0)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define RGAB5515_DARK_GREY      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
/* And then the 8 brighter ones */
#define RGAB5515_LIGHT_GREY     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
#define RGAB5515_RED            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00)
#define RGAB5515_GREEN          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00)
#define RGAB5515_YELLOW         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00)
#define RGAB5515_BLUE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff)
#define RGAB5515_MAGENTA        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff)
#define RGAB5515_CYAN           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff)
#define RGAB5515_WHITE          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)
/*  Now lighter colors for other palettes */
#define RGAB5515_LIGHT_RED      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x80)
#define RGAB5515_LIGHT_GREEN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x80)
#define RGAB5515_LIGHT_YELLOW   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x80)
#define RGAB5515_LIGHT_BLUE     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0xff)
#define RGAB5515_LIGHT_MAGENTA  PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xff)
#define RGAB5515_LIGHT_CYAN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0xff)
/* And others, too... */
#define RGAB5515_DARK_ORANGE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x40, 0x00)
#define RGAB5515_ORANGE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x00)
#define RGAB5515_LIGHT_ORANGE   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xc0, 0x00)

/** @brief Green CRT monitor green on black */
uint16_t vgaboard_palette_1bpp_green[2] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_LIGHT_GREEN,
};

/** @brief Amber CRT monitor orange on black */
uint16_t vgaboard_palette_1bpp_amber[2] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_LIGHT_ORANGE,
};

/** @brief Amstrad CPC Yellow on dark blue */
uint16_t vgaboard_amstrad_cpc_palette_1bpp[2] = {
    /* 00 */ RGAB5515_DARK_BLUE,
    /* 01 */ RGAB5515_YELLOW,
};

/** @brief Amstrad CPC-ish yellow on dark blue with 2 others shades */
uint16_t vgaboard_palette_2bpp_default[4] = {
    /* 00 */ RGAB5515_DARK_BLUE,
    /* 01 */ RGAB5515_DARK_YELLOW,
    /* 02 */ RGAB5515_BLUE,
    /* 03 */ RGAB5515_YELLOW,
};

/** @brief Amber CRT monitor orange on black */
uint16_t vgaboard_palette_2bpp_amber[4] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_DARK_ORANGE,
    /* 02 */ RGAB5515_ORANGE,
    /* 03 */ RGAB5515_LIGHT_ORANGE,
};

/** @brief Green CRT monitor green on black */
uint16_t vgaboard_palette_2bpp_green[4] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00),
    /* 02 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xc0, 0x00),
    /* 03 */ PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00),
};

/** @brief IRGB 16 colors palette with dark and light grey in order of luminosity */
uint16_t vgaboard_palette_4bpp_irgb[16] = {
    /* 00 */ RGAB5515_BLACK,
    /* 01 */ RGAB5515_DARK_RED,
    /* 02 */ RGAB5515_DARK_GREEN,
    /* 03 */ RGAB5515_DARK_YELLOW,
    /* 04 */ RGAB5515_DARK_BLUE,
    /* 05 */ RGAB5515_DARK_MAGENTA,
    /* 06 */ RGAB5515_DARK_CYAN,
    /* 07 */ RGAB5515_DARK_GREY,
    /* 08 */ RGAB5515_LIGHT_GREY,
    /* 09 */ RGAB5515_RED,
    /* 10 */ RGAB5515_GREEN,
    /* 11 */ RGAB5515_YELLOW,
    /* 12 */ RGAB5515_BLUE,
    /* 13 */ RGAB5515_MAGENTA,
    /* 14 */ RGAB5515_CYAN,
    /* 15 */ RGAB5515_WHITE
};

/** @brief Convenient (?) definitions of color indexes */
typedef enum {
    IRGB16_BLACK,
    IRGB16_DARK_RED,
    IRGB16_DARK_GREEN,
    IRGB16_DARK_YELLOW,
    IRGB16_DARK_BLUE,
    IRGB16_DARK_MAGENTA,
    IRGB16_DARK_CYAN,
    IRGB16_DARK_GREY,
    IRGB16_LIGHT_GREY,
    IRGB16_RED,
    IRGB16_GREEN,
    IRGB16_YELLOW,
    IRGB16_BLUE,
    IRGB16_MAGENTA,
    IRGB16_CYAN,
    IRGB16_WHITE
} irgb16_indexes_t;

// /* Specific to 8 bits depth / 256 colors mode */
// uint16_t vgaboard_palette_8bpp_default[256];

// Empty palette for 16bpp modes
const uint16_t vgaboard_palette_16bpp_empty[0];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_H */
