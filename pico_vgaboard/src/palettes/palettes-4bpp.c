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
#include "pico-vgaboard.h"
#include "palettes/grey.h"
#include "palettes/dawnbringer16.h"
#include "palettes/bubblegum16.h"

const BGAR5515 palette_4bpp_dawnbringer16[16] = {
    DAWNBRINGER16_00,
    DAWNBRINGER16_01,
    DAWNBRINGER16_02,
    DAWNBRINGER16_03,
    DAWNBRINGER16_04,
    DAWNBRINGER16_05,
    DAWNBRINGER16_06,
    DAWNBRINGER16_07,
    DAWNBRINGER16_08,
    DAWNBRINGER16_09,
    DAWNBRINGER16_10,
    DAWNBRINGER16_11,
    DAWNBRINGER16_12,
    DAWNBRINGER16_13,
    DAWNBRINGER16_14,
    DAWNBRINGER16_15,
};

const BGAR5515 palette_4bpp_bubblegum16[16] = {
    BUBBLEGUM16_00,
    BUBBLEGUM16_01,
    BUBBLEGUM16_02,
    BUBBLEGUM16_03,
    BUBBLEGUM16_04,
    BUBBLEGUM16_05,
    BUBBLEGUM16_06,
    BUBBLEGUM16_07,
    BUBBLEGUM16_08,
    BUBBLEGUM16_09,
    BUBBLEGUM16_10,
    BUBBLEGUM16_11,
    BUBBLEGUM16_12,
    BUBBLEGUM16_13,
    BUBBLEGUM16_14,
    BUBBLEGUM16_15,
};

// const BGAR5515 palette_4bpp_co16[16] = {
//     CO16_00,
//     CO16_01,
//     CO16_02,
//     CO16_03,
//     CO16_04,
//     CO16_05,
//     CO16_06,
//     CO16_07,
//     CO16_08,
//     CO16_09,
//     CO16_10,
//     CO16_11,
//     CO16_12,
//     CO16_13,
//     CO16_14,
//     CO16_15,
// };

/* There are only 32 shades of grey in RGB555... */
/*const BGAR5515 palette_8bpp_grey[256] = {
};*/
/* Shell command generating lines for copy/paste above:
for c in $(seq 255); do
    printf "    PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x%02x, 0x%02x, 0x%02x),\n" $c $c $c
done
*/

/* EOF */
