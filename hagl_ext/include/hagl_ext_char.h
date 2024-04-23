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

#ifndef _HAGL_EXT_CHAR_H
#define _HAGL_EXT_CHAR_H

#include <stdint.h>
#include <wchar.h>

#include "hagl/color.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _hagl_ext_char_mode_t {
    HAGL_EXT_CHAR_MODE_OPAQUE      = 0x00,
    HAGL_EXT_CHAR_MODE_REVERSE     = 0x01,
    HAGL_EXT_CHAR_MODE_TRANSPARENT = 0x02
} hagl_ext_char_mode_t;

typedef struct _hagl_ext_char_style_t {
    const uint8_t    *font;
    hagl_color_t      background_color;
    hagl_color_t      foreground_color;
    hagl_ext_char_mode_t  mode;
    uint16_t          scale_x_numerator;
    uint16_t          scale_x_denominator;
    uint16_t          scale_y_numerator;
    uint16_t          scale_y_denominator;
} hagl_ext_char_style_t;

/**
 * Draw a single character, extended version
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/embedded-fonts
 *
 * @param _surface
 * @param code  unicode code point
 * @param x0
 * @param y0
 * @param style
 * @return width of the drawn character
 */
uint8_t
hagl_ext_put_char(hagl_ext_surface_t *ext_surface, wchar_t code, int16_t x0, int16_t y0, const hagl_ext_char_style_t *style);

/**
 * Draw a string, extended version
 *
 * Output will be clipped to the current clip window. Library itself
 * includes only a couple of fonts. You can find more fonts at:
 *
 * https://github.com/tuupola/embedded-fonts
 *
 * @param _surface
 * @param str pointer to an wide char string
 * @param x0
 * @param y0
 * @param style
 * @return width of the drawn string
 */
uint16_t
hagl_ext_put_text(hagl_ext_surface_t *ext_surface, const wchar_t *str, int16_t x0, int16_t y0, const hagl_ext_char_style_t *style);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAGL_EXT_CHAR_H */
