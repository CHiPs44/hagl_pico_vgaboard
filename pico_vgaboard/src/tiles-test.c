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
 - https://github.com/CHiPs44/hagl_pico_vgaboard
 - https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <string.h>
#include <wchar.h>

#include "../include/tiles.h"
#include "../include/palettes/palettes.h"

tileset_t tileset_test = {
    .tile_w = 8,
    .tile_h = 8,
    .log_depth = LOG_DEPTH_2_COLORS,
    .palette = palette_1bpp_amber,
    .cols = 1,
    .rows = 16,
    .data = {
        /* clang-format off */
        // Tile #0: ┏
        0b00000000,
        0b00000000,
        0b00000000,
        0b00011111,
        0b00011111,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #1: ┳
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111111,
        0b11111111,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #2: ┓
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111000,
        0b11111000,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #3: ┗
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011111,
        0b00011111,
        0b00000000,
        0b00000000,
        0b00000000,
        // Tile #4: ┻
        0b00011000,
        0b00011000,
        0b00011000,
        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,
        0b00000000,
        // Tile #5: ┛
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111000,
        0b11111000,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #6: ┣
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011111,
        0b00011111,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #7: ╋
        0b00011000,
        0b00011000,
        0b00011000,
        0b11111111,
        0b11111111,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #8: ┫
        0b00011000,
        0b00011000,
        0b00011000,
        0b11111000,
        0b11111000,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #9: ━
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111111,
        0b11111111,
        0b00000000,
        0b00000000,
        0b00000000,
        // Tile #10: ┃
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        // Tile #11: ▒
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        // Tile #12: ░
        0b11001100,
        0b00110011,
        0b11001100,
        0b00110011,
        0b11001100,
        0b00110011,
        0b11001100,
        0b00110011,
        // Tile #13: ▓
        0b11111111,
        0b10101010,
        0b11111111,
        0b10101010,
        0b11111111,
        0b10101010,
        0b11111111,
        0b10101010,
        // Tile #14: █
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        // Tile #15: empty
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        /* clang-format on */
    },
};

//                                       0123456789012345
wchar_t tilemap_test_wchar_to_tile[] = L"┏┳┓┗┻┛┣╋┫━┃▒░▓█ ";

wchar_t *tilemap_test_wtext[16] = {
    /* clang-format off */
//    0123456789012345
    L"┏━━━━┳━━━━━┳━━━┓",
    L"┃▒▒▒▒┃▒▒▒▒▒┃▒▒▒┃",
    L"┃▒▒▒▒░▒▒▒▒▒░▒▒▒┃",
    L"┃▒▒▒▒┣━━━━━┫▒▒▒┃",
    L"┣━░━━┫▒▒▒▒▒┃▒▒▒┃",
    L"┃▒▒▒▒░▒▒▒▒▒┣━░━┫",
    L"┣━━░━┻━┳━░━┛▒▒▒┃",
    L"┃▒▒▒▒▒▒┃▒▒▒▒▒▒▒┃",
    L"┃▒▒▒▒▒▒┣━━┳━░━━┫",
    L"┃▒▒▒▒▒▒░▒▒░▒▒▒▒┃",
    L"┃▒▒▒▒▒▒┃▒▒┃▒▒▒▒┃",
    L"┣━━░━━━╋━━╋━━░━┫",
    L"┃▒▒▒▒▒▒┃▒▒┃▒▒▒▒┃",
    L"┃▒▒▒▒▒▒░▒▒▒▒▒▒▒┃",
    L"┣━━░━━━╋━━░━━━━┫",
    L"┃▒▒▒▒▒▒┃▒▒▒▒▒▒▒┃",
    L"┗━━━━━━┻━━━━━━━┛",
    /* clang-format on */
};

int8_t tilemap_test_data[16][16];

tilemap_t tilemap_test = {
    /* clang-format off */
    .tileset  = &tileset_test,
    .cols     = 16,
    .rows     = 16,
    .data     = &tilemap_test_data,
    .screen_x = 16,
    .screen_y = 16,
    .screen_w = 8 * 8,
    .screen_h = 8 * 8,
    /* clang-format on */
};

bool tilemap_test_run()
{
    if (!tileset_init(&tileset_test))
        return false;
    if (!tilemap_init(&tilemap_test))
        return false;
    tilemap_convert_text_to_data(tilemap_test.cols, tilemap_test.rows, tilemap_test_wtext, tilemap_test_data);
}
