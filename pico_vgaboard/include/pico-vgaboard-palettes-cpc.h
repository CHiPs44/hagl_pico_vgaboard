/*

MIT License

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

#ifndef _HAGL_PICO_VGABOARD_PALETTES_CPC_H
#define _HAGL_PICO_VGABOARD_PALETTES_CPC_H

#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://www.cpcwiki.eu/index.php/CPC_Palette */

#define CPC_00_BLACK            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00) /* #000000 */
#define CPC_01_BLUE             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x80) /* #000080 */
#define CPC_02_BRIGHT_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff) /* #0000FF */
#define CPC_03_RED              PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x00) /* #800000 */
#define CPC_04_MAGENTA          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00) /* #FF0000 */
#define CPC_05_MAUVE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x80) /* #800080 */
#define CPC_06_BRIGHT_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0xff) /* #8000FF */
#define CPC_07_PURPLE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x80) /* #FF0080 */
#define CPC_08_BRIGHT_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff) /* #FF00FF */
#define CPC_09_GREEN            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00) /* #008000 */
#define CPC_10_CYAN             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x80) /* #008080 */
#define CPC_11_SKY_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0xff) /* #0080FF */
#define CPC_12_YELLOW           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x00) /* #808000 */
#define CPC_13_WHITE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x80) /* #808080 */
#define CPC_14_PASTEL_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0xff) /* #8080FF */
#define CPC_15_ORANGE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x00) /* #FF8000 */
#define CPC_16_PINK             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x80) /* #FF8080 */
#define CPC_17_PASTEL_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xff) /* #FF80FF */
#define CPC_18_BRIGHT_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00) /* #00FF00 */
#define CPC_19_SEA_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x80) /* #00FF80 */
#define CPC_20_BRIGHT_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff) /* #00FFFF */
#define CPC_21_LIME             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x00) /* #80FF00 */
#define CPC_22_PASTEL_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x80) /* #80FF80 */
#define CPC_23_PASTEL_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0xff) /* #80FFFF */
#define CPC_24_BRIGHT_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00) /* #FFFF00 */
#define CPC_25_PASTEL_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x80) /* #FFFF80 */
#define CPC_26_BRIGHT_WHITE     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff) /* #FFFFFF */

/* cf. https://www.manualslib.com/manual/851404/Amstrad-Cpc-6128.html?page=63 */

/** @brief Amstrad CPC default palette for mode 0 - 160x200 / 16 colors */
extern const BGAR5515 pico_vgaboard_palette_4bpp_cpc0[16];

/** @brief Amstrad CPC default palette for mode 1 - 320x200 / 4 colors  */
extern const BGAR5515 pico_vgaboard_palette_2bpp_cpc1[4];

/** @brief Amstrad CPC default palette for mode 2 - 640x200 / 2 colors  */
extern const BGAR5515 pico_vgaboard_palette_1bpp_cpc2[2];

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_CPC_H */
