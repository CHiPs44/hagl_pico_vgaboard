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

#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <types.h>

#include "pico/time.h"

struct s_pvga_console_screen
{
    t_pvga_console_cell **buffer;
    t_pvga_console_font *fonts[PVGA_CONSOLE_FONT_COUNT];
    uint16_t *palette;
    uint8_t rows;
    uint8_t cols;
    uint8_t color_mask; // 0x0f for 16 colors, 0xff for 256 colors
    uint8_t background;
    uint8_t foreground;
    uint8_t attributes;
    bool auto_scroll; // automatic scrolling at end of console
    // cursor
    uint8_t row;
    uint8_t col;
    uint16_t anim;
    // states & associated timers
    bool state_fast;
    bool state_slow;
    absolute_time_t timer_fast;
    absolute_time_t timer_slow;
};

void pvga_console_timers_init(t_pvga_console *console)
{
    console->timer_fast = make_timeout_time_ms(PTVS_BLINK_FAST);
    console->state_fast = false;
    console->timer_slow = make_timeout_time_ms(PTVS_BLINK_SLOW);
    console->state_slow = false;
}

void pvga_console_timers_refresh(t_pvga_console *console)
{
    absolute_time_t absolute_time = get_absolute_time();
    if (absolute_time_diff_us(absolute_time, console->timer_fast) < 0)
    {
        console->timer_fast = make_timeout_time_ms(PVTS_SPEED_FAST);
        console->state_fast = !console->state_fast;
    }
    if (absolute_time_diff_us(absolute_time, console->timer_slow) < 0)
    {
        console->timer_slow = make_timeout_time_ms(PVTS_SPEED_SLOW);
        console->state_slow = !console->state_slow;
    }
}

void pvga_console_clear(t_pvga_console *console)
{
    pvga_console_cell cell = {
        .c = '\0',
        .a = PVTS_TRANSPARENT,
        .b = 0x00,
        .f = 0xff & console->color_mask};
    uint16_t offset = 0;
    for (uint8_t row = 0; row <= console->rows; row += 1)
    {
        for (uint8_t col = 0; col <= console->cols; col += 1)
        {
            memcpy(console->buffer[offset], &cell, sizeof(pvga_console_cell));
            offset += sizeof(pvga_console_cell);
        }
    }
}

void pvga_console_reset(t_pvga_console *console, uint8_t cols, uint8_t rows)
{
    // default font at 0 and clear others
    console->fonts[0] = &bios_f08_font;
    for (uint_t i = 1; i < PVTS_FONT_COUNT; i += 1)
        console->fonts[i] = NULL;
    // default palette & colors
    pvga_console_set_palette(console, palette_8bpp_default, 0xff);
    pvga_console_set_background(console, 0x00);
    pvga_console_set_foreground(console, 0xff);
    // default attributes
    console->attributes = PVTS_TRANSPARENT;
    // reset cursor position & hide it
    console->col = 0;
    console->row = 0;
    console->anim = CURSOR_HIDDEN;
    // clear console
    pvga_console_clear(console);
}

t_pvga_console *pvga_console_init(uint8_t cols, uint8_t rows)
{
    t_pvga_console *console = calloc(1, sizeof(t_pvga_console));
    if (console == NULL)
        return NULL;
    console->cols = cols;
    console->rows = rows;
    console->buffer = calloc(cols * rows, sizeof(pvga_console_cell));
    if (console->buffer == NULL)
    {
        free(console);
        return NULL;
    }
    pvga_console_reset(console);
    pvga_console_timers_init(console);
    return console;
}

void pvga_console_done(t_pvga_console *console)
{
    if (console == NULL)
        return;
    if (console->buffer != NULL)
        free(console->buffer);
    free(console);
}

void pvga_console_set_palette(t_pvga_console *console, uint16_t *palette, uint8_t color_mask)
{
    console->palette = palette;
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

void pvga_console_set_attributes(t_pvga_console *console, pvga_console_attributes attributes)
{
    console->attributes = attributes;
};

void pvga_console_scroll_up(t_pvga_console *console)
{
    // copy lines 1... to lines 0... in one
    uint16_t line_size = console->cols * sizeof(pvga_console_cell);
    uint16_t dst = console->buffer;
    uint16_t src = dst + line_size;
    memcpy(dst, src, line_size * (console->rows - 1));
    // fill last line with default cell
    pvga_console_cell cell = {
        .c = '\0',
        .a = PVTS_TRANSPARENT,
        .b = 0x00,
        .f = 0xff & console->color_mask};
    uint16_t offset = line_size * (console->rows - 1);
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(console->buffer[offset], &cell, sizeof(pvga_console_cell));
        offset += sizeof(pvga_console_cell);
    }
}

