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
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"

#include "colors.h"
#include "palettes/palettes.h"
#include "pico_vgaboard.h"

const BGAR5515 palette_4bpp_c64[16] = {
    /* 00 */ C64_00_BLACK,
    /* 01 */ C64_01_WHITE,
    /* 02 */ C64_02_RED,
    /* 03 */ C64_03_CYAN,
    /* 04 */ C64_04_PURPLE,
    /* 05 */ C64_05_GREEN,
    /* 06 */ C64_06_BLUE,
    /* 07 */ C64_07_YELLOW,
    /* 08 */ C64_08_ORANGE,
    /* 09 */ C64_09_BROWN,
    /* 10 */ C64_10_LIGHT_RED,
    /* 11 */ C64_11_DARK_GRAY,
    /* 12 */ C64_12_MID_GRAY,
    /* 13 */ C64_13_LIGHT_GREEN,
    /* 14 */ C64_14_LIGHT_BLUE,
    /* 15 */ C64_15_LIGHT_GRAY,
};

/* EOF */
