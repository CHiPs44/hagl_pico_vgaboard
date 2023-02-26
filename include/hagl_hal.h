/*

MIT License

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

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

#ifndef _PICO_VGABOARD_HAL_H
#define _PICO_VGABOARD_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <hagl/backend.h>
#include <hagl/color.h>
#include "pico/scanvideo.h"

#ifndef HAGL_HAL_DEBUG
#define HAGL_HAL_DEBUG 0
#endif

    /** @brief Initialize the HAL */
    void
    hagl_hal_init(hagl_backend_t *backend);

    /**
     * @brief 
     * 
     * @param backend 
     */
    void 
    hagl_hal_dump(hagl_backend_t *backend);

    /**
     * @brief Convert RGB to HAL color type
     *
     * This is used for HAL implementations which use some other pixel
     * format than RGB555.
     */
    static inline hagl_color_t 
    hagl_hal_color(uint8_t r, uint8_t g, uint8_t b)
    {
        hagl_color_t rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
        return rgb;
    }

    /**
     * @brief Put a pixel on the screen
     *
     * This is the only mandatory function HAL must provide.
     */
    void hagl_hal_put_pixel(void *self, int16_t x0, int16_t y0, hagl_color_t color);

    /**
     * @brief Get color of a single pixel
     *
     * @param x0 X coordinate
     * @param y0 Y coordinate
     *
     * @return pixel color at given coordinates
     */
    hagl_color_t hagl_hal_get_pixel(void *self, int16_t x0, int16_t y0);

    /**
     * @brief Draw an horizontal line
     */
    void hagl_hal_hline(void *self, int16_t x0, int16_t y0, uint16_t w, hagl_color_t color);

    /**
     * @brief Draw a vertical line
     */
    void hagl_hal_vline(void *self, int16_t x0, int16_t y0, uint16_t h, hagl_color_t color);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_HAL_H */
