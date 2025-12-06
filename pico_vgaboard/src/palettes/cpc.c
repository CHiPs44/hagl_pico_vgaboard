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
#include "palettes/cpc.h"

const BGAR5515 palette_4bpp_cpc0[16] = {
    /* 00 */ CPC_01_BLUE,
    /* 01 */ CPC_24_BRIGHT_YELLOW,
    /* 02 */ CPC_20_BRIGHT_CYAN,
    /* 03 */ CPC_06_BRIGHT_RED,
    /* 04 */ CPC_26_BRIGHT_WHITE,
    /* 05 */ CPC_00_BLACK,
    /* 06 */ CPC_02_BRIGHT_BLUE,
    /* 07 */ CPC_08_BRIGHT_MAGENTA,
    /* 08 */ CPC_10_CYAN,
    /* 09 */ CPC_12_YELLOW,
    /* 10 */ CPC_14_PASTEL_BLUE,
    /* 11 */ CPC_16_PINK,
    /* 12 */ CPC_18_BRIGHT_GREEN,
    /* 13 */ CPC_22_PASTEL_GREEN,
    /* 14 */ CPC_01_BLUE, // should be flashing with CPC_24_BRIGHT_YELLOW
    /* 15 */ CPC_16_PINK, // should be flashing with CPC_11_SKY_BLUE
};

const BGAR5515 palette_2bpp_cpc1[4] = {
    /* 00 */ CPC_01_BLUE,
    /* 01 */ CPC_24_BRIGHT_YELLOW,
    /* 02 */ CPC_20_BRIGHT_CYAN,
    /* 03 */ CPC_06_BRIGHT_RED
};

const BGAR5515 palette_1bpp_cpc2[2] = {
    /* 00 */ CPC_01_BLUE,
    /* 01 */ CPC_24_BRIGHT_YELLOW
};
