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

    typedef struct _pico_vgaboard_textcell_t
    {
        uint8_t c; // character to display
        uint8_t b; // background color
        uint8_t f; // foreground color
        uint8_t i; // font index
    } pico_vgaboard_textcell_t;

    typedef enum _pico_vgaboard_textcursor_shape_t
    {
        HORIZONTAL_LINE,
        BLOCK,
        VERTICAL_LINE,
        REVERSE_BLOCK,
    } pico_vgaboard_textcursor_shape_t;

    typedef enum _pico_vgaboard_textcursor_animation_t
    {
        OFF,
        FIXED,
        FLASH_SLOW,
        FLASH_FAST,
    } pico_vgaboard_textcursor_animation_t;

    typedef struct _pico_vgaboard_textcursor_t
    {
        pico_vgaboard_textcursor_shape_t shape;
        pico_vgaboard_textcursor_animation_t animation;
        uint8_t column;
        uint8_t line;
    } pico_vgaboard_textcursor_t;

    typedef struct pico_vgaboard_textmode_t
    {
        pico_vgaboard_font_t **fonts;
        pico_vgaboard_textcell_t **screen;
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
