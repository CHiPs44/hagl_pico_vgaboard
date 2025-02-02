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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico.h"
#include "pico/time.h"

#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"

#include "colors.h"
#include "palettes/palettes.h"
#include "pico-vgaboard-console.h"

/** @brief Canonical 8x8 BIOS US font from IBM (CP437) */
pico_vgaboard_console_font pico_vgaboard_console_font_bios_f08 = {
    .bitmap = (uint8_t *)&pico_vgaboard_console_font_bios_f08_8_256,
    .size = sizeof(pico_vgaboard_console_font_bios_f08_8_256),
    .codepage = 437,
    .width = 8,
    .height = 8,
    .first = 0,
    .last = 255,
    .name = "[CP437] IBM BIOS 8x8"};

// /** @brief Fictional example for an ASCII only 6x8 font */
// pico_vgaboard_console_font ascii_5x8_font = {
//     .bitmap = NULL, // ascii_6x8,
//     .size = 0,      // ascii_6x8_len,
//     .codepage = 0,
//     .width = 6,
//     .height = 8,
//     .first = 32,
//     .last = 126,
//     .name = "[ASCII] 6x8 (TODO)"};

void pico_vgaboard_console_init(
    pico_vgaboard_console *console,
    int plane,
    uint16_t screen_width, uint16_t screen_height,
    uint8_t margin_top, uint8_t margin_bottom,
    uint8_t margin_left, uint8_t margin_right,
    const uint16_t *palette,
    uint8_t cols, uint8_t rows, pico_vgaboard_console_cell *buffer)
{
    console->screen_width = screen_width;
    console->screen_height = screen_height;
    console->margins[POS_TOP] = margin_top;
    console->margins[POS_BOTTOM] = margin_bottom;
    console->margins[POS_LEFT] = margin_left;
    console->margins[POS_RIGHT] = margin_right;
    console->cols = cols;
    console->rows = rows;
    console->buffer = buffer;
    // default font at 0 and clear others
    console->fonts[0] = &pico_vgaboard_console_font_bios_f08;
    for (uint8_t i = 1; i < PICO_VGABOARD_CONSOLE_FONT_COUNT; i += 1)
        console->fonts[i] = NULL;
    // default attributes
    console->cell.c = '\0';
    console->cell.blink = false;
    console->cell.reverse = false;
    console->cell.transparent = false;
    console->cell.underline = false;
    console->cell.font = 0;
    // palette & colors
    pico_vgaboard_console_set_palette(console, palette);
    pico_vgaboard_console_set_background(console, 0);
    pico_vgaboard_console_set_foreground(console, 15);
    // reset cursor position & hide it
    console->cursor_col = 0;
    console->cursor_row = 0;
    console->cursor_shape = PICO_VGABOARD_CONSOLE_CURSOR_OFF;
    console->cursor_anim = PICO_VGABOARD_CONSOLE_CURSOR_STILL;
    // clear console
    pico_vgaboard_console_clear(console);
    // setup timers
    pico_vgaboard_console_timers_init(console);
    // setup plane
    pico_vgaboard_init_plane(
        plane, PICO_VGABOARD_PLANE_CONSOLE, 0, console,
        pico_vgaboard_console_init_plane, pico_vgaboard_console_render_scanline);
}

void pico_vgaboard_console_timers_init(pico_vgaboard_console *console)
{
#if !PICO_NO_HARDWARE
    console->timer_fast = make_timeout_time_ms(PICO_VGABOARD_CONSOLE_BLINK_FAST_MS);
    console->timer_slow = make_timeout_time_ms(PICO_VGABOARD_CONSOLE_BLINK_SLOW_MS);
#endif
    console->state_fast = false;
    console->state_slow = false;
}

