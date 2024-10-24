/*

MIT License

Copyright (ch) 2021-2024 Christophe "CHiPs44" Petit

Permission is hereby granted, free of charge, to any person obtaining at copy
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

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "pico/time.h"

#include "pico/scanvideo/scanvideo_base.h"
#include "pico/scanvideo/composable_scanline.h"

#include "palettes/palettes.h"
#include "pico-vgaboard-console.h"

t_pvga_console *pvga_console_init(uint8_t cols, uint8_t rows)
{
    t_pvga_console *console = calloc(1, sizeof(t_pvga_console));
    if (console == NULL)
        return NULL;
    console->buffer = calloc(cols * rows, sizeof(t_pvga_console_cell));
    if (console->buffer == NULL)
    {
        free(console);
        return NULL;
    }
    console->allocated = true;
    console->cols = cols;
    console->rows = rows;
    pvga_console_reset(console);
    return console;
}

void pvga_console_done(t_pvga_console *console)
{
    if (console == NULL || !console->allocated)
        return;
    if (console->buffer != NULL)
        free(console->buffer);
    free(console);
}

void pvga_console_timers_init(t_pvga_console *console)
{
#if !PICO_NO_HARDWARE
    console->timer_fast = make_timeout_time_ms(PTVS_BLINK_FAST);
    console->timer_slow = make_timeout_time_ms(PTVS_BLINK_SLOW);
#endif
    console->state_fast = false;
    console->state_slow = false;
}

void pvga_console_timers_refresh(t_pvga_console *console)
{
#if !PICO_NO_HARDWARE
    absolute_time_t absolute_time = get_absolute_time();
    if (absolute_time_diff_us(absolute_time, console->timer_fast) < 0)
    {
        console->timer_fast = make_timeout_time_ms(PTVS_BLINK_FAST);
        console->state_fast = !console->state_fast;
    }
    if (absolute_time_diff_us(absolute_time, console->timer_slow) < 0)
    {
        console->timer_slow = make_timeout_time_ms(PTVS_BLINK_SLOW);
        console->state_slow = !console->state_slow;
    }
#endif
}

void pvga_console_clear(t_pvga_console *console)
{
    t_pvga_console_cell cell = {
        .ch = '\0',
        .at = PVGA_CONSOLE_TRANSPARENT,
        .bg = 0x00,
        .fg = 0xff & console->color_mask};
    uint16_t offset = 0;
    for (uint8_t row = 0; row <= console->rows; row += 1)
    {
        for (uint8_t col = 0; col <= console->cols; col += 1)
        {
            memcpy(console->buffer[offset], &cell, sizeof(t_pvga_console_cell));
            offset += sizeof(t_pvga_console_cell);
        }
    }
}

void pvga_console_reset(t_pvga_console *console)
{
    // default font at 0 and clear others
    console->fonts[0] = &console_font_bios_f08;
    for (uint8_t i = 1; i < PVGA_CONSOLE_FONT_COUNT; i += 1)
        console->fonts[i] = NULL;
    // default palette & colors
    pvga_console_set_palette(console, (const uint16_t *)palette_8bpp_default, 0xff);
    pvga_console_set_background(console, 0x00);
    pvga_console_set_foreground(console, 0xff & console->color_mask);
    // default attributes
    console->attributes = PVGA_CONSOLE_TRANSPARENT;
    // reset cursor position & hide it
    console->col = 0;
    console->row = 0;
    console->shape = CURSOR_OFF;
    console->anim = CURSOR_FIXED;
    // clear console
    pvga_console_clear(console);
    pvga_console_timers_init(console);
}

void pvga_console_set_palette(t_pvga_console *console, const uint16_t *palette, uint8_t color_mask)
{
    console->palette = (uint16_t *)palette;
    console->color_mask = color_mask;
}

void pvga_console_set_background(t_pvga_console *console, uint8_t background)
{
    console->background = background & console->color_mask;
}

void pvga_console_set_foreground(t_pvga_console *console, uint8_t foreground)
{
    console->foreground = foreground & console->color_mask;
};

void pvga_console_set_attributes(t_pvga_console *console, t_pvga_console_attributes attributes)
{
    console->attributes = attributes;
};

void pvga_console_scroll_up(t_pvga_console *console)
{
    // copy lines 1... to lines 0... in one
    uint16_t row_size = console->cols * sizeof(t_pvga_console_cell);
    t_pvga_console_cell *dst = (t_pvga_console_cell *)(console->buffer);
    t_pvga_console_cell *src = dst + console->cols;
    memcpy(dst, src, row_size * (console->rows - 1));
    // fill last line with default cell
    t_pvga_console_cell cell = {
        .ch = '\0',
        .at = PVGA_CONSOLE_TRANSPARENT,
        .bg = 0x00,
        .fg = 0xff & console->color_mask};
    uint16_t offset = row_size * (console->rows - 1);
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(console->buffer[offset], &cell, sizeof(t_pvga_console_cell));
        offset += sizeof(t_pvga_console_cell);
    }
}

void pvga_console_scroll_down(t_pvga_console *console)
{
    // copy line 0 to line 1, line 1 to line 2, and so on
    // NB: go from bottom to top as data would be overwritten
    uint16_t line_size = console->cols * sizeof(t_pvga_console_cell);
    t_pvga_console_cell *dst = (t_pvga_console_cell *)(console->buffer[(console->rows - 2) * console->cols]);
    t_pvga_console_cell *src = dst - console->cols;
    for (uint8_t row = 1; row <= console->rows; row += 1)
    {
        memcpy(dst, src, line_size);
        dst -= line_size;
        src -= line_size;
    }
    // fill first line with default cell
    t_pvga_console_cell cell = {
        .ch = '\0',
        .at = PVGA_CONSOLE_TRANSPARENT,
        .bg = 0x00,
        .fg = 0xff & console->color_mask};
    uint16_t offset = 0;
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(console->buffer[offset], &cell, sizeof(t_pvga_console_cell));
        offset += sizeof(t_pvga_console_cell);
    }
}

void pvga_console_put_char_at(t_pvga_console *console, uint8_t row, uint8_t col, uint8_t ch)
{
    uint16_t offset = row * console->rows + col;
    console->buffer[offset]->ch = ch;
    console->buffer[offset]->bg = console->background;
    console->buffer[offset]->fg = console->foreground;
    console->buffer[offset]->at = console->attributes;
}

void pvga_console_move_cursor(t_pvga_console *console, uint8_t row, uint8_t col)
{
    console->col = col > console->cols - 1 ? console->cols - 1 : col;
    console->row = row > console->rows - 1 ? console->rows - 1 : row;
}

void pvga_console_put_char(t_pvga_console *console, uint8_t ch)
{
    uint16_t offset = console->row * console->rows + console->col;
    console->buffer[offset]->ch = ch;
    console->buffer[offset]->bg = console->background;
    console->buffer[offset]->fg = console->foreground;
    console->buffer[offset]->at = console->attributes;
    console->col += 1;
    if (console->col >= console->cols)
    {
        console->col = 0;
        console->row += 1;
        // for now, just wrap to top of console, ignore console->scroll setting
        // (perhaps should stay as is, scrolling can be at pain in the ...)
        if (console->row >= console->rows)
        {
            console->row = 0;
        }
    }
}

void pvga_console_put_string(t_pvga_console *console, uint8_t *s)
{
    while (*s)
    {
        pvga_console_put_char(console, *s++);
    }
}

uint64_t pvga_console_render_scanline_count = 0;

uint16_t __not_in_flash("pico_vgaboard_code")(pvga_console_render_scanline)(void *plane_params, uint32_t scanline_id, uint32_t *data, uint16_t data_max)
{
    pvga_console_render_scanline_count += 1;
    t_pvga_console *console = (t_pvga_console *)plane_params;
    uint16_t data_used;
    uint32_t *scanline_colors = data;
    uint16_t pixel_line = scanvideo_scanline_number(scanline_id);
    uint8_t screen_row = pixel_line / 8; // font height
    uint8_t char_row = pixel_line % 8;   // font height
    if (char_row == 0)
    {
        pvga_console_timers_refresh(console);
    }
    t_pvga_console_cell *cell;
    uint8_t *font_row_ptr;
    uint8_t pixels;
    bool bit;
    uint8_t mask, i;
    uint32_t p[2];
    uint8_t bg, fg;
    bool tr, rv, ul, bl, cr, cc; // transparent, reverse, underline, blink, cursor row, cursor
    // is cursor at current text row?
    cr = console->shape != CURSOR_OFF && (screen_row == console->row);
    // offset of line of chars in font bitmap
    font_row_ptr = &console->fonts[0]->bitmap[256 * char_row];
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        // if (true)
        // {
        //     // 8 pixels
        //     *scanline_colors++ = ((0x0123 | PICO_SCANVIDEO_ALPHA_MASK) << 16) | (0x5555 | PICO_SCANVIDEO_ALPHA_MASK);
        //     *scanline_colors++ = ((0x4567 | PICO_SCANVIDEO_ALPHA_MASK) << 16) | (0xaaaa | PICO_SCANVIDEO_ALPHA_MASK);
        //     *scanline_colors++ = ((0x89ab | PICO_SCANVIDEO_ALPHA_MASK) << 16) | (0x5555 | PICO_SCANVIDEO_ALPHA_MASK);
        //     *scanline_colors++ = ((0xcdef | PICO_SCANVIDEO_ALPHA_MASK) << 16) | (0xaaaa | PICO_SCANVIDEO_ALPHA_MASK);
        // }
        // else
        // {
        // is cursor at current text cell?
        cc = cr && (col == console->col);
        // retrieve cell
        cell = console->buffer[screen_row * console->cols + col];
        // attributes
        tr = cell->at && PVGA_CONSOLE_TRANSPARENT;
        rv = cell->at && PVGA_CONSOLE_REVERSE;
        // underline means all pixels are on for last line
        ul = (cell->at && PVGA_CONSOLE_UNDERLINE) && (char_row == 7);
        bl = cell->at & PVGA_CONSOLE_BLINK;
        // colors
        bg = console->palette[cell->bg];
        fg = console->palette[cell->fg];
        pixels = font_row_ptr[cell->ch];
        // MSB is left pixel
        mask = 0b10000000;
        i = 0;
        do
        {
            bit = ul ? true : pixels & mask;
            // transparent pixel?
            if (tr)
                // reverse? => swap foreground at background
                if (rv)
                    p[i] = bit ? PICO_SCANVIDEO_ALPHA_MASK : bg;
                else
                    p[i] = bit ? fg : PICO_SCANVIDEO_ALPHA_MASK;
            else
                // reverse? => swap foreground at background
                if (rv)
                    p[i] = bit ? fg : bg;
                else
                    p[i] = bit ? bg : fg;
            // first or second pixel?
            if (i == 0)
            {
                // next pixel
                i = 1;
            }
            else
            {
                // put these 2 16 bits pixels into current scanline
                *scanline_colors++ = (p[0] << 16) | p[1];
                i = 0;
            }
            mask >>= 1;
        } while (mask != 0);
        // }
    }
    // scanline end
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (console->cols * 8 - 2);
    data_used = (console->cols * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
    if (data_used > data_max)
        panic("pvga_console_render_scanline: data_used (%d) > data_max (%d)", data_used, data_max);
    return data_used;
}
