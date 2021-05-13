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
https://github.com/CHiPs44/hagl_pico_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <stdio.h>
#include <stdint.h>

// #include "hardware/clocks.h"
// #include "pico.h"
// #include "pico/multicore.h"
// #include "pico/scanvideo/composable_scanline.h"
// #include "pico/stdlib.h"

#include "bitmap.h"
#include "hagl_hal.h"
#include "pico-vga-framebuffer.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"

/* IRGB palette from pico-vga-framebuffer */

// /*                                                        RED  GREEN BLUE  */
// /* Let's go for the dark colors */
#define PAL16_BLACK        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u,   0u)
#define PAL16_DARK_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u,   0u)
#define PAL16_DARK_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u,   0u)
#define PAL16_DARK_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u,   0u)
#define PAL16_DARK_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 128u)
#define PAL16_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u, 128u)
#define PAL16_DARK_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u, 128u)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define PAL16_DARK_GREY    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
#define PAL16_LIGHT_GREY   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
/* And then the brighter ones */
#define PAL16_RED          PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u,   0u)
#define PAL16_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u,   0u)
#define PAL16_YELLOW       PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u,   0u)
#define PAL16_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 255u)
#define PAL16_MAGENTA      PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u, 255u)
#define PAL16_CYAN         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u, 255u)
#define PAL16_WHITE        PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 255u)

const uint16_t hagl_hal_default_palette[] = {
    /* 00 */ PAL16_BLACK,
    /* 01 */ PAL16_DARK_RED,
    /* 02 */ PAL16_DARK_GREEN,
    /* 03 */ PAL16_DARK_YELLOW,
    /* 04 */ PAL16_DARK_BLUE,
    /* 05 */ PAL16_DARK_MAGENTA,
    /* 06 */ PAL16_DARK_CYAN,
    /* 07 */ PAL16_DARK_GREY,
    /* 08 */ PAL16_LIGHT_GREY,
    /* 09 */ PAL16_RED,
    /* 10 */ PAL16_GREEN,
    /* 11 */ PAL16_YELLOW,
    /* 12 */ PAL16_BLUE,
    /* 13 */ PAL16_MAGENTA,
    /* 14 */ PAL16_CYAN,
    /* 15 */ PAL16_WHITE
};

scanvideo_mode_t *hgal_hal_vga_mode = NULL;
uint16_t *hagl_hal_palette = NULL;

void hagl_hal_set_vga_mode(const scanvideo_mode_t *vga_mode)
{
    hgal_hal_vga_mode = (scanvideo_mode_t *)vga_mode;
}

void hagl_hal_set_palette(const uint16_t *palette)
{
    hagl_hal_palette = (uint16_t *)palette;
}

uint16_t *hagl_hal_get_palette()
{
    return hagl_hal_palette;
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
        hagl_hal_palette = (uint16_t *)hagl_hal_default_palette;
    }
    setup_video(hgal_hal_vga_mode, hagl_hal_palette);
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

// EOF
