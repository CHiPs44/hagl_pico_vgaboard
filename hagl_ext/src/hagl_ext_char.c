/*

MIT License

Copyright (c) 2018-2023 Mika Tuupola

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

This file is part of the HAGL graphics library:
https://github.com/tuupola/hagl


SPDX-License-Identifier: MIT

*/

#include "hagl/color.h"
#include "hagl/bitmap.h"
#include "hagl/blit.h"
#include "hagl.h"
#include "fontx.h"
#include "hagl_ext.h"
#include "hagl_ext_blit.h"
#include "hagl_ext_char.h"

uint8_t
hagl_ext_put_char(hagl_ext_surface_t *ext_surface, wchar_t code, int16_t x0, int16_t y0, const hagl_ext_char_style_t *style)
{
    static uint8_t *buffer = NULL;
    const hagl_surface_t *surface = ext_surface->_surface;
    uint8_t set, status;
    hagl_bitmap_t bitmap;
    fontx_glyph_t glyph;
    bool reverse = style->mode & HAGL_EXT_CHAR_MODE_REVERSE ? true : false;
    hagl_color_t foreground_color = reverse ? style->background_color : style->foreground_color;
    hagl_color_t background_color = reverse ? style->foreground_color : style->background_color;
    hagl_color_t transparent_color = foreground_color == 0 ? 1 : 0;

    status = fontx_glyph(&glyph, code, style->font);
    if (0 != status)
        return 0;
    /* Initialize character buffer when first called. */
    if (NULL == buffer)
        buffer = calloc(HAGL_CHAR_BUFFER_SIZE, sizeof(uint8_t));
    hagl_bitmap_init(&bitmap, glyph.width, glyph.height, surface->depth, (uint8_t *)buffer);
    hagl_color_t *ptr = (hagl_color_t *)bitmap.buffer;
    for (uint8_t y = 0; y < glyph.height; y++)
    {
        for (uint8_t x = 0; x < glyph.width; x++)
        {
            set = *(glyph.buffer + x / 8) & (0x80 >> (x % 8));
            if (style->mode & HAGL_EXT_CHAR_MODE_TRANSPARENT)
            {
                *ptr = set ? foreground_color : transparent_color;
            }
            else
            {
                *ptr = set ? foreground_color : background_color;
            }
            ptr++;
        }
        glyph.buffer += glyph.pitch;
    }

    if (style->scale_x_numerator <= 1 && style->scale_x_denominator <= 1 && style->scale_y_numerator <= 1 && style->scale_y_denominator <= 1)
    {
        if (style->mode & HAGL_EXT_CHAR_MODE_TRANSPARENT)
        {
            hagl_ext_blit_transparent(ext_surface, x0, y0, &bitmap, transparent_color);
        }
        else
        {
            hagl_blit(surface, x0, y0, &bitmap);
        }
        return bitmap.width;
    }
    else
    {
        if (style->mode & HAGL_EXT_CHAR_MODE_TRANSPARENT)
        {
            hagl_ext_blit_xywh_transparent(
                ext_surface,
                x0, y0,
                bitmap.width * style->scale_x_numerator / style->scale_x_denominator,
                bitmap.height * style->scale_y_numerator / style->scale_y_denominator,
                &bitmap,
                transparent_color);
        }
        else
        {
            hagl_blit_xywh(
                ext_surface,
                x0, y0,
                bitmap.width * style->scale_x_numerator / style->scale_x_denominator,
                bitmap.height * style->scale_y_numerator / style->scale_y_denominator,
                &bitmap);
        }
        return bitmap.width * style->scale_x_numerator / style->scale_x_denominator;
    }
}

uint16_t
hagl_ext_put_text(hagl_ext_surface_t *ext_surface, const wchar_t *str, int16_t x0, int16_t y0, const hagl_ext_char_style_t *style)
{
    wchar_t c;
    uint8_t status;
    fontx_meta_t meta;
    int16_t x = x0;
    int16_t y = y0;

    status = fontx_meta(&meta, style->font);
    if (0 != status)
    {
        return 0;
    }
    while (*str != 0)
    {
        c = *str++;
        if (13 == c || 10 == c)
        {
            x = x0;
            y += meta.height;
        }
        else
        {
            x += hagl_ext_put_char(ext_surface, c, x, y, style);
        }
    }
    /* NB: return 0 if text wrapped through CR or LF */
    return y == y0 ? x - x0 : 0;
}
