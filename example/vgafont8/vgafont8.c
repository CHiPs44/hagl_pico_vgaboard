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

#include <hagl.h>

static hagl_backend_t *vgafont8_hagl = NULL;
static uint8_t *vgafont8_font_bitmap = NULL;
static hagl_color_t vgafont8_background_color = 0;
static hagl_color_t vgafont8_foreground_color = 1;

// Configuration
void vgafont8_set_hagl_backend(hagl_backend_t *hagl_backend)
{
    vgafont8_hagl = hagl_backend;
}

void vgafont8_set_font_bitmap(const uint8_t *font)
{
    vgafont8_font_bitmap = (uint8_t *)font;
}

void vgafont8_set_background_color(hagl_color_t background_color)
{
    vgafont8_background_color = background_color;
}

void vgafont8_set_foreground_color(hagl_color_t foreground_color)
{
    vgafont8_foreground_color = foreground_color;
}

// Display single char or string
void vgafont8_put_char(uint16_t x, uint16_t y, uint8_t c)
{
    uint8_t *glyph = &vgafont8_font_bitmap[8 * c];
    for (uint8_t i = 0; i < 8; i += 1)
    {
        uint8_t mask = 128;
        for (uint8_t j = 0; j < 8; j += 1)
        {
            bool bit = (glyph[i] & mask) != 0;
            hagl_color_t color = bit ? vgafont8_foreground_color : vgafont8_background_color;
            hagl_put_pixel(vgafont8_hagl, x + j, y + i, color);
            mask /= 2;
        }
    }
}

void vgafont8_put_text(uint16_t x, uint16_t y, const uint8_t *s)
{
    while (*s)
    {
        vgafont8_put_char(x, y, *s++);
        x += 8;
    }
}

// // Retrieve single char or text string an an HAGL bitmap
// void vgafont8_get_char(uint8_t c, hagl_bitmap *bitmap)
// {
// }

// void vgafont8_get_text(uint8_t *s, hagl_bitmap *bitmap)
// {
// }

// EOF
