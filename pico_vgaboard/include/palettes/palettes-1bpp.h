/*

MIT License

Copyright (c) 2025 CHiPs44 <chips44@gmail.com>

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

#ifndef _PICO_VGABOARD_PALETTES_1BPP_H
#define _PICO_VGABOARD_PALETTES_1BPP_H

#include "pico-vgaboard.h"

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Default 2 colors palette */
extern const BGAR5515 *palette_1bpp_default;

/** @brief White on black */
extern const BGAR5515 palette_1bpp_black[2];

/** @brief Green CRT monitor green on black */
extern const BGAR5515 palette_1bpp_green[2];

/** @brief Amber CRT monitor orange on black */
extern const BGAR5515 palette_1bpp_amber[2];

/** @brief Casio LCD black on greenish */
extern const BGAR5515 palette_1bpp_casio[2];

/** @brief Sharp LCD bluish black on grey */
extern const BGAR5515 palette_1bpp_sharp[2];

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* _PICO_VGABOARD_PALETTES_1BPP_H */
