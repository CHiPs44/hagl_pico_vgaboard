/*

MIT License

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

#ifdef __cplusplus
extern "C"
{
#endif

/* cf. https://www.cpcwiki.eu/index.php/CPC_Palette */

#DEFINE CPC_00_BLACK            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x00) /* #000000 */
#DEFINE CPC_01_BLUE             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0x80) /* #000080 */
#DEFINE CPC_02_BRIGHT_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x00, 0xff) /* #0000FF */
#DEFINE CPC_03_RED              PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x00) /* #800000 */
#DEFINE CPC_04_MAGENTA          PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x00) /* #FF0000 */
#DEFINE CPC_05_MAUVE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0x80) /* #800080 */
#DEFINE CPC_06_BRIGHT_RED       PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x00, 0xff) /* #8000FF */
#DEFINE CPC_07_PURPLE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0x80) /* #FF0080 */
#DEFINE CPC_08_BRIGHT_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x00, 0xff) /* #FF00FF */
#DEFINE CPC_09_GREEN            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x00) /* #008000 */
#DEFINE CPC_10_CYAN             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0x80) /* #008080 */
#DEFINE CPC_11_SKY_BLUE         PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0x80, 0xff) /* #0080FF */
#DEFINE CPC_12_YELLOW           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x00) /* #808000 */
#DEFINE CPC_13_WHITE            PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0x80) /* #808080 */
#DEFINE CPC_14_PASTEL_BLUE      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0x80, 0xff) /* #8080FF */
#DEFINE CPC_15_ORANGE           PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x00) /* #FF8000 */
#DEFINE CPC_16_PINK             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0x80) /* #FF8080 */
#DEFINE CPC_17_PASTEL_MAGENTA   PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0x80, 0xff) /* #FF80FF */
#DEFINE CPC_18_BRIGHT_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x00) /* #00FF00 */
#DEFINE CPC_19_SEA_GREEN        PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0x80) /* #00FF80 */
#DEFINE CPC_20_BRIGHT_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 0xff, 0xff) /* #00FFFF */
#DEFINE CPC_21_LIME             PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x00) /* #80FF00 */
#DEFINE CPC_22_PASTEL_GREEN     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0x80) /* #80FF80 */
#DEFINE CPC_23_PASTEL_CYAN      PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x80, 0xff, 0xff) /* #80FFFF */
#DEFINE CPC_24_BRIGHT_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x00) /* #FFFF00 */
#DEFINE CPC_25_PASTEL_YELLOW    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0x80) /* #FFFF80 */
#DEFINE CPC_26_BRIGHT_WHITE     PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff) /* #FFFFFF */

/* cf. https://www.manualslib.com/manual/851404/Amstrad-Cpc-6128.html?page=63 */

/** @brief Amstrad CPC default palette for mode 0 - 160x200 / 16 colors */
uint16_t vgaboard_palette_4bpp_cpc_mode0[] = {
    CPC_01_BLUE,
    CPC_24_BRIGHT_YELLOW,
    CPC_20_BRIGHT_CYAN,
    CPC_06_BRIGHT_RED,
    CPC_26_BRIGHT_WHITE,
    CPC_00_BLACK,
    CPC_02_BRIGHT_BLUE,
    CPC_08_BRIGHT_MAGENTA,
    CPC_10_CYAN,
    CPC_12_YELLOW,
    CPC_14_PASTEL_BLUE,
    CPC_16_PINK,
    CPC_18_BRIGHT_GREEN,
    CPC_22_PASTEL_GREEN,
    CPC_01_BLUE, // Flashing with CPC_24_BRIGHT_YELLOW
    CPC_16_PINK, // Flashing with CPC_11_SKY_BLUE
};

/** @brief Amstrad CPC default palette for mode 1 - 320x200 / 4 colors  */
uint16_t vgaboard_palette_2bpp_cpc_mode1[] = {
    CPC_01_BLUE,
    CPC_24_BRIGHT_YELLOW,
    CPC_20_BRIGHT_CYAN,
    CPC_06_BRIGHT_RED
};

/** @brief Amstrad CPC default palette for mode 2 - 630x200 / 2 colors  */
uint16_t vgaboard_palette_1bpp_cpc_mode2[] = {
    CPC_01_BLUE,
    CPC_24_BRIGHT_YELLOW
};

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_PALETTES_CPC_H */