void pvga_console_scroll_down(t_pvga_console *console)
{
    // copy line 0 to line 1, line 1 to line 2, and so on
    // NB: go from bottom to top as data would be overwritten
    uint16_t line_size = console->cols * sizeof(pvga_console_cell);
    uint16_t dst = console->buffer + line_size * (console->rows - 2);
    uint16_t src = dst - line_size;
    for (uint8_t row = 1; row <= console->rows; row += 1)
    {
        memcpy(dst, src, line_size);
        dst -= line_size;
        src -= line_size;
    }
    // fill first line with default cell
    pvga_console_cell cell = {
        .c = '\0',
        .a = PVTS_TRANSPARENT,
        .b = 0x00,
        .f = 0xff & console->color_mask};
    uint16_t offset = 0;
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(console->buffer[offset], &cell, sizeof(pvga_console_cell));
        offset += sizeof(pvga_console_cell);
    }
}

void pvga_console_put_char_at(t_pvga_console *console, uint8_t row, uint8_t col, uint8_t c)
{
    uint16_t offset = row * console->rows + col;
    console->buffer[offset]->c = c;
    console->buffer[offset]->b = console->background;
    console->buffer[offset]->f = console->foreground;
    console->buffer[offset]->a = console->attributes;
}

void pvga_console_move_cursor(t_pvga_console *console, uint8_t row, uint8_t col)
{
    console->col = col > console->cols - 1 ? console->cols - 1 : col;
    console->row = row > console->rows - 1 ? console->rows - 1 : row;
}

void pvga_console_put_char(t_pvga_console *console, uint8_t c)
{
    uint16_t offset = console->row * console->rows + console->col;
    console->buffer[offset]->c = c;
    console->buffer[offset]->b = console->background;
    console->buffer[offset]->f = console->foreground;
    console->buffer[offset]->a = console->attributes;
    console->col += 1;
    if (console->col >= console->cols)
    {
        console->col = 0;
        console->row += 1;
        // for now, just wrap to top of console, ignore console->scroll setting
        // (perhaps should stay as is, scrolling can be a pain in the ...)
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

void pvga_console_render_scanline(t_pvga_console *console, scanvideo_scanline_buffer_t *buffer)
{
    uint32_t *scanline_colors = buffer->data;
    uint16_t line = scanvideo_scanline_number(buffer->scanline_id);
    uint8_t screen_row = line / 8; // font height
    uint8_t char_line = line % 8;  // font height
    if (char_line == 0)
    {
        pvga_console_timers_refresh(console);
    }
    pvga_console_cell cell;
    uint8_t *font_line_ptr;
    uint8_t pixels;
    bool bit;
    uint8_t mask, i;
    uint32_t p[2];
    uint8_t b, f;
    bool t, r, u, bl, cr, c; // transparent, reverse, underline, blink, cursor row, cursor
    // cursor at current text line?
    cr = console->anim != CURSOR_HIDDEN && screen_row == console->row;
    // offset of line of chars in font bitmap
    font_line_ptr = console->fonts[0]->bitmap[256 * char_line];
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        // cursor at current text col?
        c = cr && col == console->col;
        // retrieve cell
        cell = console->buffer[screen_row * console->cols + col];
        // attributes
        t = cell.a && PVTS_TRANSPARENT;
        r = cell.a && PVTS_REVERSE;
        // underline means all pixels are on for last line
        u = (cell.a && PVTS_UNDERLINE) && (char_line == 7);
        bl = cell.a & PVTS_BLINK;
        // colors
        b = console->palette[cell.b];
        f = console->palette[cell.f];
        pixels = font_line_ptr[cell.c];
        // MSB is left pixel
        mask = 0b10000000;
        i = 0;
        do
        {
            bit = u ? true : pixels & mask;
            // transparent pixel?
            if (t)
                // reverse? => swap foreground a background
                if (r)
                    p[i] = bit ? PICO_SCANVIDEO_ALPHA_MASK : b;
                else
                    p[i] = bit ? f : PICO_SCANVIDEO_ALPHA_MASK;
            else
                // reverse? => swap foreground a background
                if (r)
                    p[i] = bit ? f : b;
                else
                    p[i] = bit ? b : f;
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
        } while (mask != 0)
    }
    // scanline end
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = buffer->data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (console->cols * 8 - 2);
    buffer->data_used = (console->cols * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
}

#ifdef __cplusplus
}
#endif

#endif /* _PVTS_H */
