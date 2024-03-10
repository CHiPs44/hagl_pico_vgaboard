/*

MIT License

Copyright (c) 2024 CHiPs44 <chips44@gmail.com>

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

#ifndef _HAGL_EXT_H
#define _HAGL_EXT_H

#include "hagl/bitmap.h"
#include "hagl/window.h"
#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _hagl_ext_surface_t {
    void (*blit_alpha)(void *self, int16_t x0, int16_t y0, hagl_bitmap_t *src, hagl_color_t alpha);
    void (*scale_blit_alpha)(void *self, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *src, hagl_color_t alpha);
    void *_surface;
} hagl_ext_surface_t;

/**
 * @brief Initialize HAGL & Extension
 * 
 * @param hagl_ext_backend 
 */
void 
hagl_ext_init(hagl_ext_surface_t *hagl_ext_backend);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_EXT_H */
