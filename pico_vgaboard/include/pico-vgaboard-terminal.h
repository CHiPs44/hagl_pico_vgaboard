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

/* PICO_VGABOARD_TERMINAL stands for Pico VGA Text Terminal */

#ifndef _PICO_VGABOARD_TERMINAL_H
#define _PICO_VGABOARD_TERMINAL_H

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "pico-vgaboard-console.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /** Organized like a numeric pad */
    typedef enum e_pvtt_cursor_move
    {
        MOVE_END = 1,
        MOVE_DOWN = 2,
        MOVE_BOTTOM = 3,
        MOVE_LEFT = 4,
        MOVE_RIGHT = 6,
        MOVE_HOME = 7,
        MOVE_UP = 8,
        MOVE_TOP = 9,
    } pvtt_cursor_move;

    /** @brief Treat CR and LF as is, CR means CR+LF (MacOS), LF means CR+LF (Unix) */
    typedef enum e_pvtt_end_of_line
    {
        EOL_AS_IS = '\0',
        EOL_CR = '\r',
        EOL_LF = '\n',
    } pvtt_end_of_line;

    typedef struct s_pvtt_terminal
    {
        pvts_screen *screen;
        /** @brief scroll screen at top or bottom? */
        bool scroll;
        /** @brief just wrap at beginning and end of screen? */
        bool wrap;
        /** @brief CR/LF treatment */
        pvtt_end_of_line eol;
    } pvtt_terminal;

    /** @brief Create a terminal */
    static inline pvts_screen *pvtt_terminal *pvtt_init(uint8_t cols, uint8_t rows)
    {
        pvts_screen *screen = pvts_init(cols, rows);
        if (screen == NULL)
            return NULL;
        pvtt_terminal *term = calloc(1, sizeof(pvtt_terminal));
        if (term == NULL)
        {
            free(screen);
            return NULL;
        }
        term->screen = screen;
        scroll = false;
    }

    /** @brief Free a terminal previously allocated */
    static inline void pvtt_done(pvtt_terminal *term)
    {
        if (term->screen != NULL)
        {
            pvts_done(term->screen);
        }
        free(term);
    }

    /** @brief Insert blank line at bottom of screen */
    static inline void pvtt_scroll_up(pvtt_terminal *term)
    {
        pvts_scroll_up(term->screen);
    }

    static inline void pvtt_scroll_down(pvtt_terminal *term)
    {
        pvts_scroll_down(term->screen);
    }

    static inline void pvtt_move_cursor_to_end(pvtt_terminal *term)
    {
        term->screen->col = term->screen->cols - 1;
    }

    static inline void pvtt_move_cursor_down(pvtt_terminal *term)
    {
        // just go down?
        if (term->screen->row < term->screen->rows - 1)
        {
            term->screen->row += 1;
        }
        else
        {
            if (term->wrap)
            {
                // go to first line
                term->screen->row = 0;
            }
            else if (term->scroll)
            {
                // insert blank line at bottom of screen
                pvtt_scroll_up(term);
            }
        }
    }

    static inline void pvtt_move_cursor_to_bottom(pvtt_terminal *term)
    {
        term->screen->row = term->screen->rows - 1;
    }

    static inline void pvtt_move_cursor_left(pvtt_terminal *term)
    {
        if (term->screen->col > 0)
        {
            // just move left
            term->screen->col -= 1;
        }
        else
        {
            // already at start of line
            if (term->wrap)
            {
                // go to last column
                term->screen->col = term->screen->cols - 1;
            }
            // at start of screen?
            if (term->scroll && term->screen->row == 0)
            {
                pvtt_scroll_down(term);
            }
        }
    }

    static inline void pvtt_move_cursor_right(pvtt_terminal *term)
    {
        // at end of line?
        if (term->screen->col < term->screen->cols - 1)
        {
            // no, just move right
            term->screen->col += 1;
        }
        else
        {
            if (term->wrap)
            {
                // go back to beginning of line
                term->screen->col = 0;
            }
            // at end of screen?
            if (term->screen->row == term->screen->rows - 1 && term->scroll)
            {
                // insert blank line at bottom of screen
                pvtt_scroll_up(term);
                // go back to beginning of line
                term->screen->col = 0;
            }
            // whenever nor wrap nor scroll are up, keep cursor at bottom right of screen
        }
    }

    static inline void pvtt_move_cursor_to_home(pvtt_terminal *term)
    {
        term->screen->col = 0;
    }

    static inline void pvtt_move_cursor_up(pvtt_terminal *term)
    {
        if (term->screen->row > 0)
        {
            // just go up
            term->screen->row -= 1;
        }
        else
        {
            // already at top of screen
            if (term->wrap)
            {
                // move to last line
                term->screen->row = term->screen->rows - 1;
            }
            else if (term->scroll)
            {
                // insert blank line at top of screen
                pvtt_scroll_down(term);
            }
        }
    }

    static inline void pvtt_move_cursor_to_top(pvtt_terminal *term)
    {
        term->screen->row = 0;
    }

    static inline void pvtt_move_cursor(pvtt_terminal *term, int move)
    {
        switch (move)
        {
        case MOVE_END:
            pvtt_cursor_move_cursor_to_end(term);
            break;
        case MOVE_DOWN:
            pvtt_cursor_move_down(term);
            break;
        case MOVE_BOTTOM:
            pvtt_cursor_move_bottom(term);
            break;
        case MOVE_LEFT:
            pvtt_cursor_move_left(term);
            break;
        case MOVE_RIGHT:
            pvtt_cursor_move_right(term);
            break;
        case MOVE_HOME:
            pvtt_cursor_move_home(term);
            break;
        case MOVE_UP:
            pvtt_cursor_move_up(term);
            break;
        case MOVE_TOP:
            pvtt_cursor_move_top(term);
            break;
        }
    }

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_TERMINAL_H */
