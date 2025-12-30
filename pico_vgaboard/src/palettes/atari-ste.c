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

#include "palettes/atari-ste.h"
#include "colors.h"
#include "pico-vgaboard.h"

const BGAR5515 palette_1bpp_white[2] = {
    /* 00 */ ATARI_STE_00_WHITE,
    /* 01 */ ATARI_STE_15_BLACK,
};

const BGAR5515 palette_2bpp_white[4] = {
    /* 00 */ ATARI_STE_00_WHITE,
    /* 01 */ ATARI_STE_02_GREEN,
    /* 02 */ ATARI_STE_01_RED,
    /* 03 */ ATARI_STE_15_BLACK,
};

const BGAR5515 palette_4bpp_atari_ste[16] = {
    ATARI_STE_00_WHITE,
    ATARI_STE_01_RED,
    ATARI_STE_02_GREEN,
    ATARI_STE_03_YELLOW,
    ATARI_STE_04_BLUE,
    ATARI_STE_05_MAGENTA,
    ATARI_STE_06_CYAN,
    ATARI_STE_07_LOW_WHITE,
    ATARI_STE_08_GREY,
    ATARI_STE_09_LIGHT_RED,
    ATARI_STE_10_LIGHT_GREEN,
    ATARI_STE_11_LIGHT_YELLOW,
    ATARI_STE_12_LIGHT_BLUE,
    ATARI_STE_13_LIGHT_MAGENTA,
    ATARI_STE_14_LIGHT_CYAN,
    ATARI_STE_15_BLACK,
};

/* EOF */
