/*

MIT License

Copyright (c) 2021 Christophe Petit

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
 - https://github.com/CHiPs44/hagl_pico_vgaboard
 - https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <stdio.h>
#include <stdint.h>

#include "bitmap.h"
#include "hagl_hal.h"
#include "pico-vga-framebuffer.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"

/*                                                       RED  GREEN BLUE  */
/* Let's go for the dark colors */
#define IRGB_BLACK        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u,   0u)
#define IRGB_DARK_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u,   0u)
#define IRGB_DARK_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u,   0u)
#define IRGB_DARK_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u,   0u)
#define IRGB_DARK_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 128u)
#define IRGB_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u, 128u)
#define IRGB_DARK_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u, 128u)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define IRGB_DARK_GREY    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
#define IRGB_LIGHT_GREY   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
/* And then the brighter ones */
#define IRGB_RED          PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u,   0u)
#define IRGB_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u,   0u)
#define IRGB_YELLOW       PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u,   0u)
#define IRGB_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 255u)
#define IRGB_MAGENTA      PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u, 255u)
#define IRGB_CYAN         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u, 255u)
#define IRGB_WHITE        PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 255u)

const uint16_t hagl_hal_default_palette_2[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_WHITE,
};

const uint16_t hagl_hal_default_palette_4[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_GREY,
    /* 02 */ IRGB_LIGHT_GREY,
    /* 03 */ IRGB_WHITE,
};

const uint16_t hagl_hal_default_palette_16[] = {
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

uint16_t hagl_hal_default_palette_256[256];

scanvideo_mode_t *hgal_hal_vga_mode = NULL;
uint16_t *hagl_hal_palette = NULL;

uint8_t hagl_hal_bpp = 0;

void hagl_hal_setup_palette_256()
{
    uint8_t i, r, g, b;

    for (uint8_t c = 0; c < 256; c++) {
        // 76543210
        // rrggbbii
        i = c & 0x03;
        r = ((((c >> 4) & 0x03) << 2) & i);
        g = ((((c >> 2) & 0x03) << 2) & i);
        b = ((((c     ) & 0x03) << 2) & i);
        hagl_hal_default_palette_256[c] = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
    }
}

void hagl_hal_set_vga_mode(const scanvideo_mode_t *vga_mode)
{
    hgal_hal_vga_mode = (scanvideo_mode_t *)vga_mode;
}

void hagl_hal_set_bpp(uint8_t bpp)
{
    hagl_hal_bpp = bpp;
}

uint8_t hagl_hal_get_bpp()
{
    return hagl_hal_bpp;
}

void hagl_hal_set_palette(const uint16_t *palette)
{
    hagl_hal_palette = (uint16_t *)palette;
}

uint16_t *hagl_hal_get_palette()
{
    return hagl_hal_palette;
}

color_t hagl_hal_get_color(uint8_t index)
{
    return hagl_hal_palette[index];
}

void hagl_hal_change_palette(uint16_t *palette)
{
    hagl_hal_palette = palette;
    setup_palette(palette);
}

bitmap_t *hagl_hal_init(void)
{
    if (hagl_hal_palette == NULL)
    {
        hagl_hal_palette = (uint16_t *)hagl_hal_default_palette_16;
    }
    setup_video(hgal_hal_vga_mode, hagl_hal_bpp, hagl_hal_palette);
    /* This HAL does not use double buffering so we return NULL. */
    return NULL;
}

void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    plot_point(x0, y0, color);
}

color_t hagl_hal_get_pixel(int16_t x0, int16_t y0)
{
    uint8_t clr = get_point(x0, y0);
    color_t color = hagl_hal_palette != NULL
                        ? hagl_hal_palette[clr]
                        : PICO_SCANVIDEO_PIXEL_FROM_RGB8(0, 0, 0);
    return color;
}

void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t w, color_t color) {
    int16_t x = x0;
    while (x < x0 + w) {
        plot_point(x, y0, color);
        x++;
    }
}

void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t w, color_t color) {
    int16_t y = y0;
    while (y < y0 + w) {
        plot_point(x0, y, color);
        y++;
    }
}

// EOF
