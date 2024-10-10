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

/* PVTS stands for Pico VGA Text Screen */

#ifndef _PVTS_H
#define _PVTS_H

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

#define PVTS_FONT_NAME_LEN 32

    typedef struct s_pvts_font
    {
        // NOT A FONTX2 FONT, JUST A 256*8 BITMAP!
        uint8_t *bitmap;
        uint16_t size;     // must be 8×256=2048
        uint16_t codepage; // must be 437 (other 8-bit likek Windows-1252 or unicode/UTF-32/UTF-16 for later)
        uint8_t width;     // must be 8
        uint8_t height;    // must be 8
        uint8_t first;     // must be 0
        uint8_t last;      // must be 255
        uint8_t name[PVTS_FONT_NAME_LEN];
    } pvts_font;

    static pvts_font bios_f08_font = {
        .bitmap = &bios_f08,
        .size = bios_f08_len,
        .codepage = 437,
        .width = 8,
        .height = 8,
        .first = 0,
        .last = 255,
        .name = "[CP437] BIOS.F08"};

    typedef enum e_pvts_attributes
    {
        PVTS_TRANSPARENT = 0b10000000,
        PVTS_REVERSE = 0b01000000,
        PVTS_UNDERLINE = 0b00100000,
        PVTS_RESERVED = 0b00010000,
    } pvts_attributes;

