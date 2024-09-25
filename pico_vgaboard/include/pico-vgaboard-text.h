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

#ifndef _PVGA_TEXT_H
#define _PVGA_TEXT_H

#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <wchar.h>

#include "palettes.h"
#include "bios-f08.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PVGA_TEXT_FONT_NAME_LEN 32

    typedef struct s_pvga_text_font
    {
        // NOT A FONTX2 FONT, JUST A 256*8 BITMAP!
        uint8_t *bitmap;
        uint16_t size; // must be 8×256=2048
        // uint16_t codepage; must be 437 (other 8-bit likek Windows-1252 or unicode/UTF-32/UTF-16 for later)
        // uint8_t width; must be 8
        // uint8_t height; must be 8
        // uint8_t first; must be 0
        // uint8_t last; must be 255
        char name[PVGA_TEXT_FONT_NAME_LEN];
    } pvga_text_font;

    static pvga_text_font bios_f08_font = {.data = &bios_f08, .size = bios_f08_len, .name = "BIOS.F08 [CP437]"};

    typedef enum e_pvga_text_attributes =
        {
            TEXT_ATTR_TRANSPARENT = 0b10000000,
            TEXT_ATTR_REVERSE = 0b01000000,
            TEXT_ATTR_UNDERLINE = 0b00100000,
    } pvga_text_attributes;

