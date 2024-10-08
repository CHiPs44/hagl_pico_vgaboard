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

/* PVTT stands for Pico VGA Text Terminal */

#ifndef _PVTT_H
#define _PVTT_H

#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "pico-vgaboard-text-screen.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum e_pvtt_cursor_movement
    {
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_LEFT,
        MOVE_DOWN,
        MOVE_END,
        MOVE_TOP,
        MOVE_HOME,
        MOVE_BOTTOM
    } pvtt_cursor_movement;

    typedef struct s_pvtt_terminal
    {
        pvts_screen *screen;
        /** @brief scroll screen at top or bottom? */
        bool scroll;
        /** @brief just wrap at beginning and end of screen? */
        bool wrap;
    } pvtt_terminal;

    /** @brief Create a terminal object */
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

    /** @brief Free a terminal object */
    static inline void pvtt_done(pvtt_terminal *term)
    {
        if (term->screen != NULL)
        {
            free(term->screen);
        }
        free(term);
    }

    static inline void pvtt_scroll_up(pvtt_terminal *term)
    {

    }

    static inline void pvtt_move_cursor(pvtt_terminal *term, int move)
    {
        switch (move)
        {
        case MOVE_RIGHT:
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
                    term->screen->col = 0;
                }
                // at end of screen?
                if (term->screen->row == term->screen->rows - 1 && term->scroll)
                {
                    term->screen->col = 0;
                    pvtt_scroll_down(term);
                }
            }
            break;
        case MOVE_UP:
            if (term->screen->row > 0)
            {
                // just go up
                term->screen->row -= 1;
            }
            else
            {
                // at top of screen
                if (term->wrap)
                {
                    // go to last line
                    term->screen->row = term->screen->rows - 1;
                }
                else if (term->scroll)
                {
                    // insert blank line at top of screen
                    pvtt_scroll_down(term);
                }
            }
            break;
        case MOVE_LEFT:
            if (term->screen->col > 0)
            {
                term->screen->col -= 1;
            }
            else
            {
                // at start of line
                if (term->wrap)
                {
                    // go to last column
                    term->screen->col = term->screen->cols - 1;
                }
                // at start of screen?
                if (term->scroll)
                {
                }
            }
            break;
        case MOVE_DOWN:
            if (term->screen->row < term->screen->rows - 1)
            {
                // just go down
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
            break;
        }
    }

#ifdef __cplusplus
}
#endif

#endif /* _PVTT_H */
