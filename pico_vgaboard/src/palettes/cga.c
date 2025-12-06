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

#include "pico/scanvideo.h"

#include "colors.h"
#include "palettes/cga.h"

const BGAR5515 palette_1bpp_cga_mono[2] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_15_WHITE,
};

const BGAR5515 palette_2bpp_cga_0_high[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_10_LIGHT_GREEN,
    /* 02 */ CGA_12_LIGHT_RED,
    /* 03 */ CGA_06_BROWN,
};

const BGAR5515 palette_2bpp_cga_0_low[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_02_GREEN,
    /* 02 */ CGA_04_RED,
    /* 03 */ CGA_14_YELLOW,
};

const BGAR5515 palette_2bpp_cga_1_high[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_13_LIGHT_MAGENTA,
    /* 02 */ CGA_11_LIGHT_CYAN,
    /* 03 */ CGA_15_WHITE,
};

const BGAR5515 palette_2bpp_cga_1_low[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_05_MAGENTA,
    /* 02 */ CGA_03_CYAN,
    /* 03 */ CGA_15_WHITE,
};

const BGAR5515 palette_2bpp_cga_2_high[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_12_LIGHT_RED,
    /* 02 */ CGA_11_LIGHT_CYAN,
    /* 03 */ CGA_15_WHITE,
};

const BGAR5515 palette_2bpp_cga_2_low[4] = {
    /* 00 */ CGA_00_BLACK,
    /* 01 */ CGA_04_RED,
    /* 02 */ CGA_03_CYAN,
    /* 03 */ CGA_07_LIGHT_GRAY,
};

const BGAR5515 palette_4bpp_cga_old[16] = {
    CGA_00_BLACK,
    CGA_01_BLUE,
    CGA_02_GREEN,
    CGA_03_CYAN,
    CGA_04_RED,
    CGA_05_MAGENTA,
    CGA_06_DARK_YELLOW,
    CGA_07_LIGHT_GRAY,
    CGA_08_DARK_GRAY,
    CGA_09_LIGHT_BLUE,
    CGA_10_LIGHT_GREEN,
    CGA_11_LIGHT_CYAN,
    CGA_12_LIGHT_RED,
    CGA_13_LIGHT_MAGENTA,
    CGA_14_YELLOW,
    CGA_15_WHITE,
};

const BGAR5515 palette_4bpp_cga[16] = {
    CGA_00_BLACK,
    CGA_01_BLUE,
    CGA_02_GREEN,
    CGA_03_CYAN,
    CGA_04_RED,
    CGA_05_MAGENTA,
    CGA_06_BROWN,
    CGA_07_LIGHT_GRAY,
    CGA_08_DARK_GRAY,
    CGA_09_LIGHT_BLUE,
    CGA_10_LIGHT_GREEN,
    CGA_11_LIGHT_CYAN,
    CGA_12_LIGHT_RED,
    CGA_13_LIGHT_MAGENTA,
    CGA_14_YELLOW,
    CGA_15_WHITE,
};

/* EOF */