void pico_vgaboard_console_timers_refresh(pico_vgaboard_console *console)
{
#if !PICO_NO_HARDWARE
    absolute_time_t absolute_time = get_absolute_time();
    if (absolute_time_diff_us(absolute_time, console->timer_fast) < 0)
    {
        console->timer_fast = make_timeout_time_ms(PICO_VGABOARD_CONSOLE_BLINK_FAST_MS);
        console->state_fast = !console->state_fast;
    }
    if (absolute_time_diff_us(absolute_time, console->timer_slow) < 0)
    {
        console->timer_slow = make_timeout_time_ms(PICO_VGABOARD_CONSOLE_BLINK_SLOW_MS);
        console->state_slow = !console->state_slow;
    }
#endif
}

void pico_vgaboard_console_clear(pico_vgaboard_console *console)
{
    pico_vgaboard_console_cell cell = {
        .c = '\0',
        .transparent = true,
        .background = 0,
        .foreground = 15};
    for (uint8_t row = 0; row <= console->rows; row += 1)
    {
        for (uint8_t col = 0; col <= console->cols; col += 1)
        {
            memcpy(&console->buffer[console->rows * row + col], &cell, sizeof(pico_vgaboard_console_cell));
        }
    }
}

void pico_vgaboard_console_set_palette(pico_vgaboard_console *console, const uint16_t *palette)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        console->palette[i] = palette[i];
    }
}

void pico_vgaboard_console_set_background(pico_vgaboard_console *console, uint8_t background)
{
    console->cell.background = background & 0xf;
}

void pico_vgaboard_console_set_foreground(pico_vgaboard_console *console, uint8_t foreground)
{
    console->cell.foreground = foreground & 0xf;
};

void pico_vgaboard_console_set_blink(pico_vgaboard_console *console, bool blink)
{
    console->cell.blink = blink;
}

void pico_vgaboard_console_set_reverse(pico_vgaboard_console *console, bool reverse)
{
    console->cell.reverse = reverse;
}

void pico_vgaboard_console_set_transparent(pico_vgaboard_console *console, bool transparent)
{
    console->cell.transparent = transparent;
}

void pico_vgaboard_console_set_underline(pico_vgaboard_console *console, bool underline)
{
    console->cell.underline = underline;
}

void pico_vgaboard_console_set_cursor(pico_vgaboard_console *console, pico_vgaboard_console_cursor_shape shape, pico_vgaboard_console_cursor_animation anim)
{
    console->cursor_shape = shape;
    console->cursor_anim = anim;
}

bool pico_vgaboard_console_show_cursor(pico_vgaboard_console *console)
{
    bool old = console->cursor_on;
    console->cursor_on = true;
    return old;
}

bool pico_vgaboard_console_hide_cursor(pico_vgaboard_console *console)
{
    bool old = console->cursor_on;
    console->cursor_on = false;
    return old;
}

void pico_vgaboard_console_scroll_up(pico_vgaboard_console *console)
{
    // copy lines 1... to lines 0... in one memcpy
    uint16_t row_size = console->cols * sizeof(pico_vgaboard_console_cell);
    pico_vgaboard_console_cell *dst = (pico_vgaboard_console_cell *)(console->buffer);
    pico_vgaboard_console_cell *src = dst + console->cols;
    memcpy(dst, src, row_size * (console->rows - 1));
    // fill last line with default cell
    pico_vgaboard_console_cell cell = {
        .c = '\0',
        .transparent = true,
        .background = 0,
        .foreground = 15};
    uint16_t offset = row_size * (console->rows - 1);
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(&console->buffer[offset], &cell, sizeof(pico_vgaboard_console_cell));
        offset += sizeof(pico_vgaboard_console_cell);
    }
}

void pico_vgaboard_console_scroll_down(pico_vgaboard_console *console)
{
    // copy line 0 to line 1, line 1 to line 2, and so on
    // NB: go from bottom to top as data would be overwritten
    uint16_t line_size = console->cols * sizeof(pico_vgaboard_console_cell);
    pico_vgaboard_console_cell *dst = &(console->buffer[(console->rows - 2) * console->cols]);
    pico_vgaboard_console_cell *src = dst - console->cols;
    for (uint8_t row = 1; row <= console->rows; row += 1)
    {
        memcpy(dst, src, line_size);
        dst -= line_size;
        src -= line_size;
    }
    // fill first line with default cell
    pico_vgaboard_console_cell cell = {.c = '\0', .transparent = true, .background = 0, .foreground = 15};
    uint16_t offset = 0;
    for (uint8_t col = 0; col < console->cols; col += 1)
    {
        memcpy(&console->buffer[offset], &cell, sizeof(pico_vgaboard_console_cell));
        offset += sizeof(pico_vgaboard_console_cell);
    }
}

