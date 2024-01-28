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

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_FONT_NAME_LENGTH 31
#define PICO_VGABOARD_FONT_MAX 4

    typedef struct _pico_vgaboard_textcell_t
    {
        uint8_t c; // character
        uint8_t b; // background color
        uint8_t f; // foreground color
        uint8_t i; // font index
    } pico_vgaboard_textcell_t;

    typedef enum _pico_vgaboard_textcursor_type_t
    {
        TEXTCURSOR_TYPE_BLOCK,
        TEXTCURSOR_TYPE_HORIZONTAL_LINE,
        TEXTCURSOR_TYPE_VERTICAL_LINE,
        TEXTCURSOR_TYPE_REVERSE_BLOCK,
    } pico_vgaboard_textcursor_type_t;

    typedef enum _pico_vgaboard_textcursor_anim_t
    {
        TEXTCURSOR_ANIM_OFF,
        TEXTCURSOR_ANIM_FIXED,
        TEXTCURSOR_ANIM_FLASH_SLOW,
        TEXTCURSOR_ANIM_FLASH_FAST,
    } pico_vgaboard_textcursor_anim_t;

    typedef struct _pico_vgaboard_textcursor_t
    {
        pico_vgaboard_textcursor_type_t type;
        pico_vgaboard_textcursor_anim_t anim;
        uint8_t col;
        uint8_t row;
        bool status; // OFF=0, ON=1
        uint16_t countdown;
    } pico_vgaboard_textcursor_t;

    typedef struct _pico_vgaboard_font_t
    {
        uint8_t *data;
        uint16_t size;
        uint16_t codepage;
        // uint8_t width; must be 8
        uint8_t height;
        uint8_t first;
        uint8_t last;
        char name[PICO_VGABOARD_FONT_NAME_LENGTH + 1];
    } pico_vgaboard_font_t;

    typedef struct pico_vgaboard_textmode_t
    {
        pico_vgaboard_font_t *fonts[PICO_VGABOARD_FONT_MAX];
        pico_vgaboard_textcell_t **screen;
        uint8_t *palette;
        uint8_t font_count;
        uint8_t columns;
        uint8_t lines;
        pico_vgaboard_textcursor_t cursor;
    } pico_vgaboard_textmode_t;

    extern pico_vgaboard_textmode_t pico_vgaboard_textmode;

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_TEXT_H */
