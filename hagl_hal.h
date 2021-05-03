/*

MIT License

Copyright (c) 2021 CHiPs44

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

#ifndef _HAGL_PICO_VGABOARD_HAL_H
#define _HAGL_PICO_VGABOARD_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "pico/scanvideo.h"
#include "../hagl/include/bitmap.h"

/* HAL must provide display dimensions and depth.
/* This HAL defaults to 640x480. */
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH   (640)
#endif
#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT  (480)
#endif
#define DISPLAY_DEPTH   (4)

#define PAL16_BLACK        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u,   0u)
#define PAL16_DARK_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u,   0u)
#define PAL16_DARK_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u,   0u)
#define PAL16_DARK_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u,   0u)
#define PAL16_DARK_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 128u)
#define PAL16_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u,   0u, 128u)
#define PAL16_DARK_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 128u, 128u)
#define PAL16_LIGHT_GREY   PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u, 128u)
#define PAL16_DARK_GREY    PICO_SCANVIDEO_PIXEL_FROM_RGB8( 64u,  64u,  64u)
#define PAL16_RED          PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u,   0u)
#define PAL16_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u,   0u)
#define PAL16_YELLOW       PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u,   0u)
#define PAL16_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u,   0u, 255u)
#define PAL16_MAGENTA      PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u,   0u, 255u)
#define PAL16_CYAN         PICO_SCANVIDEO_PIXEL_FROM_RGB8(  0u, 255u, 255u)
#define PAL16_WHITE        PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 255u)

/* These are the optional features this HAL provides. */
#define HAGL_HAS_HAL_INIT
#define HAGL_HAS_HAL_CLOSE
#define HAGL_HAS_HAL_COLOR

/** 
 * HAL must provide typedef for colors. 
 * This HAL uses a palette of 16 IRGB colors (Intensity, Red, Green, Blue, each on one bit).
 * NB: scanvideo still needs 16 bits to store colors
 */
typedef uint16_t color_t;

/**
 * @brief Draw a single pixel
 *
 * This is the only mandatory function HAL must provide.
 *
 * @param x0 X coordinate
 * @param y0 Y coordinate
 * @param color color
 */
void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color);

/**
 * @brief Initialize the HAL
 *
 * Initialises all hardware and possible memory buffers needed
 * to draw and display an image. If HAL uses double or triple
 * buffering should return a pointer to current back buffer.
 *
 * @return pointer to bitmap_t or NULL
 */
bitmap_t *hagl_hal_init(void);

/**
 * @brief Close and clean up the HAL
 *
 * This is used for HAL implementations which need some cleanup, such
 * as deallocating memory, to be done when closing the program.
 */
void hagl_hal_close();

/**
 * @brief Convert RGB to HAL color type
 *
 * This is used for HAL implementations which use some other pixel
 * format than RGB565.
 */
static inline color_t hagl_hal_color(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
    return rgb;
    // if (rgb==PAL16_BLACK       ) return  0;
    // if (rgb==PAL16_DARK_RED    ) return  1;
    // if (rgb==PAL16_DARK_GREEN  ) return  2;
    // if (rgb==PAL16_DARK_YELLOW ) return  3;
    // if (rgb==PAL16_DARK_BLUE   ) return  4;
    // if (rgb==PAL16_DARK_MAGENTA) return  5;
    // if (rgb==PAL16_DARK_CYAN   ) return  6;
    // if (rgb==PAL16_LIGHT_GREY  ) return  7;
    // if (rgb==PAL16_DARK_GREY   ) return  8;
    // if (rgb==PAL16_RED         ) return  9;
    // if (rgb==PAL16_GREEN       ) return 10;
    // if (rgb==PAL16_YELLOW      ) return 11;
    // if (rgb==PAL16_BLUE        ) return 12;
    // if (rgb==PAL16_MAGENTA     ) return 13;
    // if (rgb==PAL16_CYAN        ) return 14;
    // if (rgb==PAL16_WHITE       ) return 15;
    // return 0;
}

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_PICO_VGABOARD_HAL_H */
