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

   typedef enum e_pvtt_direction
   {
        DIR_RIGHT=0, DIR_UP=1, DIR_LEFT=2, DIR_DOWN=3
    } pvtt_direction;

    typedef struct s_pvtt_terminal
    {
        pvts_screen *screen;
        bool        scroll;
    } pvts_terminal;

    static inline pvts_terminal *pvtt_init(pvts_screen *screen)
    {
    }

    static inline void pvtt_done(pvts_terminal *term)
    {
    }

    static inline void pvtt_move_cursor(pvts_terminal *term, int dir)
    {
     switch (dir) {
         case DIR_RIGHT:
           break;
         case DIR_UP:
        if (term->screen->row > 0)
        {
             term->screen->row -= 1;
        } else {
             if (term->scroll)
             {
                 pvtt_scroll_up(term);
             }
        }
        break;
         case DIR_LEFT:
           break;
         case DIR_DOWN:
        if (term->screen->row < term->screen->rows - 1)
        {
             term->screen->row += 1;
        } else {
             if (term->scroll)
             {
                 pvtt_scroll_down(term);
             }
        }
           break;
    }
    }

#ifdef __cplusplus
}
#endif

#endif /* _PVTT_H */
