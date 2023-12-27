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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_H
#define _HAGL_PICO_VGABOARD_PALETTES_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief Default white on black */
extern const BGAR5515 pico_vgaboard_palette_1bpp_default[2];

/** @brief Green CRT monitor green on black */
extern const BGAR5515 pico_vgaboard_palette_1bpp_green[2];

/** @brief Amber CRT monitor orange on black */
extern const BGAR5515 pico_vgaboard_palette_1bpp_amber[2];

/** @brief Default white on black */
extern const BGAR5515 pico_vgaboard_palette_2bpp_default[4];

/** @brief Green CRT monitor green on black */
extern const BGAR5515 pico_vgaboard_palette_2bpp_green[4];

/** @brief Amber CRT monitor orange on black */
extern const BGAR5515 pico_vgaboard_palette_2bpp_amber[4];

/** @brief IRGB 16 colors palette with dark and light grey in order of luminosity */
extern const BGAR5515 pico_vgaboard_palette_4bpp_default[16];

/** @brief IRGB 8 bpp / 256 colors VGA default palette */
extern const BGAR5515 pico_vgaboard_palette_8bpp_default[256];

/** @brief RGB685 240 + 16 gray levels = 256 colors palette */
extern const BGAR5515 pico_vgaboard_palette_8bpp_rgb685[256];

/** @brief Aurora 256 colors palette from Dawnbringer */
extern const BGAR5515 pico_vgaboard_palette_8bpp_aurora[256];

/** @brief RGB332 256 colors palette */
extern const BGAR5515 pico_vgaboard_palette_8bpp_rgb332[256];

/** @brief "ANSI" 256 colors palette */
extern const BGAR5515 pico_vgaboard_palette_8bpp_ansi[256];

/** @brief Empty palette for 16bpp modes */
extern const BGAR5515 pico_vgaboard_palette_16bpp_empty[0];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_H */
