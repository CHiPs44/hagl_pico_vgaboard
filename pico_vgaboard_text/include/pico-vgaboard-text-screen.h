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

/* PVGA_CONSOLE stands for Pico VGA Console */

#ifndef _PVGA_CONSOLE_H
#define _PVGA_CONSOLE_H

#include <stdint.h>
#include <types.h>

#include "bios-f08.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PVGA_CONSOLE_FONT_NAME_LEN 32

    typedef struct s_pvga_console_font
    {
        // NOT A FONTX2 FONT, JUST A 256*8 BITMAP!
        uint8_t *bitmap;
        uint16_t size;     // must be 256x8=2048
        uint16_t codepage; // must be 437 (other 8-bit like Windows-1252 or unicode/UTF-32/UTF-16 for later)
        uint8_t width;     // must be 8
        uint8_t height;    // must be 8
        uint8_t first;     // must be 0
        uint8_t last;      // must be 255
        char *name;
    } t_pvga_console_font;

    /** @brief Canonical 8x8 BIOS US font from IBM */
    static t_pvga_console_font bios_f08_font = {
        .bitmap = &bios_f08,
        .size = bios_f08_len,
        .codepage = 437,
        .width = 8,
        .height = 8,
        .first = 0,
        .last = 255,
        .name = "[CP437] BIOS.F08"};

    /** @brief Example for an ASCII only 6x8 font */
    static t_pvga_console_font ascii_5x8_font = {
        .bitmap = NULL, // &ascii_5x8,
        .size = 0,      // ascii_5x8_len,
        .codepage = 0,
        .width = 6,
        .height = 8,
        .first = 32,
        .last = 126,
        .name = "[ASCII] 6x8 (TODO)"};

    /** @brief Character attributes */
    typedef enum e_pvga_console_attributes
    {
        /** @brief Set if background should be transparent, opaque otherwise */
        PVGA_CONSOLE_TRANSPARENT = 0b10000000,
        /** @ brief Set if background & foreground colors should be swapped */
        PVGA_CONSOLE_REVERSE = 0b01000000,
        /** @ brief Set if last line should be on */
        PVGA_CONSOLE_UNDERLINE = 0b00100000,
        /** @ brief Set if character should blink at "slow" pace */
        PVGA_CONSOLE_BLINK = 0b00010000,
        /** @ brief Reserved for future use */
        PVGA_CONSOLE_RESERVED2 = 0b00001000,
        /** @ brief Font number mask */
        PVGA_CONSOLE_FONT_MASK = 0b00000011,
        /** @ brief Font count (with 4 fonts, we could do normal, bold, italics & bold italics for example)*/
        PVGA_CONSOLE_FONT_COUNT = PVGA_CONSOLE_FONT_MASK + 1
    } t_pvga_console_attributes;

    /**
     * @brief Text cell for each character (32 bits / 4 bytes)
     * With 8x8 font:
     *  1024x768 based modes:
     *      256x192: 3,072 bytes for 32x24
     *      512x256: 4,800 bytes for 64x32
     *      512x384: 4,800 bytes for 64x48
     *  640x400 based modes:
     *      320x200: 4,000 bytes for 40x25
     *      640x200: 8,000 bytes for 80x25
     *  640x480 based modes:
     *      320x240: 4,800 bytes for 40x30
     *      640x240: 9,600 bytes for 80x30
     */
    typedef struct s_pvga_console_cell
    {
        /** @brief character */
        uint8_t ch;
        /** @brief background color */
        uint8_t bg;
        /** @brief foreground color */
        uint8_t fg;
        /** @brief attributes */
        uint8_t at;
    } t_pvga_console_cell;

#define PTVS_BLINK_FAST 250u
#define PTVS_BLINK_SLOW 500u

    typedef enum e_pvga_console_cursor_animation
    {
        /** @brief hide cursor */
        CURSOR_HIDDEN = 0,
        /** @brief show fixed cursor */
        CURSOR_FIXED = 1,
        /** @brief blink cursor at 250 ms intervals */
        CURSOR_BLINK_FAST = 2,
        /** @brief blink cursor at 500 ms intervals */
        CURSOR_BLINK_SLOW = 3,
    } t_pvga_console_cursor_animation;

    /** @brief Text console state */
    typedef struct s_pvga_console_screen t_pvga_console;

    /** @brief Reset timers & states */
    void pvga_console_timers_init(t_pvga_console *console);

    /** @brief Refresh timers & states */
    void pvga_console_timers_refresh(t_pvga_console *console);

    /** @brief Clear console */
    void pvga_console_clear(t_pvga_console *console);

    /** @brief Reset console to defaults */
    void pvga_console_reset(t_pvga_console *console, uint8_t cols, uint8_t rows);

    /** @brief Allocate console & console buffer, set defaults & clear it */
    t_pvga_console *pvga_console_init(uint8_t cols, uint8_t rows);

    /** @brief Free console & console */
    void pvga_console_done(t_pvga_console *console);

    /** @brief Set color palette & mask */
    void pvga_console_set_palette(t_pvga_console *console, uint16_t *palette, uint8_t color_mask);

    /** @brief Set background color (paper) */
    void pvga_console_set_background(t_pvga_console *console, uint8_t background);

    /** @brief Set foreground clor (ink) */
    void pvga_console_set_foreground(t_pvga_console *console, uint8_t foreground);

    /** @brief Set character attributes */
    void pvga_console_set_attributes(t_pvga_console *console, t_pvga_console_attributes attributes);

    /** @brief Scroll up */
    void pvga_console_scroll_up(t_pvga_console *console);

    /** @brief Scroll down */
    void pvga_console_scroll_down(t_pvga_console *console);

    /** @brief Put raw char into console at row and colmun (no changes of cursor position) */
    void pvga_console_put_char_at(t_pvga_console *console, uint8_t row, uint8_t col, uint8_t c);

    /** @brief Move cursor. NB: row and col are 0 based, e.g. col is between 0 and 79 for 80 columns */
    void pvga_console_move_cursor(t_pvga_console *console, uint8_t row, uint8_t col);

    /** @brief Put raw char into console and advance cursor (no interpretation of TAB, CR, LF nor ESC sequences) */
    void pvga_console_put_char(t_pvga_console *console, uint8_t c);

    /** @brief Put string of raw chars */
    void pvga_console_put_string(t_pvga_console *console, uint8_t *s);

    /** @brief Render one line of console chars */
    uint16_t pvga_console_render_scanline(void *plane_params, uint32_t scanline_id, uint32_t *data, uint16_t data_max)

#ifdef __cplusplus
}
#endif

#endif /* _PVGA_CONSOLE_H */
