/*

MIT License

Copyright (c) 2021-2024 CHiPs44 <chips44@gmail.com>

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

#include "hagl.h"
#include "hagl_hal.h"
#include "pico-vgaboard.h"

void inline 
hagl_hal_put_pixel(void *self, int16_t x0, int16_t y0, hagl_color_t color)
{
    pico_vgaboard_put_pixel(x0, y0, color);
}

hagl_color_t inline 
hagl_hal_get_pixel(void *self, int16_t x0, int16_t y0)
{
    hagl_color_t color = pico_vgaboard_get_pixel_color(x0, y0);
    return color;
}

void 
hagl_hal_hline(void *self, int16_t x0, int16_t y0, uint16_t w, hagl_color_t color)
{
    int16_t x = x0;
    while (x < x0 + w)
    {
        hagl_hal_put_pixel(self, x++, y0, color);
    }
}

void 
hagl_hal_vline(void *self, int16_t x0, int16_t y0, uint16_t h, hagl_color_t color)
{
    int16_t y = y0;
    while (y < y0 + h)
    {
        hagl_hal_put_pixel(self, x0, y++, color);
    }
}

void 
hagl_hal_dump(hagl_backend_t *backend)
{
    printf("------------------------------\n");
    printf("WxHxD: %dx%dx%d\n", backend->width, backend->height, backend->depth);
    printf("Clip: (%d,%d)-(%d,%d)\n", backend->clip.x0, backend->clip.y0, backend->clip.x1, backend->clip.y1);
    printf("put_pixel: %p, get_pixel: %p,\n", backend->put_pixel, backend->get_pixel);
    printf("hline: %p, vline: %p,\n", backend->hline, backend->vline);
    printf("------------------------------\n");
}

void 
hagl_hal_init(hagl_backend_t *hagl_backend)
{
#if HAGL_HAL_DEBUG
    printf("HAGL HAL INIT: BEGIN\n");
    hagl_hal_dump(hagl_backend);
#endif
    hagl_backend->width = pico_vgaboard->display_width;
    hagl_backend->height = pico_vgaboard->display_height;
    hagl_backend->depth = pico_vgaboard->depth;
    hagl_backend->put_pixel = hagl_hal_put_pixel;
    hagl_backend->get_pixel = hagl_hal_get_pixel;
    hagl_backend->hline = hagl_hal_hline;
    hagl_backend->vline = hagl_hal_vline;
    hagl_set_clip(hagl_backend, 0, 0, hagl_backend->width - 1, hagl_backend->height - 1);
#if HAGL_HAL_DEBUG
    hagl_hal_dump(hagl_backend);
    printf("HAGL HAL INIT: END\n");
#endif
}

// EOF
