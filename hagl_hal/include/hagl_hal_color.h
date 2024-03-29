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

This file is part of the GD HAL for the HAGL graphics library:
https://github.com/tuupola/hagl_gd

SPDX-License-Identifier: MIT

*/

#ifndef _HAGL_HAL_COLOR_H
#define _HAGL_HAL_COLOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef HAGL_HAL_COLOR_BITS
#define HAGL_HAL_COLOR_BITS 8
#endif

#if HAGL_HAL_COLOR_BITS == 8

/**
 * @brief HAL must provide typedef for colors.
 * This HAL uses RGAB5515 in 15bpp "true color" mode plus one alpha bit for transparency,
 * as an 8 bits palette index at
 *      => 1bpp (0-1, mask 0x1),
 *      => 2bpp (0-3, mask 0x3),
 *      => 4bpp (0-15, mask 0xf) and
 *      => 8bpp (0-255, mask 0xff).
 */
typedef uint8_t hagl_color_t;

#else

/**
 * @brief HAL must provide typedef for colors.
 * This HAL uses RGAB5515 in 15bpp "true color" mode plus one alpha bit for transparency,
 */
typedef uint16_t hagl_color_t;

#endif

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_HAL_COLOR_H */
