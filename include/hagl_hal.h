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
#include "bitmap.h"

#define hagl_hal_debug(fmt, ...) \
    do { if (HAGL_HAL_DEBUG) printf("[HAGL HAL] " fmt, __VA_ARGS__); } while (0)

/* HAL must provide display dimensions and depth.
/* This HAL defaults to 640x480x16 colors. */
#ifndef VGA_MODE
#define VGA_MODE        (&vga_mode_640x480_60)
#define DISPLAY_WIDTH   (640)
#define DISPLAY_HEIGHT  (480)
#endif
/* Colors: 16 is 2^_4_ */
#define DISPLAY_DEPTH   (4)

extern const uint16_t hagl_hal_default_palette[];

/* These are the optional features this HAL provides. */
#define HAGL_HAS_HAL_INIT
#define HAGL_HAS_HAL_COLOR
#define HAGL_HAS_HAL_GET_PIXEL
#define HAGL_HAS_HAL_HLINE
#define HAGL_HAS_HAL_VLINE

/** 
 * HAL must provide typedef for colors.
 * 
 * This HAL uses a default palette of 16 IRGB colors
 * (Intensity, Red, Green, Blue, each on one bit).
 * 
 * NB: scanvideo still needs 16 bits to store colors
 */
typedef uint16_t color_t;

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
    color_t rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
    return rgb;
}

/**
 * @brief Put a pixel on the screen
 * 
 * This is the only mandatory function HAL must provide.
 * (hence no HAGL_HAS_HAL_PUT_PIXEL define)
 * 
 * NB : color is an rgb555 value, e.g. a PAL16_xxxx value, 
 * not an index in the 16 colours palette
 */
void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color);

/**
 * @brief Get color of a single pixel
 *
 * This is the only mandatory function HAL must provide.
 *
 * @param x0 X coordinate
 * @param y0 Y coordinate
 *
 * @return pixel color at given coordinates
 */
color_t hagl_hal_get_pixel(int16_t x0, int16_t y0);

/**
 * @brief Draw an horizontal line
 */
void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t w, color_t color);

/**
 * @brief Draw a vertical line
 */
void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t w, color_t color);

/**
 * Specific to this HAL that makes it not so abstract ;-)
 */

/**
 * @brief Set VGA mode
 */
void hagl_hal_set_vga_mode(const scanvideo_mode_t *vga_mode);

/**
 * @brief Set bits per pixel
 */
void hagl_hal_set_bpp(uint8_t bpp);

/**
 * @brief Set bits per pixel
 */
uint8_t hagl_hal_get_bpp();


/**
 * @brief Set palette before setup_video()
 */
void hagl_hal_set_palette(const uint16_t *palette);

/**
 * @brief Change palette after setup_video()
 */
void hagl_hal_change_palette(uint16_t *palette);

/**
 * @brief Get current palette
 */
uint16_t *hagl_hal_get_palette();

/**
 * @brief Get color for given index in palette 
 */
color_t hagl_hal_get_color(uint8_t index);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_PICO_VGABOARD_HAL_H */
