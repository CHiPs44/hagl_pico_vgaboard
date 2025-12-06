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
#include "palettes/sweetie16.h"

const BGAR5515 palette_4bpp_sweetie16[16] = {
    SWEETIE16_00_BLACK,
    SWEETIE16_01_VIOLET,
    SWEETIE16_02_RED,
    SWEETIE16_03_ORANGE,
    SWEETIE16_04_YELLOW,
    SWEETIE16_05_LIGHT_GREEN,
    SWEETIE16_06_GREEN,
    SWEETIE16_07_GREEN_BLUE,
    SWEETIE16_08_DARK_BLUE,
    SWEETIE16_09_BLUE,
    SWEETIE16_10_LIGHT_BLUE,
    SWEETIE16_11_LIGHT_CYAN,
    SWEETIE16_12_WHITE,
    SWEETIE16_13_LIGHT_GREY,
    SWEETIE16_14_GREY,
    SWEETIE16_15_BLUE_GREY,
};

/* EOF */