#define PVGA_TEXT_FONT_MASK 0b00000111
#define PVGA_TEXT_FONT_COUNT 8

    /**
     * @brief Text cell for each char
     *        32 bits / 4 bytes => 8,000 bytes for 80x25
     */
    typedef struct s_pvga_text_cell
    {
        /** @brief character */
        uint8_t c;
        /** @brief background color */
        uint8_t b;
        /** @brief foreground color */
        uint8_t f;
        /** @brief attributes */
        uint8_t a;
    } pvga_text_cell;

    typedef enum e_pvga_text_cursor_anim
    {
        TEXT_CURSOR_ANIM_HIDDEN = 0,
        TEXT_CURSOR_ANIM_FIXED,
        TEXT_CURSOR_ANIM_FLASH_SLOW,
        TEXT_CURSOR_ANIM_FLASH_FAST,
    } pico_vgaboard_textcursor_anim;

    /** @brief Text screen state */
    typedef struct s_pico_vgaboard_text
    {
        pvga_text_cell **screen;
        pvga_text_font *fonts[PVGA_TEXT_FONT_COUNT];
        uint8_t cols;
        uint8_t rows;
        uint16_t *palette;
        uint8_t color_mask; // 0x0f for 16 colors, 0xff for 256 colors
        uint8_t background;
        uint8_t foreground;
        pvga_text_attributes attributes;
        bool scroll;
        // cursor
        uint8_t col;
        uint8_t row;
        pico_vgaboard_textcursor_anim anim;
        bool state;
        uint64_t timer;
    } pico_vgaboard_text;

    /** @brief Clear screen */
    static inline pico_vgaboard_text *pvga_text_clear(pico_vgaboard_text *text)
    {
        pvga_text_cell cell = {
            .c = '\0',
            .a = TEXT_ATTR_TRANSPARENT,
            .b = 0,
            .f = 0xff & text->color_mask};
        uint16_t offset = 0;
        for (uint8_t row = 0; row <= text->rows; row += 1)
        {
            for (uint8_t col = 0; col <= text->cols; col += 1)
            {
                memcpy(text->screen[offset], &cell, sizeof(pvga_text_cell));
                offset += sizeof(pvga_text_cell);
            }
        }
    }

    /** @brief Reset text to defaults */
    static inline pico_vgaboard_text *pvga_text_reset(pico_vgaboard_text *text, uint8_t cols, uint8_t rows)
    {
        // default font at 0 and clear others
        text->fonts[0] = &bios_f08_font;
        for (uint_t i = 1; i < PVGA_TEXT_FONT_COUNT; i += 1)
            text->fonts[i] = NULL;
        // default palette
        pvga_text_set_palette(text, palette_8bpp_default, 0xff);
        // default colors
        text->background = 0;
        text->foreground = 0xff & text->color_mask;
        // default attributes
        text->attributes = TEXT_ATTR_TRANSPARENT;
        // reset cursor position & hide it
        text->col = 0;
        text->row = 0;
        text->anim = TEXT_CURSOR_ANIM_HIDDEN;
        pvga_text_clear(text);
    }

    /** @brief Allocate text & screen and set defaults */
    static inline pico_vgaboard_text *pvga_text_init(uint8_t cols, uint8_t rows)
    {
        pico_vgaboard_text *text = calloc(1, sizeof(pico_vgaboard_text));
        if (text == NULL)
            return NULL;
        text->cols = cols;
        text->rows = rows;
        text->screen = calloc(cols * rows, sizeof(pvga_text_cell));
        if (text->screen == NULL)
        {
            free(text);
            return NULL;
        }
        // defaults
        pvga_text_reset(text);
        // done!
        return text;
    }

    /** @brief Free screen & text */
    static inline void pvga_text_done(pico_vgaboard_text *text)
    {
        if (text == NULL)
            return;
        if (text->screen != NULL)
            free(text->screen);
        free(text);
    }

    /** @brief Set color palette & mask */
    static inline void pvga_text_set_palette(pico_vgaboard_text *text, uint16_t *palette, uint8_t color_mask)
    {
        text->palette = palette;
        text->color_mask = color_mask;
    }

    /** @brief Set background color (paper) */
    static inline void pvga_text_set_background(pico_vgaboard_text *text, uint8_t background)
    {
        text->background = background & text->color_mask;
    }

    /** @brief Set foreground clor (ink) */
    static inline void pvga_text_set_foreground(pico_vgaboard_text *text, uint8_t foreground)
    {
        text->foreground = foreground & text->color_mask;
    };

    /** @brief Set character attributes */
    static inline void pvga_text_set_attributes(pico_vgaboard_text *text, pvga_text_attributes attributes)
    {
        text->attributes = attributes;
    };

    /** @brief Move cursor. NB: row and col are 0 based, e.g. col is between 0 and 79 for 80 columns */
    static inline void pvga_text_move_cursor(pico_vgaboard_text *text, uint8_t row, uint8_t col)
    {
        text->col = col > text->cols - 1 ? text->cols - 1 : col;
        text->row = row > text->rows - 1 ? text->rows - 1 : row;
    }

    /** @brief Put raw char into screen and advance cursor (no interpretation of TAB, CR, LF nor ESC sequences) */
    static inline void pvga_text_put_char(pico_vgaboard_text *text, char c)
    {
        uint16_t offset = text->row * text->rows + text->col;
        text->screen[offset]->a = text->attributes;
        text->screen[offset]->b = text->background;
        text->screen[offset]->f = text->foreground;
        text->screen[offset]->c = c;
        text->col += 1;
        if (text->col >= text->cols)
        {
            text->col = 0;
            text->row += 1;
            // for now, just wrap to top of screen, ignore text->scroll setting
            if (text->row >= text->rows)
            {
                text->row = 0;
            }
        }
    }

    /** @brief Put string of raw chars */
    static inline void pvga_text_put_string(pico_vgaboard_text *text, char *s)
    {
        while (*s)
        {
            pvga_text_put_char(text, *s++);
        }
    }

    static inline void pvga_text_render_scanline(pico_vgaboard_text *text, scanvideo_scanline_buffer_t *buffer)
    {
        uint32_t *scanline_colors = buffer->data;
        uint8_t line = scanvideo_scanline_number(buffer->scanline_id) % 8; // font height
        pvga_text_cell cell;
        uint8_t *char_ptr;
        uint8_t pixels;
        bool bit;
        uint8_t mask, i;
        uint32_t p[0];
        uint8_t b, f;
        bool t, r, u;
        // offset of line of chars in font bitmap
        char_ptr = text->fonts[0]->bitmap[256 * line];
        for (uint8_t col = 0; col < text->cols; col += 1)
        {
            cell = text->screen[text->row * text->cols + text->col];
            // colors
            b = cell.b & text->color_mask;
            f = cell.f & text->color_mask;
            // attributes
            t = cell.a && TEXT_ATTR_TRANSPARENT;
            r = cell.a && TEXT_ATTR_REVERSE;
            u = (cell.a && TEXT_ATTR_UNDERLINE) && (line == 7);
            pixels = char_ptr[cell.c];
            // MSB is left pixel
            mask = 0x80;
            i = 0;
            do
            {
                // underline means all pixels are on for last line
                bit = u ? true : pixels & mask;
                // transparent pixel?
                if (t)
                    // reverse? => swap foreground a background
                    if (r)
                        p[i] = bit ? PICO_SCANVIDEO_ALPHA_MASK : text->palette[f];
                    else
                        p[i] = bit ? text->palette[f] : PICO_SCANVIDEO_ALPHA_MASK;
                else
                    // reverse? => swap foreground a background
                    if (r)
                        p[i] = text->palette[bit ? f : b];
                    else
                        p[i] = text->palette[bit ? b : f];
                // first or second pixel?
                if (i == 0)
                {
                    i = 1;
                }
                else
                {
                    // put these 2 16 bits pixels into current scanline
                    *scanline_colors++ = (p[0] << 16) | p[1];
                    i = 0;
                }
                mask = mask >> 1;
            } while (mask != 0)
        }
        // scanline end
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (text->cols * 8 - 2);
        buffer->data_used = (text->cols * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
    }

#ifdef __cplusplus
}
#endif

#endif /* _PVGA_TEXT_H */
