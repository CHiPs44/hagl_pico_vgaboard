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

#ifndef _PICO_VGABOARD_PALETTES_4BPP_H
#define _PICO_VGABOARD_PALETTES_4BPP_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /***********************************************************************/
    /* 4 BPP / 16 COLORS                                                   */
    /***********************************************************************/

    /** @brief IRGB 16 colors palette with dark and light grey in order of luminosity */
    extern const BGAR5515 palette_4bpp_ansi[16];

    /** @brief Default Atari STE color palette (white background, RGB444) */
    extern const BGAR5515 palette_4bpp_atari_ste[16];

    /** @brief Default 16 colors palette => IRGB */
    extern const BGAR5515 *palette_4bpp_default;

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_PALETTES_4BPP_H */