void pico_vgaboard_console_put_char_at(pico_vgaboard_console *console, uint8_t row, uint8_t col, uint8_t ch)
{
    if (row >= console->rows || col >= console->cols)
        return;
    uint16_t offset = console->cols * row + col;
    console->buffer[offset].c = ch;
    console->buffer[offset].background = console->cell.background;
    console->buffer[offset].foreground = console->cell.foreground;
    console->buffer[offset].blink = console->cell.blink;
    console->buffer[offset].reverse = console->cell.reverse;
    console->buffer[offset].transparent = console->cell.transparent;
    console->buffer[offset].underline = console->cell.underline;
}

void pico_vgaboard_console_move_cursor_to(pico_vgaboard_console *console, uint8_t row, uint8_t col)
{
    console->cursor_col = col >= console->cols ? console->cols - 1 : col;
    console->cursor_row = row >= console->rows ? console->rows - 1 : row;
}

void pico_vgaboard_console_put_char(pico_vgaboard_console *console, uint8_t ch)
{
    uint16_t offset = console->cols * console->cursor_row + console->cursor_col;
    console->buffer[offset].c = ch;
    console->buffer[offset].background = console->cell.background;
    console->buffer[offset].foreground = console->cell.foreground;
    console->buffer[offset].blink = console->cell.blink;
    console->buffer[offset].reverse = console->cell.reverse;
    console->buffer[offset].transparent = console->cell.transparent;
    console->buffer[offset].underline = console->cell.underline;
    console->cursor_col += 1;
    if (console->cursor_col >= console->cols)
    {
        console->cursor_col = 0;
        console->cursor_row += 1;
        // for now, just wrap to top of console, ignore console->scroll setting
        // (perhaps should stay as is, as auto scrolling can be a pain in the ...)
        if (console->cursor_row >= console->rows)
        {
            console->cursor_row = 0;
        }
    }
}

void pico_vgaboard_console_put_string(pico_vgaboard_console *console, uint8_t *s)
{
    while (*s)
    {
        pico_vgaboard_console_put_char(console, *s++);
    }
}

void pico_vgaboard_console_init_plane(void *plane_state)
{
#if PICO_VGABOARD_DEBUG
    printf("*** PICO_VGABOARD_CONSOLE_INIT_PLANE ***\n");
#endif
}

