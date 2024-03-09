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

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:

https://github.com/CHiPs44/hagl_pico_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#ifndef _PICO_VGABOARD_PALETTES_H
#define _PICO_VGABOARD_PALETTES_H

#include "pico-vgaboard.h"

/* clang-format off */

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************/
/* 1 BPP / 2 COLORS                                                    */
/***********************************************************************/

/** @brief Default 2 colors palette */
extern const BGAR5515 *palette_1bpp_default;

/** @brief White on black */
extern const BGAR5515 palette_1bpp_black[2];

/** @brief Green CRT monitor green on black */
extern const BGAR5515 palette_1bpp_green[2];

/** @brief Amber CRT monitor orange on black */
extern const BGAR5515 palette_1bpp_amber[2];

/***********************************************************************/
/* 2 BPP / 4 COLORS                                                    */
/***********************************************************************/

/** @brief Default 4 colors palette */
extern const BGAR5515 *palette_2bpp_default;

/** @brief White on black */
extern const BGAR5515 palette_2bpp_black[4];

/** @brief Green CRT monitor green on black */
extern const BGAR5515 palette_2bpp_green[4];

/** @brief Amber CRT monitor orange on black */
extern const BGAR5515 palette_2bpp_amber[4];

/** @brief Four shades of grey from white to black */
extern const BGAR5515 palette_2bpp_paper[4];

/***********************************************************************/
/* 4 BPP / 16 COLORS                                                   */
/***********************************************************************/

/** @brief IRGB 16 colors palette with dark and light grey in order of luminosity */
extern const BGAR5515 palette_4bpp_ansi[16];

/** @brief Default Atari STE color palette (white background, RGB444) */
extern const BGAR5515 palette_4bpp_atari_ste[16];

/** @brief Default 16 colors palette => IRGB */
extern const BGAR5515 *palette_4bpp_default;

/***********************************************************************/
/* 8 BPP / 256 COLORS                                                  */
/***********************************************************************/

/** @brief Default 256 colors palette => ANSI */
extern const BGAR5515 *palette_8bpp_default;

/** @brief RGBI 8 bpp / 256 colors */
extern const BGAR5515 palette_8bpp_rgbi[256];

/** @brief RGB685 240 + 16 gray levels = 256 colors palette */
extern const BGAR5515 palette_8bpp_rgb685[256];

/** @brief Aurora 256 colors palette from Dawnbringer */
extern const BGAR5515 palette_8bpp_aurora[256];

/** @brief RGB332 256 colors palette */
extern const BGAR5515 palette_8bpp_rgb332[256];

/** @brief "ANSI" 256 colors palette */
extern const BGAR5515 palette_8bpp_ansi[256];

/***********************************************************************/
/* 16 BPP / 65536 COLORS                                               */
/***********************************************************************/

/** @brief Empty palette for 16bpp modes */
extern const BGAR5515 palette_16bpp_empty[0];

#ifdef __cplusplus
}
#endif

/* clang-format on */

#endif /* _PICO_VGABOARD_PALETTES_H */
