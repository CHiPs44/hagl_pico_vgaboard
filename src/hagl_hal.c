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

void hagl_hal_put_pixel(void *self, int16_t x0, int16_t y0, color_t color)
{
    hagl_window_t clip = ((hagl_backend_t *)(self))->clip;
    if (x0 < clip.x0 || x0 > clip.x1 || y0 < clip.y0 || y0 > clip.y1)
    {
        return;
    }
    vgaboard_put_pixel(x0, y0, color);
}

color_t hagl_hal_get_pixel(void *self, int16_t x0, int16_t y0)
{
    color_t color = vgaboard_get_pixel_color(x0, y0);
    return color;
}

void hagl_hal_hline(void *self, int16_t x0, int16_t y0, uint16_t w, color_t color)
{
    int16_t x = x0;
    while (x < x0 + w)
    {
        hagl_hal_put_pixel(self, x++, y0, color);
    }
}

void hagl_hal_vline(void *self, int16_t x0, int16_t y0, uint16_t h, color_t color)
{
    int16_t y = y0;
    while (y < y0 + h)
    {
        hagl_hal_put_pixel(self, x0, y, color);
        y++;
    }
}

void hagl_hal_dump(hagl_backend_t *backend)
{
    puts("------------------------------");
    printf("WxH: %dx%d\n", backend->width, backend->height);
    printf("Clip: (%d,%d)-(%d,%d)\n", backend->clip.x0, backend->clip.y0, backend->clip.x1, backend->clip.y1);
    printf("put_pixel: %p, get_pixel: %p,\n", backend->put_pixel, backend->get_pixel);
    puts("------------------------------");
}

static hagl_backend_t *hagl_hal_backend = NULL;

void hagl_hal_init(hagl_backend_t *backend)
{
#if HAGL_HAL_DEBUG
    printf("HAGL HAL INIT\n");
    hagl_hal_dump(backend);
#endif
    backend->width = DISPLAY_WIDTH;
    backend->height = DISPLAY_HEIGHT;
    backend->depth = DISPLAY_DEPTH;
    backend->put_pixel = hagl_hal_put_pixel;
    backend->get_pixel = hagl_hal_get_pixel;
    backend->hline = hagl_hal_hline;
    backend->vline = hagl_hal_vline;
#if HAGL_HAL_DEBUG
    hagl_hal_dump(backend);
#endif
    hagl_hal_backend = backend;
}

void hagl_hal_set_width(int16_t width)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->width = width;
#if HAGL_HAL_DEBUG
        hagl_hal_dump(hagl_hal_backend->backend);
#endif
    }
}

void hagl_hal_set_height(int16_t height)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->height = height;
#if HAGL_HAL_DEBUG
        hagl_hal_dump(hagl_hal_backend->backend);
#endif
    }
}

void hagl_hal_set_depth(uint8_t depth)
{
    if (hagl_hal_backend != NULL)
    {
        hagl_hal_backend->depth = depth;
#if HAGL_HAL_DEBUG
        hagl_hal_dump(hagl_hal_backend->backend);
#endif
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
