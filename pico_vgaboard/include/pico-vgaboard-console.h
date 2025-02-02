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

#ifndef _PICO_VGABOARD_CONSOLE_H
#define _PICO_VGABOARD_CONSOLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_CONSOLE_FONT_NAME_LEN 32
#define PICO_VGABOARD_CONSOLE_FONT_COUNT 4u
#define PICO_VGABOARD_CONSOLE_BLINK_FAST_MS 250u
#define PICO_VGABOARD_CONSOLE_BLINK_SLOW_MS 500u
#define PICO_VGABOARD_CONSOLE_ERROR_MSG_MAX_LEN (128U)
#define PICO_VGABOARD_CONSOLE_DEBUG_MSG_MAX_LEN (128U)

    /** @brief Font descriptor */
    typedef struct pico_vgaboard_console_font
    {
        uint8_t *bitmap;   // NOT A FONTX2 FONT, JUST A 256*8 BITMAP!
        uint16_t size;     // must be 256 * 8 = 2048
        uint16_t codepage; // must be 437 (other 8-bit like 850, Windows-1252 or unicode/UTF-32/UTF-16 for later)
        uint8_t width;     // must be 8 for now
        uint8_t height;    // must be 8 for now
        uint8_t first;     // must be 0 for now
        uint8_t last;      // must be 255 for now
        char *name;
    } pico_vgaboard_console_font;

    extern pico_vgaboard_console_font pico_vgaboard_console_font_bios_f08;

    /** @brief Text cell for each character on the screen (3 bytes) */
    typedef struct __packed pico_vgaboard_console_cell
    {
        uint8_t c : 8; /** @brief character */
        bool transparent : 1;
        bool reverse : 1;
        bool underline : 1;
        bool blink : 1;
        bool reserved1 : 1;
        bool reserved2 : 1;
        uint8_t font : 2;
        uint8_t background : 4;
        uint8_t foreground : 4;
    } pico_vgaboard_console_cell;

    /** @brief Cursor shape: off, block, right, bottom */
    typedef enum pico_vgaboard_console_cursor_shape
    {
        PICO_VGABOARD_CONSOLE_CURSOR_OFF,    /** @brief no cursor */
        PICO_VGABOARD_CONSOLE_CURSOR_BLOCK,  /** @brief block cursor */
        PICO_VGABOARD_CONSOLE_CURSOR_RIGHT,  /** @brief right vertical line */
        PICO_VGABOARD_CONSOLE_CURSOR_BOTTOM, /** @brief bottom line */
    } pico_vgaboard_console_cursor_shape;

    /** @brief Cursor animation: still, blink fast, blink slow */
    typedef enum pico_vgaboard_console_cursor_animation
    {
        PICO_VGABOARD_CONSOLE_CURSOR_STILL,      /** @brief show still/fixed cursor */
        PICO_VGABOARD_CONSOLE_CURSOR_BLINK_FAST, /** @brief blink cursor at 250 ms intervals */
        PICO_VGABOARD_CONSOLE_CURSOR_BLINK_SLOW, /** @brief blink cursor at 500 ms intervals */
    } pico_vgaboard_console_cursor_animation;

    /** @brief Console state */
    typedef struct pico_vgaboard_console
    {
        pico_vgaboard_console_cell *buffer;                                  /** @brief console content: chars, colors & attributes */
        pico_vgaboard_console_font *fonts[PICO_VGABOARD_CONSOLE_FONT_COUNT]; /** @brief fonts */
        BGAR5515 palette[16];                                                /** @brief palette */
        uint8_t rows;                                                        /** @brief screen height in characters */
        uint8_t cols;                                                        /** @brief screen width in characters */
        pico_vgaboard_console_cell cell;                                     /** @brief default attributes including "paper" & "ink" colors */
        bool auto_scroll;                                                    /** @brief should we auto_scroll at end of console? */
        bool cursor_on;                                                      /** @brief show cursor? */
        uint8_t cursor_row;                                                  /** @brief current cursor line */
        uint8_t cursor_col;                                                  /** @brief current cursor column */
        pico_vgaboard_console_cursor_shape cursor_shape;                     /** @brief shape of cursor: off/block/right/bottom/... */
        pico_vgaboard_console_cursor_animation cursor_anim;                  /** @brief animation of cursor: none/... */
        bool state_fast;                                                     /** @brief true if "fast" timer is up */
        bool state_slow;                                                     /** @brief true if "slow" timer is up */
        absolute_time_t timer_fast;                                          /** @brief timeout time for "fast" timer */
        absolute_time_t timer_slow;                                          /** @brief timeout time for "slow" timer */
        uint16_t screen_width;                                               /** @brief screen width, should be same as vgaboard->model */
        uint16_t screen_height;                                              /** @brief screen height, should be same as vgaboard->model */
        uint8_t margins[4];                                                  /** @brief EVEN number of pixels to show as transparent at edges of screen */
        char error_message[PICO_VGABOARD_CONSOLE_ERROR_MSG_MAX_LEN];         /** @brief Error message */
        char debug_message[PICO_VGABOARD_CONSOLE_DEBUG_MSG_MAX_LEN];         /** @brief Debug message */
    } pico_vgaboard_console;

    /** @brief Initialize & reset console to defaults, allocate console */
    void pico_vgaboard_console_init(
        pico_vgaboard_console *console,
        int plane,
        uint16_t screen_width, uint16_t screen_height,
        uint8_t margin_top, uint8_t margin_bottom,
        uint8_t margin_left, uint8_t margin_right,
        const uint16_t *palette,
        uint8_t cols, uint8_t rows,
        pico_vgaboard_console_cell *buffer);

    /** @brief Reset timers & states */
    void pico_vgaboard_console_timers_init(pico_vgaboard_console *console);

    /** @brief Refresh timers & states */
    void pico_vgaboard_console_timers_refresh(pico_vgaboard_console *console);

    /** @brief Clear console */
    void pico_vgaboard_console_clear(pico_vgaboard_console *console);

    /** @brief Set color palette & mask */
    void pico_vgaboard_console_set_palette(pico_vgaboard_console *console, const uint16_t *palette);

    /** @brief Set background color (paper) */
    void pico_vgaboard_console_set_background(pico_vgaboard_console *console, uint8_t background);

    /** @brief Set foreground clor (ink) */
    void pico_vgaboard_console_set_foreground(pico_vgaboard_console *console, uint8_t foreground);

    /** @brief Set blink attribute */
    void pico_vgaboard_console_set_blink(pico_vgaboard_console *console, bool blink);

    /** @brief Set reverse attribute */
    void pico_vgaboard_console_set_reverse(pico_vgaboard_console *console, bool reverse);

    /** @brief Set transparent attribute */
    void pico_vgaboard_console_set_transparent(pico_vgaboard_console *console, bool transparent);

    /** @brief Set underline attribute */
    void pico_vgaboard_console_set_underline(pico_vgaboard_console *console, bool underline);

    /** @brief Set cursor shape (block/right/bottom/...) & animation (none/fixed/fast/slow/...) */
    void pico_vgaboard_console_set_cursor(pico_vgaboard_console *console, pico_vgaboard_console_cursor_shape shape, pico_vgaboard_console_cursor_animation anim);

    /** @brief Show cursor and return previous state */
    bool pico_vgaboard_console_show_cursor(pico_vgaboard_console *console);

    /** @brief Hide cursor and return previous state */
    bool pico_vgaboard_console_hide_cursor(pico_vgaboard_console *console);

    /** @brief Scroll up */
    void pico_vgaboard_console_scroll_up(pico_vgaboard_console *console);

    /** @brief Scroll down */
    void pico_vgaboard_console_scroll_down(pico_vgaboard_console *console);

    /** @brief Put raw char into console at row and column (no changes of cursor position) */
    void pico_vgaboard_console_put_char_at(pico_vgaboard_console *console, uint8_t row, uint8_t col, uint8_t c);

    /** @brief Move cursor. NB: row and col are 0 based, e.g. col is between 0 and 79 for 80 columns */
    void pico_vgaboard_console_move_cursor_to(pico_vgaboard_console *console, uint8_t row, uint8_t col);

    /** @brief Put raw char into console and advance cursor (no interpretation of TAB, CR, LF nor ESC sequences) */
    void pico_vgaboard_console_put_char(pico_vgaboard_console *console, uint8_t c);

    /** @brief Put string of raw chars */
    void pico_vgaboard_console_put_string(pico_vgaboard_console *console, uint8_t *s);

    /** @brief Initialize renderer (on core1) */
    void pico_vgaboard_console_init_plane(void *plane_state);

    /** @brief Render one scanline of console chars (on core1) */
    uint16_t pico_vgaboard_console_render_scanline(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max);

    /** @brief Dumps console's settings to serial */
    void pico_vgaboard_console_dump_settings(pico_vgaboard_console *console);

    /** @brief Dumps console's buffer to serial (ASCII only) */
    void pico_vgaboard_console_dump_buffer(pico_vgaboard_console *console);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_CONSOLE_H */
