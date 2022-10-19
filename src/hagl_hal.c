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

#include "hagl_hal.h"
#include "pico-vgaboard-framebuffer.h"
// #include "pico/scanvideo.h"
// #include "pico/scanvideo/scanvideo_base.h"

void put_pixel(void *self, int16_t x0, int16_t y0, color_t color)
{
    vgaboard_put_pixel(x0, y0, color);
}

color_t get_pixel(void *self, int16_t x0, int16_t y0)
{
    color_t color = vgaboard_get_pixel_color(x0, y0);
    return color;
}

void hal_hline(void *self, int16_t x0, int16_t y0, uint16_t w, color_t color)
{
    int16_t x = x0;
    while (x < x0 + w)
    {
        vgaboard_put_pixel(x++, y0, color);
    }
}

void hal_vline(void *self, int16_t x0, int16_t y0, uint16_t h, color_t color)
{
    int16_t y = y0;
    while (y < y0 + h)
    {
        vgaboard_put_pixel(x0, y, color);
        y++;
    }
}

static hagl_backend_t *hagl_hal_backend = NULL;

void hagl_hal_init(hagl_backend_t *backend)
{
#if HAGL_HAL_DEBUG
    printf("HAGL HAL INIT\n");
#endif
    backend->width = DISPLAY_WIDTH;
    backend->height = DISPLAY_HEIGHT;
    backend->depth = DISPLAY_DEPTH;
    backend->put_pixel = put_pixel;
    backend->get_pixel = get_pixel;
    backend->hline = hal_hline;
    backend->vline = hal_vline;
    hagl_hal_backend = backend;
}

void hagl_hal_set_width(int16_t width)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->width = width;
    }
}

void hagl_hal_set_height(int16_t height)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->height = height;
    }
}

void hagl_hal_set_depth(uint8_t depth)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->depth = depth;
    }
}

int16_t hagl_hal_get_width()
{
    return hagl_hal_backend == NULL ? 0 : hagl_hal_backend->width;
}

int16_t hagl_hal_get_height()
{
    return hagl_hal_backend == NULL ? 0 : hagl_hal_backend->height;
}

uint8_t hagl_hal_get_depth()
{
    return hagl_hal_backend == NULL ? 0 : hagl_hal_backend->depth;
}

// EOF
