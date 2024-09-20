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

#ifndef _PICO_VGABOARD_TEXT_H
#define _PICO_VGABOARD_TEXT_H

#include <stdint.h>
#include <malloc.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_FONT_NAME_LENGTH 31
#define PICO_VGABOARD_FONT_COUNT 8

    typedef struct s_pico_vgaboard_text_font
    {
        // NOT A FONTX2 FONT, JUST A BITMAP!
        uint8_t *data;
        // uint16_t size; must be 8×256=2048
        // uint16_t codepage; must be 437 (other 8-bit or unicode powa for later)
        // uint8_t width; must be 8
        // uint8_t height; must be 8
        // uint8_t first; must be 0
        // uint8_t last; must be 255
        char name[PICO_VGABOARD_FONT_NAME_LENGTH + 1];
    } pico_vgaboard_text_font;

    // typedef enum _pico_vgaboard_text_attributes
    // {
    //     /* clang-format off */
    //     ATTRIBUTE_NONE      = 0x00000000,
    //     ATTRIBUTE_REVERSE   = 0x00000001,
    //     ATTRIBUTE_BOLD      = 0x00000010,
    //     ATTRIBUTE_UNDERLINE = 0x00000100,
    //     ATTRIBUTE_ITALIC    = 0x00001000,
    //     ATTRIBUTE_STRIKE    = 0x00010000,
    //     ATTRIBUTE_RESERVED  = 0x00100000,
    //     ATTRIBUTE_FONT0     = 0x00000000,
    //     ATTRIBUTE_FONT1     = 0x00100000,
    //     ATTRIBUTE_FONT2     = 0x01000000,
    //     ATTRIBUTE_FONT3     = 0x01100000,
    //     ...
    //     ATTRIBUTE_FONT7     = 0x11100000,
    //     /* clang-format on */
    // } pico_vgaboard_text_attributes;

    typedef struct s_pico_vgaboard_text_attributes
    {
        bool reverse : 1;
        bool bold : 1;
        bool underline : 1;
        bool italic : 1;
        bool strike : 1;
        uint8_t font : 3;
    } pico_vgaboard_text_attributes;

    /**
     * @brief Text cell for each char
     */
    typedef struct s_pico_vgaboard_text_cell
    {
        /* clang-format off */
                    // bits
        uint8_t c;  //  8 character
        uint8_t b;  //  8 background color
        uint8_t f;  //  8 foreground color
        uint8_t a;  //  8 attributes
                    // 32 bits / 4 bytes => 8,000 bytes for 80x25
        /* clang-format on */
    } pico_vgaboard_text_cell;

    typedef enum e_pico_vgaboard_text_cursor_anim
    {
        TEXT_CURSOR_ANIM_OFF = 0,
        TEXT_CURSOR_ANIM_FIXED,
        TEXT_CURSOR_ANIM_FLASH_SLOW,
        TEXT_CURSOR_ANIM_FLASH_FAST,
    } pico_vgaboard_textcursor_anim;

    typedef struct s_pico_vgaboard_text
    {
        pico_vgaboard_text_cell **screen;
        pico_vgaboard_text_font *font[PICO_VGABOARD_FONT_COUNT];
        uint16_t *palette;
        uint8_t cols;
        uint8_t rows;
        uint8_t background;
        uint8_t foreground;
        uint8_t attributes;
        bool scroll;
        // cursor
        pico_vgaboard_textcursor_anim anim;
        uint8_t col;
        uint8_t row;
        bool state;
        uint64_t timer;
    } pico_vgaboard_text;

    static inline pico_vgaboard_text *pico_vgaboard_text_init(uint8_t cols, uint8_t rows)
    {
        pico_vgaboard_text *text = calloc(1, sizeof(pico_vgaboard_text));
        if (text == NULL)
            return NULL;
        text->cols = cols;
        text->rows = rows;
        text->screen = calloc(cols * rows, sizeof(pico_vgaboard_text_cell));
        if (text->screen == NULL)
        {
            free(text);
            return NULL;
        }
        // cursor
        text->col = 0;
        text->row = 0;
        return text;
    }

    static inline void pico_vgaboard_text_done(pico_vgaboard_text *text)
    {
        if (text == NULL)
            return;
        if (text->screen != NULL)
            free(text->screen);
        free(text);
    }

    // void pico_vgaboard_text_set_background(pico_vgaboard_text *text, uint8_t background);
    // void pico_vgaboard_text_set_foreground(pico_vgaboard_text *text, uint8_t foreground);
    // void pico_vgaboard_text_set_attributes(pico_vgaboard_text *text, uint8_t attributes);

    static inline pico_vgaboard_text_move_to(pico_vgaboard_text *text, uint8_t row, uint8_t col)
    {
        text->col = col > text->cols - 1 ? text->cols - 1 : col;
        text->row = row > text->rows - 1 ? text->rows - 1 : row;
    }

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_TEXT_H */