#define PVTS_FONT_MASK 0b00000111
#define PVTS_FONT_COUNT 8

    /**
     * @brief Text cell for each character (32 bits / 4 bytes)
     *  => 3,072 bytes for 32x24 (256x192)
     *  => 4,000 bytes for 40x25 (320x200)
     *  => 4,800 bytes for 40x30 (320x240)
     *  => 8,000 bytes for 80x25 (640x200)
     *  => 9,600 bytes for 80x30 (640x240)
     */
    typedef struct s_pvts_cell
    {
        /** @brief character */
        uint8_t c;
        /** @brief background color */
        uint8_t b;
        /** @brief foreground color */
        uint8_t f;
        /** @brief attributes */
        uint8_t a;
    } pvts_cell;

    typedef enum e_pvts_cursor_anim
    {
        PVTS_CURSOR_ANIM_HIDDEN = 0,
        PVTS_CURSOR_ANIM_FIXED,
        PVTS_CURSOR_ANIM_FLASH_SLOW,
        PVTS_CURSOR_ANIM_FLASH_FAST,
    } pvts_cursor_anim;

    /** @brief Text screen state */
    typedef struct s_pvts_screen
    {
        pvts_cell **buffer;
        pvts_font *fonts[PVTS_FONT_COUNT];
        uint16_t *palette;
        uint8_t cols;
        uint8_t rows;
        uint8_t color_mask; // 0x0f for 16 colors, 0xff for 256 colors
        uint8_t background;
        uint8_t foreground;
        uint8_t attributes;
        bool auto_scroll; // automatic scrolling at end of screen
        // cursor
        uint8_t col;
        uint8_t row;
        pvts_cursor_anim anim;
        bool state;
        uint64_t timer;
    } pvts_screen;

    /** @brief Clear screen */
    static inline pvts_screen *pvts_clear(pvts_screen *screen)
    {
        pvts_cell cell = {
            .c = '\0',
            .a = PVTS_TRANSPARENT,
            .b = 0x00,
            .f = 0xff & screen->color_mask};
        uint16_t offset = 0;
        for (uint8_t row = 0; row <= screen->rows; row += 1)
        {
            for (uint8_t col = 0; col <= screen->cols; col += 1)
            {
                memcpy(screen->buffer[offset], &cell, sizeof(pvts_cell));
                offset += sizeof(pvts_cell);
            }
        }
    }

    /** @brief Reset screen to defaults */
    static inline pvts_screen *pvts_reset(pvts_screen *screen, uint8_t cols, uint8_t rows)
    {
        // default font at 0 and clear others
        screen->fonts[0] = &bios_f08_font;
        for (uint_t i = 1; i < PVTS_FONT_COUNT; i += 1)
            screen->fonts[i] = NULL;
        // default palette & colors
        pvts_set_palette(screen, palette_8bpp_default, 0xff);
        pvts_set_background(screen, 0x00);
        pvts_set_foreground(screen, 0xff);
        // default attributes
        screen->attributes = PVTS_TRANSPARENT;
        // reset cursor position & hide it
        screen->col = 0;
        screen->row = 0;
        screen->anim = PVTS_CURSOR_ANIM_HIDDEN;
        pvts_clear(screen);
    }

    /** @brief Allocate screen & screen buffer and set defaults */
    static inline pvts_screen *pvts_init(uint8_t cols, uint8_t rows)
    {
        pvts_screen *screen = calloc(1, sizeof(pvts_screen));
        if (screen == NULL)
            return NULL;
        screen->cols = cols;
        screen->rows = rows;
        screen->buffer = calloc(cols * rows, sizeof(pvts_cell));
        if (screen->buffer == NULL)
        {
            free(screen);
            return NULL;
        }
        pvts_reset(screen);
        return screen;
    }

    /** @brief Free screen & screen */
    static inline void pvts_done(pvts_screen *screen)
    {
        if (screen == NULL)
            return;
        if (screen->buffer != NULL)
            free(screen->buffer);
        free(screen);
    }

    /** @brief Set color palette & mask */
    static inline void pvts_set_palette(pvts_screen *screen, uint16_t *palette, uint8_t color_mask)
    {
        screen->palette = palette;
        screen->color_mask = color_mask;
    }

    /** @brief Set background color (paper) */
    static inline void pvts_set_background(pvts_screen *screen, uint8_t background)
    {
        screen->background = background & screen->color_mask;
    }

    /** @brief Set foreground clor (ink) */
    static inline void pvts_set_foreground(pvts_screen *screen, uint8_t foreground)
    {
        screen->foreground = foreground & screen->color_mask;
    };

    /** @brief Set character attributes */
    static inline void pvts_set_attributes(pvts_screen *screen, pvts_attributes attributes)
    {
        screen->attributes = attributes;
    };

    /** @brief Scroll up */
    static inline pvts_screen *pvts_scroll_up(pvts_screen *screen)
    {
        // copy line 1 to line 0, line 2 to line 1, and so on
        uint16_t size = screen->cols * sizeof(pvts_cell);
        uint16_t dst = screen->buffer;
        uint16_t src = dst + size;
        memcpy(dst, src, size * (screen->rows - 1));
        // fill last line with default cell
        pvts_cell cell = {
            .c = '\0',
            .a = PVTS_TRANSPARENT,
            .b = 0x00,
            .f = 0xff & screen->color_mask};
        uint16_t offset = size * (screen->rows - 1);
        for (uint8_t col = 0; col < screen->cols; col += 1)
        {
            memcpy(screen->buffer[offset], &cell, sizeof(pvts_cell));
            offset += sizeof(pvts_cell);
        }
    }

    /** @brief Scroll down */
    static inline pvts_screen *pvts_scroll_up(pvts_screen *screen)
    {
        // copy line 0 to line 1, line 1 to line 2, and so on
        // NB: go from bottom to top as data would be overwritten
        uint16_t size = screen->cols * sizeof(pvts_cell);
        uint16_t dst = screen->buffer + size * (screen->rows - 2);
        uint16_t src = dst - size;
        for (uint8_t row = 1; row <= screen->rows; row += 1)
        {
            memcpy(dst, src, size);
            dst -= size;
            src -= size;
        }
        // fill first line with default cell
        pvts_cell cell = {
            .c = '\0',
            .a = PVTS_TRANSPARENT,
            .b = 0x00,
            .f = 0xff & screen->color_mask};
        uint16_t offset = 0;
        for (uint8_t col = 0; col < screen->cols; col += 1)
        {
            memcpy(screen->buffer[offset], &cell, sizeof(pvts_cell));
            offset += sizeof(pvts_cell);
        }
    }

    /** @brief Move cursor. NB: row and col are 0 based, e.g. col is between 0 and 79 for 80 columns */
    static inline void pvts_move_cursor(pvts_screen *screen, uint8_t row, uint8_t col)
    {
        screen->col = col > screen->cols - 1 ? screen->cols - 1 : col;
        screen->row = row > screen->rows - 1 ? screen->rows - 1 : row;
    }

    /** @brief Put raw char into screen and advance cursor (no interpretation of TAB, CR, LF nor ESC sequences) */
    static inline void pvts_put_char(pvts_screen *screen, uint8_t c)
    {
        uint16_t offset = screen->row * screen->rows + screen->col;
        screen->buffer[offset]->c = c;
        screen->buffer[offset]->b = screen->background;
        screen->buffer[offset]->f = screen->foreground;
        screen->buffer[offset]->a = screen->attributes;
        screen->col += 1;
        if (screen->col >= screen->cols)
        {
            screen->col = 0;
            screen->row += 1;
            // for now, just wrap to top of screen, ignore screen->scroll setting
            if (screen->row >= screen->rows)
            {
                screen->row = 0;
            }
        }
    }

    /** @brief Put string of raw chars */
    static inline void pvts_put_string(pvts_screen *screen, uint8_t *s)
    {
        while (*s)
        {
            pvts_put_char(screen, *s++);
        }
    }

    /** @brief Render one line of screen chars */
    static inline void pvts_render_scanline(pvts_screen *screen, scanvideo_scanline_buffer_t *buffer)
    {
        uint32_t *scanline_colors = buffer->data;
        uint8_t line = scanvideo_scanline_number(buffer->scanline_id) % 8; // font height
        pvts_cell cell;
        uint8_t *font_line_ptr;
        uint8_t pixels;
        bool bit;
        uint8_t mask, i;
        uint32_t p[2];
        uint8_t b, f;
        bool t, r, u;
        // offset of line of chars in font bitmap
        font_line_ptr = screen->fonts[0]->bitmap[256 * line];
        for (uint8_t col = 0; col < screen->cols; col += 1)
        {
            cell = screen->buffer[screen->row * screen->cols + screen->col];
            // colors
            b = cell.b & screen->color_mask;
            f = cell.f & screen->color_mask;
            // attributes
            t = cell.a && PVTS_TRANSPARENT;
            r = cell.a && PVTS_REVERSE;
            // underline means all pixels are on for last line
            u = (cell.a && PVTS_UNDERLINE) && (line == 7);
            pixels = font_line_ptr[cell.c];
            // MSB is left pixel
            mask = 0b10000000;
            i = 0;
            do
            {
                bit = u ? true : pixels & mask;
                // transparent pixel?
                if (t)
                    // reverse? => swap foreground a background
                    if (r)
                        p[i] = bit ? PICO_SCANVIDEO_ALPHA_MASK : screen->palette[f];
                    else
                        p[i] = bit ? screen->palette[f] : PICO_SCANVIDEO_ALPHA_MASK;
                else
                    // reverse? => swap foreground a background
                    if (r)
                        p[i] = screen->palette[bit ? f : b];
                    else
                        p[i] = screen->palette[bit ? b : f];
                // first or second pixel?
                if (i == 0)
                {
                    // next pixel
                    i = 1;
                }
                else
                {
                    // put these 2 16 bits pixels into current scanline
                    *scanline_colors++ = (p[0] << 16) | p[1];
                    i = 0;
                }
                mask >>= 1;
            } while (mask != 0)
        }
        // scanline end
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (screen->cols * 8 - 2);
        buffer->data_used = (screen->cols * 8 + 4) / 2; // 2 16 bits pixels in each 32 bits word
    }

#ifdef __cplusplus
}
#endif

#endif /* _PVTS_H */
