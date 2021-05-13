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

#ifndef _HAGL_PICO_VGABOARD_HAL_H
#define _HAGL_PICO_VGABOARD_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "pico/scanvideo.h"
#include <bitmap.h>

#define hagl_hal_debug(fmt, ...) \
    do { if (HAGL_HAL_DEBUG) printf("[HAGL HAL] " fmt, __VA_ARGS__); } while (0)

/* HAL must provide display dimensions and depth.
/* This HAL defaults to 640x480x16 colors. */
#ifndef VGA_MODE
#define VGA_MODE        (&vga_mode_640x480_60)
#endif
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH   (640)
#endif
#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT  (480)
#endif
/* Colors: 16 is 2^_4_ */
#define DISPLAY_DEPTH   (4)

/*                                                        RED  GREEN BLUE  */
/* Let's go for the dark colors */
#define PAL16_BLACK        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00)
#define PAL16_DARK_RED     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x00)
#define PAL16_DARK_GREEN   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00)
#define PAL16_DARK_YELLOW  PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x00)
#define PAL16_DARK_BLUE    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x80)
#define PAL16_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x80)
#define PAL16_DARK_CYAN    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x80)
/* NB: light and dark grey are evenly distributed to make a grayscale with black and white */
#define PAL16_DARK_GREY    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
#define PAL16_LIGHT_GREY   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
/* And then the brighter ones */
#define PAL16_RED          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00)
#define PAL16_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00)
#define PAL16_YELLOW       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00)
#define PAL16_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff)
#define PAL16_MAGENTA      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff)
#define PAL16_CYAN         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff)
#define PAL16_WHITE        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff)

/**
 * Added for this HAL: set VGA video mode
 */
// void hagl_hal_set_vga_mode(const scanvideo_mode_t *vga_mode);

/* These are the optional features this HAL provides. */
#define HAGL_HAS_HAL_INIT
#define HAGL_HAS_HAL_COLOR
#define HAGL_HAS_HAL_GET_PIXEL

/** 
 * HAL must provide typedef for colors. 
 * This HAL uses a palette of 16 IRGB colors (Intensity, Red, Green, Blue, each on one bit).
 * NB: scanvideo still needs 16 bits to store colors
 */
typedef uint16_t color_t;

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
 * @brief Convert RGB to HAL color type
 *
 * This is used for HAL implementations which use some other pixel
 * format than RGB565.
 */
static inline color_t hagl_hal_color(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t color_trgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
    return rgb;
}

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
 * @brief Draw a single pixel
 *
 * This is the only mandatory function HAL must provide.
 *
 * @param x0 X coordinate
 * @param y0 Y coordinate
 *
 * @return pixel color at given coordinates
 */
color_t hagl_hal_get_pixel(int16_t x0, int16_t y0);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_PICO_VGABOARD_HAL_H */
