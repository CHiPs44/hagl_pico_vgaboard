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

#include "fonts/bios-f08.h"
#include "pico-vgaboard.h"

#ifndef _CONSOLE_H
#define _CONSOLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CONSOLE_MAX_COLS
#define CONSOLE_MAX_COLS 80u
#endif

#ifndef CONSOLE_MAX_ROWS
#define CONSOLE_MAX_ROWS 60u
#endif

#define CONSOLE_FONT_NAME_LEN 32
#define CONSOLE_FONT_COUNT 4u
#define CONSOLE_BLINK_MS 250u
#define CONSOLE_ERROR_MSG_MAX_LEN (128U)
#define CONSOLE_DEBUG_MSG_MAX_LEN (128U)

    /** @brief Font descriptor */
    typedef struct console_font
    {
        uint8_t *bitmap;   // NOT A FONTX2 FONT, JUST A 256*8 BITMAP!
        uint16_t size;     // for now, must be 256 * 8 = 2048
        uint16_t codepage; // for now, must be 437 (other 8-bit like 850, Windows-1252 or unicode/UTF-32/UTF-16 for later)
        uint8_t width;     // for now, must be 8
        uint8_t height;    // for now, must be 8
        uint8_t first;     // for now, must be 0
        uint8_t last;      // for now, must be 255
        char *name;
    } console_font;

    extern console_font console_font_bios_f08;

    /** @brief Text cell for each glyph on the screen (3 bytes) */
    typedef struct __packed console_cell
    {
        uint8_t glyph : 8;      /** @brief glyph */
        bool transparent : 1;   /** @brief paper is alpha "color" */
        bool reverse : 1;       /** @brief swap ink & paper */
        bool underline : 1;     /** @brief fill last scanline */
        bool blink : 1;         /** @brief blink glyph if timer is on */
        uint8_t font : 4;       /** @brief up to 16 fonts */
        uint8_t background : 4; /** @brief paper */
        uint8_t foreground : 4; /** @brief ink */
    } console_cell;

#define CONSOLE_CELL_SIZE sizeof(console_cell)

    /** @brief Console state */
    typedef struct console_state
    {
        console_cell *buffer;                          /** @brief console content: glyphs, colors & attributes */
        console_font *fonts[CONSOLE_FONT_COUNT];       /** @brief fonts */
        uint8_t glyph_height, glyph_width;             /** @brief  */
        BGAR5515 palette[16];                          /** @brief palette */
        uint8_t rows;                                  /** @brief screen height in glyphs */
        uint8_t cols;                                  /** @brief screen width in glyphs */
        uint8_t cursor_row;                            /** @brief current cursor line */
        uint8_t cursor_col;                            /** @brief current cursor column */
        bool cursor_on;                                /** @brief show cursor? */
        bool cursor_blink;                             /** @brief true if cursor should blink */
        bool cursor_state;                             /** @brief true if cursor should be visible, depending on blink state */
        console_cell cell;                             /** @brief default attributes including "paper" & "ink" colors */
        bool auto_scroll;                              /** @brief should we auto_scroll at end of console? */
        bool blink_state;                              /** @brief true if timer is up */
        absolute_time_t blink_timer;                   /** @brief timeout time for blink timer */
        uint8_t margins[4];                            /** @brief EVEN number of pixels to show as transparent at edges of screen */
        char error_message[CONSOLE_ERROR_MSG_MAX_LEN]; /** @brief Error message */
        char debug_message[CONSOLE_DEBUG_MSG_MAX_LEN]; /** @brief Debug message */
        // uint16_t screen_width;                         /** @brief screen width, should be same as vgaboard->model */
        // uint16_t screen_height;                        /** @brief screen height, should be same as vgaboard->model */
    } console_state;

#define CONSOLE_STATE_SIZE sizeof(console_state)

    /** @brief Initialize & reset console to defaults, allocate console */
    void console_init(
        console_state *console,
        int plane,
        // uint16_t screen_width, uint16_t screen_height,
        uint8_t margin_top, uint8_t margin_bottom,
        uint8_t margin_left, uint8_t margin_right,
        const BGAR5515 *palette,
        uint8_t cols, uint8_t rows,
        console_cell *buffer);

    /** @brief Reset timer & state */
    void console_timer_init(console_state *console);

    /** @brief Refresh timer & state */
    void console_timer_refresh(console_state *console);

    /** @brief Clear console */
    void console_clear(console_state *console);

    /** @brief Set color palette */
    void console_set_palette(console_state *console, const BGAR5515 *palette);

    /** @brief Set color at palette index */
    void console_set_palette_index(console_state *console, uint8_t index, BGAR5515 color);

    /** @brief Set background color (paper) */
    void console_set_background(console_state *console, uint8_t background);

    /** @brief Set foreground color (ink) */
    void console_set_foreground(console_state *console, uint8_t foreground);

    /** @brief Set blink attribute */
    void console_set_blink(console_state *console, bool blink);

    /** @brief Set reverse attribute */
    void console_set_reverse(console_state *console, bool reverse);

    /** @brief Set transparent attribute */
    void console_set_transparent(console_state *console, bool transparent);

    /** @brief Set underline attribute */
    void console_set_underline(console_state *console, bool underline);

    /** @brief Set cursor off/on & blinking */
    void console_set_cursor(console_state *console, bool state, bool blink);

    /** @brief Show cursor and return previous state */
    bool console_show_cursor(console_state *console);

    /** @brief Hide cursor and return previous state */
    bool console_hide_cursor(console_state *console);

    /** @brief Scroll up */
    void console_scroll_up(console_state *console);

    /** @brief Scroll down */
    void console_scroll_down(console_state *console);

    /** @brief Put raw char into console at row and column (no changes of cursor position) */
    void console_put_char_at(console_state *console, uint8_t row, uint8_t glyphol, uint8_t glyph);

    /** @brief Move cursor. NB: row and col are 0 based, e.g. col is between 0 and 79 for 80 columns */
    void console_move_cursor_to(console_state *console, uint8_t row, uint8_t glyphol);

    /** @brief Get cursor position */
    void console_get_cursor_pos(console_state *console, uint8_t *row, uint8_t *col);

    /** @brief Put raw char into console and advance cursor (no interpretation of TAB, CR, LF nor ESC sequences) */
    void console_put_char(console_state *console, uint8_t glyph);

    /** @brief Put string of raw chars */
    void console_put_string(console_state *console, uint8_t *s);

    /** @brief Initialize renderer (on core1) */
    void console_init_plane(void *plane_state);

    /** @brief Render one scanline of console chars (on core1) */
    uint16_t console_render_scanline(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max);

    /** @brief Dumps console's settings to serial */
    void console_dump_settings(console_state *console);

    /** @brief Dumps console's buffer to serial (ASCII only) */
    void console_dump_buffer(console_state *console);

#ifdef __cplusplus
}
#endif

#endif /* _CONSOLE_H */
