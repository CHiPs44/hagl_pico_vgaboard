/*

MIT License

Copyright (c) 2021-2022 Christophe Petit

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
#include "pico-vgaboard-framebuffer.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"

bitmap_t *hagl_hal_init(void)
{
    printf("HAGL HAL INIT\n");
    /* This HAL does not use double buffering so we return NULL. */
    return NULL;
}

void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    vgaboard_put_pixel(x0, y0, color);
}

color_t hagl_hal_get_pixel(int16_t x0, int16_t y0)
{
    color_t color = vgaboard_get_pixel_color(x0, y0);
    return color;
}

void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t w, color_t color)
{
    int16_t x = x0;
    while (x < x0 + w)
    {
        vgaboard_put_pixel(x, y0, color);
        x++;
    }
}

void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t w, color_t color)
{
    int16_t y = y0;
    while (y < y0 + w)
    {
        vgaboard_put_pixel(x0, y, color);
        y++;
    }
}

// EOF