uint16_t __not_in_flash("pico_vgaboard_code")(pico_vgaboard_console_render_scanline)(
    void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max)
{
    pico_vgaboard_console *console = plane_state;
    uint16_t data_used;
    uint32_t *scanline_colors = data;

    static uint32_t counter = 0;
    counter += 1;

    // are we in top or bottom margins?
    if ((console->margins[POS_TOP] > 0 || console->margins[POS_BOTTOM] > 0) &&
        ((scanline_number < console->margins[POS_TOP]) ||
         (scanline_number > console->screen_height - console->margins[POS_BOTTOM] - 1)))
    {
        // whole transparent line
        if (scanline_number % 2 == 0)
            scanline_colors[0] = COMPOSABLE_RAW_1P | (0 << 16);
        else
            scanline_colors[0] = COMPOSABLE_RAW_1P | (0xffff << 16);
        scanline_colors[1] = COMPOSABLE_EOL_SKIP_ALIGN;
        if (counter > 10000 && console->debug_message[0] == '\0')
        {
            snprintf(console->debug_message, PICO_VGABOARD_CONSOLE_DEBUG_MSG_MAX_LEN, "%05d [CONSOLE] Top/Bottom!", scanline_number);
            counter = 0;
        }
        return 2;
    }

    uint8_t screen_row = (scanline_number - console->margins[POS_TOP]) / console->fonts[0]->height;
    uint8_t char_row = (scanline_number - console->margins[POS_TOP]) % console->fonts[0]->height;
    // would it be better to have all this state in console itself instead of stack?
    pico_vgaboard_console_cell *cell;
    uint8_t *font_row;
    uint8_t pixels;
    bool bit;
    uint8_t mask, i;
    uint32_t p[2];
    BGAR5515 bg, fg;
    bool transparent, reverse, underline, blink, cursor_row, cursor_col, cursor_visible;

    // update timers on top line of chars
    if (char_row == 0)
        pico_vgaboard_console_timers_refresh(console);

    // is cursor at current text row?
    cursor_row = console->cursor_shape != PICO_VGABOARD_CONSOLE_CURSOR_OFF && (screen_row == console->cursor_row);
    // is cursor at current text cell and should it be visible?
    switch (console->cursor_anim)
    {
    case PICO_VGABOARD_CONSOLE_CURSOR_STILL:
        cursor_visible = cursor_row;
        break;
    case PICO_VGABOARD_CONSOLE_CURSOR_BLINK_FAST:
        cursor_visible = cursor_row && console->state_fast;
        break;
    case PICO_VGABOARD_CONSOLE_CURSOR_BLINK_SLOW:
        cursor_visible = cursor_row && console->state_slow;
        break;
    default:
        cursor_visible = false;
        break;
    }

    // offset of line of chars in font bitmap
    font_row = &console->fonts[0]->bitmap[256 * char_row];

    int debug_left = 0, debug_text = 0, debug_right = 0;

    // insert left margin?
    if (console->margins[POS_LEFT] > 0)
    {
        for (uint8_t i = 0; i < console->margins[POS_LEFT] / 2; ++i)
        {
            ++scanline_colors;
            *scanline_colors = PICO_SCANVIDEO_ALPHA_MASK << 16 | PICO_SCANVIDEO_ALPHA_MASK;
            debug_left++;
        }
    }

    for (uint8_t screen_col = 0; screen_col < console->cols; screen_col += 1)
    {
        // show cursor?
        cursor_col = cursor_visible && (screen_col == console->cursor_col);
        // retrieve cell
        cell = &console->buffer[screen_row * console->cols + screen_col];
        // attributes
        transparent = cell->transparent;
        reverse = cell->reverse;
        // underline means all pixels are on for last line
        underline = (cell->underline) && (char_row == 7);
        // use fast timer, slow is awful ;-)
        blink = (cell->blink) && console->state_fast;
        // colors
        bg = console->palette[cell->background];
        fg = console->palette[cell->foreground];
        pixels = font_row[cell->c];
        if (cursor_col)
        {
            switch (console->cursor_shape)
            {
            case PICO_VGABOARD_CONSOLE_CURSOR_BLOCK:
                pixels = ~pixels;
                break;
            case PICO_VGABOARD_CONSOLE_CURSOR_RIGHT:
                // LSB is rightmost pixel
                pixels |= 1;
                break;
            case PICO_VGABOARD_CONSOLE_CURSOR_BOTTOM:
                if (char_row == 7)
                    pixels = 0xff;
                break;
            }
        }
        else if (blink)
            pixels = ~pixels;
        // MSB is leftmost pixel
        mask = 0b10000000;
        i = 0;
        for (uint8_t j = 0; j < 8; j += 1)
        {
            bit = underline ? true : pixels & mask;
            // transparent pixel?
            if (transparent)
                // reverse? => swap foreground and background
                if (reverse)
                    p[i] = bit ? 0 : bg | PICO_SCANVIDEO_ALPHA_MASK;
                else
                    p[i] = bit ? fg | PICO_SCANVIDEO_ALPHA_MASK : 0;
            else
                // reverse? => swap foreground and background
                if (reverse)
                    p[i] = (bit ? bg : fg) | PICO_SCANVIDEO_ALPHA_MASK;
                else
                    p[i] = (bit ? fg : bg) | PICO_SCANVIDEO_ALPHA_MASK;
            // first or second pixel?
            if (i == 0)
            {
                // next pixel
                i = 1;
            }
            else
            {
                // put these 2 16 bits pixels into current scanline
                *++scanline_colors = (p[1] << 16) | p[0];
                i = 0;
                debug_text++;
            }
            mask >>= 1;
        }
    }

    // append right margin?
    if (console->margins[POS_RIGHT] > 0)
    {
        for (uint8_t i = 0; i < console->margins[POS_RIGHT] / 2; ++i)
        {
            ++scanline_colors;
            *scanline_colors = 0; // PICO_SCANVIDEO_ALPHA_MASK << 16 | PICO_SCANVIDEO_ALPHA_MASK;
            debug_right++;
        }
    }

    // scanline end
    *++scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (console->cols * 8 - 2);
    data_used = (console->margins[POS_LEFT] + console->cols * 8 + console->margins[POS_RIGHT] + 4) / 2; // 2 16 bits pixels in each 32 bits word
    if (data_used > data_max)
        panic("pico_vgaboard_console_render_scanline: data_used (%d) > data_max (%d)", data_used, data_max);

    if (counter > 10000 && console->debug_message[0] == '\0')
    {
        printf("!");
        snprintf(console->debug_message, 255,
                 "%05d [CONSOLE] data_used: %d/%d left=%d, text=%d, right=%d, total=%d",
                 scanline_number, data_used, data_max,
                 debug_left, debug_text, debug_right, debug_left + debug_text + debug_right);
        counter = 0;
    }

    return data_used;
}

