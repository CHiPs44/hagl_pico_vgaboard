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
#include <string.h>

#include "hardware/structs/timer.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanrow.h"
#include "pico/scanvideo/scanvideo_base.h"

#include "pico-vgaboard-textmode.h"

// pico_vgaboard_term_t _pico_vgaboard_textmode;
// pico_vgaboard_term_t *pico_vgaboard_textmode = &_pico_vgaboard_textmode;

/**
 * @brief Initialize a text mode structure
 */
void pico_vgaboard_term_init(pico_vgaboard_term_mode_t *textmode, uint8_t cols, uint8_t rows)
{
    textmode->font = NULL;
    textmode->buffer = NULL;
    textmode->cols = 0;
    textmode->rows = 0;
    textmode->cursor = {
        .anim = TEXTCURSOR_ANIM_OFF,
        .col = 0,
        .row = 0,
        .state = false,
        .timer = 0ll};
}

void pico_vgaboard_term_set_background(pico_vgaboard_term_mode_t *textmode, uint8_t background)
{
    textmode->background = background;
}

void pico_vgaboard_term_set_foreground(pico_vgaboard_term_mode_t *textmode, uint8_t foreground)
{
    textmode->foreground = foreground;
}

void pico_vgaboard_term_set_alpha(pico_vgaboard_term_mode_t *textmode, bool alpha)
{
    textmode->alpha = alpha;
}

bool pico_vgaboard_term_forward(pico_vgaboard_term_mode_t *textmode)
{
    textmode->cursor.col += 1;
    if (textmode->cursor.col >= textmode->cols)
    {
        textmode->cursor.col = 0;
        textmode->cursor.row += 1;
        if (textmode->cursor.row >= textmode->rows)
        {
            if (textmode->scroll)
            {
                // stay on last line of screen
                textmode->cursor.row = textmode->rows - 1;
                // copy  lines 1..rows-1 to 0..rows
                pico_vgaboard_term_cell_t *src = &textmode->screen[1 * textmode->cols];
                pico_vgaboard_term_cell_t *dst = &textmode->screen[0 * textmode->cols];
                size_t len = (textmode->rows - 1) * sizeof(pico_vgaboard_term_cell_t);
                memmove(dst, src, len);
                // fill new line with spaces
                pico_vgaboard_term_cell_t cell;
                for (uint8_t text_col = 0; text_col < textmode->cols; text_col += 1)
                {
                    cell = textmode->screen[textmode->cols * (textmode->rows - 1) + text_col];
                    cell.ch = ' ';
                    cell.bg = textmode->bg;
                    cell.fg = textmode->fg;
                }
            }
            else
            {
                // wrap to home (0, 0)
                textmode->cursor.row = 0;
            }
        }
    }
    return true;
}

bool pico_vgaboard_term_write_byte(pico_vgaboard_term_mode_t *textmode, uint8_t ch)
{
    int index = textmode->cursor.row * textmode->cols + textmode->cursor.col;
    if (index < 0 || index >= textmode->rows * textmode->cols)
        return false;
    pico_vgaboard_term_cell_t cell = textmode->screen[index];
    cell.ch = ch;
    cell.bg = textmode->bg;
    cell.fg = textmode->fg;
    pico_vgaboard_term_forward(textmode) return true;
}

void pico_vgaboard_term_render_scanline(
    pico_vgaboard_term_mode_t *textmode,
    struct scanvideo_scanrow_buffer *buffer,
    uint16_t display_row)
{
    uint32_t *scanline_colors = buffer->data;
    uint8_t text_row = display_row / textmode->font->height;
    uint8_t char_row = display_row % textmode->font->height;
    bool at_cursor;
    uint8_t pixels, mask, i;
    uint8_t bg, fg;
    uint32_t pixel1, pixel2;
    pico_vgaboard_textcell_t cell;
    for (uint8_t text_col = 0; text_col < textmode->cols; text_col += 1)
    {
        cell = textmode->screen[text_row * textmode->cols + text_col];
        at_cursor = textmode->cursor.col == text_col && textmode->cursor.row == text_col;
        bg = at_cursor ? textmode->palette[cell.fg] : textmode->palette[cell.bg];
        fg = at_cursor ? textmode->palette[cell.bg] : textmode->palette[cell.fg];
        if (cell.ch >= textmode->font->first && cell.ch <= textmode->font->last)
            pixels = textmode->font->data[cell.ch * 8 + char_row];
        else
            pixels = 0;
        mask = 128;
        // 8 pixels, 2 by 2 to fill 2 * 16 bits = 1 32 bit location of scanline buffer
        for (i = 0; i < 8; i += 2)
        {
            // first pixel
            pixel1 = pixels[i + 0] & mask ? fg : bg;
            mask >>= 1;
            // second pixel
            pixel2 = pixels[i + 1] & mask ? fg << 16 : bg;
            mask >>= 1;
            // at cursor position?
            if (at_cursor)
            {
                switch (textmode->cursor.anim)
                {
                case TEXTCURSOR_ANIM_OFF:
                    // Nothing to do
                    break;
                case TEXTCURSOR_ANIM_FIXED:

                    break;
                case TEXTCURSOR_ANIM_FLASH_SLOW:
                    break;
                case TEXTCURSOR_ANIM_FLASH_FAST:
                    break;
                default:
                    break;
                }
            }
            // append to scanline buffer
            ++scanline_colors;
            *scanline_colors = pixel1 | pixel2;
        }
    }
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = buffer->data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (textmode->cols * 8 - 2);
    buffer->data_used = (textmode->cols * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
}

// EOF
