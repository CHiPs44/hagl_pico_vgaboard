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
 - https://github.com/CHiPs44/hagl_pico_vgaboard
 - https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <stdio.h>

#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanrow.h"
#include "pico/scanvideo/scanvideo_base.h"

#include "pico-vgaboard-textmode->h"

pico_vgaboard_textmode_t pico_vgaboard_textmode;

/**
 * @brief Initialize a text mode structure
 */
void pico_vgaboard_textmode_init(pico_vgaboard_textmode_t *textmode)
{
    textmode->fonts = NULL;
    textmode->screen = NULL;
    textmode->font_count = 0;
    textmode->cols = 0;
    textmode->rows = 0;
    textmode->cursor = {
        .shape = TEXTCURSOR_TYPE_BLOCK,
        .animation = TEXTCURSOR_ANIM_OFF,
        .col = 0,
        .row = 0};
}

// #include "unscii_437_8x8.h"
// #include "unscii_850_8x8.h"
// #include "unscii_16_437_8x8.h"
// #include "unscii_16_850_8x8.h"

// /* clang-format off */
// pico_vgaboard_textmode_font_t *fonts = {
//     // 1234567890123456789012345678901
//     { .name = "Unscii Standard CP437 8x8" , .data = &unscii_437_8x8   , .size = unscii_437_8x8_len   , .codepage = 437, .height =  8, .first = 0, .last = 255 },
//     { .name = "Unscii Standard CP437 8x16", .data = &unscii_16_437_8x8, .size = unscii_16_437_8x8_len, .codepage = 437, .height = 16, .first = 0, .last = 255 },
//     { .name = "Unscii Standard CP850 8x8" , .data = &unscii_850_8x8   , .size = unscii_850_8x8_len   , .codepage = 850, .height =  8, .first = 0, .last = 255 },
//     { .name = "Unscii Standard CP850 8x16", .data = &unscii_16_850_8x8, .size = unscii_16_850_8x8_len, .codepage = 850, .height = 16, .first = 0, .last = 255 },
// };
// /* clang-format on */

void pico_vgaboard_textmode_render_scanline(
    pico_vgaboard_textmode_t *textmode,
    struct scanvideo_scanrow_buffer *buffer,
    uint16_t display_row)
{
    uint32_t *scanline_colors = buffer->data;
    uint8_t text_row = display_row / textmode->height;
    uint8_t char_row = display_row % textmode->height;
    bool at_cursor;
    uint8_t pixels, mask, i;
    uint16_t color1, color2;
    pico_vgaboard_textcell_t cell;
    for (uint8_t text_col = 0; text_col < textmode->columns; text_col += 1)
    {
        at_cursor = textmode->cursor.col == text_col && textmode->cursor.row == text_col;
        cell = textmode->screen[text_row * 8 + text_col];
        pixels = textmode->fonts[cell.i]->data[cell.c * 8 + char_row];
        mask = 128;
        // 8 pixels, 2 by 2 to fill 2 * 16 bits = 1 32 bit location of scanline buffer
        for (i = 0; i < 8; i += 2)
        {
            // first pixel
            if (pixels[i] & mask) {
                color1 = textmode->palette[cell.f] << 16;
            } else {
                color1 = textmode->palette[cell.b] << 16;
            }
            mask /= 2;
            // second pixel
            if (pixels[i+1] & mask) {
                color2 = textmode->palette[cell.f];
            } else {
                color2 = textmode->palette[cell.b];
            }
            mask /= 2;
            // append to scanline buffer
            ++scanline_colors;
            *scanline_colors = color1 | color2;
        }
    }
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = buffer->data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (textmode->columns * 8 - 2);
    buffer->data_used = (textmode->columns * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
}

// EOF