void pico_vgaboard_console_dump_settings(pico_vgaboard_console *console)
{
    printf("*** CONSOLE@%p\n", console);
    printf("Row: %03d/%03d (%03d), Col: %03d/%03d (%03d)\n",
           console->cursor_row, console->rows, console->rows * 8,
           console->cursor_col, console->cols, console->cols * 8);
    printf("Screen: %04d/%04d\n", console->screen_width, console->screen_height);
    printf("Margins: top=%03d, bottom=%03d, left=%03d, right=%03d\n",
           console->margins[POS_TOP], console->margins[POS_BOTTOM], console->margins[POS_LEFT], console->margins[POS_RIGHT]);
    printf("Bg: %02d, Fg: %02d, Attributes: %s%s%s\n",
           console->cell.background,
           console->cell.foreground,
           console->cell.transparent ? "T" : "-",
           console->cell.reverse ? "R" : "-",
           console->cell.underline ? "U" : "-",
           console->cell.blink ? "B" : "-");
    pico_vgaboard_console_font *font = console->fonts[0];
    printf("Font #0: %s (%dx%d)\n", font->name, font->width, font->height);
    printf("Palette:\n");
    for (uint8_t color = 0; color < 16; color += 1)
    {
        if (color > 0 && color % 8 == 0)
            printf("\n");
        printf("%02d: %02x%02x%02x ", color,
               PICO_SCANVIDEO_R5_FROM_PIXEL(console->palette[color]) << 3,
               PICO_SCANVIDEO_G5_FROM_PIXEL(console->palette[color]) << 3,
               PICO_SCANVIDEO_B5_FROM_PIXEL(console->palette[color]) << 3);
    }
    printf("\n");
}

void pico_vgaboard_console_dump_buffer(pico_vgaboard_console *console)
{
    uint8_t ch;
    printf("Buffer: %p\n", console->buffer);
    for (uint8_t row = 0; row <= console->rows; row += 1)
    {
        printf("%03d: ", row);
        for (uint8_t col = 0; col <= console->cols; col += 1)
        {
            ch = console->buffer[console->rows * row + col].c;
            printf("%c", ch >= 32 && ch < 127 ? ch : '.');
        }
        printf("\n");
    }
    printf("\n");
}
