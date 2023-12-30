/*

MIT No Attribution

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#include "hagl.h"
#include "hagl_hal.h"
#include "pico-vgaboard.h"
#include <string.h>
#include <wchar.h>

extern void specs_calc(bool for_scroller);
extern wchar_t *specs_scroller;

#define SCROLLER_DEBUG 0
// #define SCROLLER_DEBUG 1

typedef struct _star_t
{
    int16_t x;
    int16_t y;
    int16_t dx;
    hagl_color_t color;
} star_t;

#define NSTARS 42
star_t stars[NSTARS];
uint32_t stars_counter;

void stars_init(int16_t y1, uint16_t h1, int16_t y2, uint16_t h2, int16_t y3, uint16_t h3)
{
    stars_counter = 0;
    int16_t y;
    for (int i = 0; i < NSTARS; i++)
    {
        stars[i].x = WIDTH * 3 / 4 + rand() % (WIDTH / 4);
        do
        {
            y = rand() % HEIGHT;
        } while ((y >= y1 && y <= y1 + h1) || (y >= y2 && y <= y2 + h2) || (y >= y3 && y <= y3 + h3));
        stars[i].y = y;
        stars[i].dx = -(1 + rand() % 3);
        stars[i].color = 1 + rand() % (COLORS - 1);
    }
}

void stars_draw()
{
    stars_counter += 1;
    for (int i = 0; i < NSTARS; i++)
    {
        hagl_put_pixel(hagl_backend, stars[i].x, stars[i].y, 0);
        stars[i].x += stars[i].dx;
        if (stars[i].x < 0)
        {
            stars[i].x = WIDTH * 3 / 4 + rand() % (WIDTH / 4);
            stars[i].dx = -(1 + rand() % 3);
        }
        if (rand() % 10 == 0)
        {
            stars[i].color = 1 + rand() % (COLORS - 1);
        }
        hagl_put_pixel(hagl_backend, stars[i].x, stars[i].y, stars[i].color);
    }
}

typedef struct _scroller_t
{
    wchar_t *text;
    uint16_t length;
    uint16_t index;
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    font_t *font;
    /** @brief pixel (column) from 0 to font width - 1 in the current char */
    int8_t pixel;
    /** @brief speed in bytes, *not* in pixels */
    uint8_t speed_in_bytes;
    /** @brief animate every modulo frame */
    uint8_t modulo;
    /** @brief text color */
    hagl_color_t color;
    /** @brief delta y */
    int16_t dy;
    /** @brief y offset */
    int16_t y_offset;
    /** @brief frame counter */
    uint32_t frame;
} scroller_t;

scroller_t _scroller1;
scroller_t *s1 = &_scroller1;
scroller_t _scroller2;
scroller_t *s2 = &_scroller2;
scroller_t _scroller3;
scroller_t *s3 = &_scroller3;

hagl_color_t scroller_get_color()
{
    if (DEPTH == 1)
        return 1;
    if (DEPTH == 2)
        return 1 + (rand() % (COLORS - 1));
    if (DEPTH == 16)
    {
        uint32_t rand_32 = rand();
        return PICO_SCANVIDEO_PIXEL_FROM_RGB8(
            0x80 | (rand_32 >> 0) & 0x7f,
            0x80 | (rand_32 >> 8) & 0x7f,
            0x80 | (rand_32 >> 16) & 0x7f);
    }
    // 16 & 256 colors
    hagl_color_t index;
    uint8_t tries = 0;
    do
    {
        index = 1 + (rand() % COLORS - 1);
        BGAR5515 rgb = pico_vgaboard_get_palette_color(index);
        uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb);
        uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb);
        uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb);
        if (r >= 0x10 && g >= 0x10 && b >= 0x10)
            return index;
        tries += 1;
        if (tries > 9)
            return index;
    } while (true);
}

void scroller_init_one(scroller_t *s, int index)
{
    /* Parameters */
    s->font = HEIGHT <= 200 ? &FONT8X8 : &FONT8X13B;
    if (index == 1)
    {
        /* Some text in english and french, somewhat in the tone of 80's demos */
        s->text =
            /*
            L"0123456789012345678901234567890123456789";
            */
            L"                                        "
            L"Yo lamers!!!                            "
            L"This is CHiPs44 speaking through the awesome VGA demo board for the mighty Raspberry Pi Pico and the magnificent HAGL library...  "
            L"Source code is available at https://github.com/CHiPs44/hagl_pico_vgaboard/ under MIT/BSD license...                               "
            L"Thanks to Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, lurk101, Rumbledethumps & Pimoroni's team!               "
            L"                                        ";
        s->y = HEIGHT * 1 / 4 - s->font->h;
        s->dy = 1;
    }
    else if (index == 2)
    {
        s->text =
            /*
            L"0123456789012345678901234567890123456789";
            */
            L"                                        "
            L"Salut bande de nazes !!!                "
            L"C'est CHiPs44 qui parle depuis la super carte de démo VGA pour le génial Raspberry Pi Pico et la magnifique bibliothèque HAGL...  "
            L"Le code source est disponible à l'URL https://github.com/CHiPs44/hagl_pico_vgaboard/ sous licence MIT/BSD...                      "
            L"Merci à Tuupola, Kilograham, Memotech Bill, DarkElvenAngel, HermannSW, lurk101, Rumbledethumps et à l'équipe de Pimoroni !        "
            L"                                        ";
        s->y = HEIGHT * 3 / 4 - s->font->h;
        s->dy = -1;
    }
    else
    {
        s->text = specs_scroller;
        s->y = HEIGHT * 1 / 2 - s->font->h;
        s->dy = 0;
    }
    s->length = wcslen(s->text);
    s->x = 0;
    s->w = WIDTH;
    s->h = 2 * s->font->h;
    s->speed_in_bytes = 1;
    s->modulo = 1;
    /* Variables */
    s->index = 0;
    s->pixel = 0;
    s->color = scroller_get_color();
    s->frame = 0;
    s->y_offset = 0;
}

void scroller_draw_one(scroller_t *s)
{
    // Pixels per byte
    uint8_t pixels_per_byte;
    switch (DEPTH)
    {
    case 1: // 2 colors
        pixels_per_byte = 8;
        break;
    case 2: // 4 colors
        pixels_per_byte = 4;
        break;
    case 4: // 16 colors
        pixels_per_byte = 2;
        break;
    case 8: // 256 colors
        pixels_per_byte = 1;
        break;
    default: // Error
        return;
        break;
    }
    uint8_t speed_in_pixels = s->speed_in_bytes * pixels_per_byte;
    // Bytes per line
    uint16_t bytes_per_line = s->w / pixels_per_byte;
    uint16_t offset;
    uint8_t *destination;
    uint8_t *source;
    size_t size;
    wchar_t scroller_text[40];
    wchar_t c = s->text[s->index];
#if SCROLLER_DEBUG
    hagl_put_text(
        hagl_backend,
        L"0123456789012345678901234567890123456789",
        0,
        s->y - s->h * 2,
        0xff,
        s->font->fontx);
    // if (scroller->index > 60 && scroller->pixel==0) return;
    // hagl_draw_hline(hagl_backend, s->x, s->y - 1       , s->w, COLORS - 1);
    // hagl_draw_hline(hagl_backend, s->x, s->y + s->h / 2, s->w, COLORS - 1);
    // hagl_draw_hline(hagl_backend, s->x, s->y + s->h    , s->w, COLORS - 1);
#endif
    if (frame_counter % s->modulo == 0)
    {
        // Move text "speed_in_bytes" byte(s) left, 1 pixel in 8bpp, 2 pixels in 4bbp, 4 pixels in 2bbp, 8 pixels in 1bpp
        // TODO take x and w into account instead of 0 and WIDTH
        for (uint16_t y = s->y; y < s->y + s->h; y += 1)
        {
            // Start of line in framebuffer
            destination = pico_vgaboard->framebuffer + y * bytes_per_line;
            source = destination + s->speed_in_bytes;
            // size = bytes_per_line - 1;
            // memcpy(destination, source, size);
            // Seems memcpy does not copy in the right direction...
            for (size_t byte = 0; byte < bytes_per_line /*- s->speed_in_bytes*/; byte += 1)
            {
                *destination++ = *source++;
            }
        }
#if SCROLLER_DEBUG
        // hagl_draw_vline_xyh(hagl_backend, s->x + 16, s->y, s->h, COLORS - 1);
        swprintf(text, sizeof(text) / sizeof(wchar_t),
                 L"px/b: %d sb: %d sp: %d pix: %d dy: %d   ",
                 pixels_per_byte, s->speed_in_bytes, speed_in_pixels, s->pixel, s->dy
                 // L"dy: %d yoff: %d      ",
                 // s->dy, s->y_offset
        );
        hagl_put_text(hagl_backend, text, 0, s->y - s->font->h, COLORS - 1, s->font->fontx);
        hagl_put_char(hagl_backend, c,
                      s->x + s->w - s->font->w,
                      s->y - s->font->h,
                      COLORS - 1, s->font->fontx);
#endif
        hagl_fill_rectangle_xywh(hagl_backend,
                                 s->x + s->w - 1 - speed_in_pixels - s->pixel,
                                 s->y,
                                 s->font->w,
                                 s->h,
                                 0);
        hagl_put_char(hagl_backend, c,
                      s->x + s->w - 1 - speed_in_pixels - s->pixel,
                      s->y + s->h / 4 + s->y_offset,
                      s->color, s->font->fontx);
        // Increment visible char area
        s->pixel += speed_in_pixels;
        if (s->pixel >= s->font->w)
        {
            // Reset visible char area
            s->pixel = 0;
            // Next char
            s->index += 1;
            // Text wrap?
            if (s->index >= s->length)
            {
                s->index = 0;
            }
            // Change color between words
            if (s->text[s->index] == L' ')
            {
                s->color = scroller_get_color();
            }
        }
        s->frame += 1;
        if (s->dy != 0)
        {
            if (s->frame % (1 * s->font->w) == 0)
            {
                s->y_offset += s->dy;
                if (abs(s->y_offset) > s->h / 4)
                {
                    s->dy = -s->dy;
                }
            }
        }
    }
}

bool scroller_init()
{
    // hagl_fill_rectangle_xywh(hagl_backend, 0, 0, WIDTH, HEIGHT, 0);
    specs_calc(true);
    scroller_init_one(s1, 1);
    scroller_init_one(s2, 2);
    scroller_init_one(s3, 3);
    stars_init(s1->y, s1->h, s2->y, s2->h, s3->y, s3->h);
    // stars_init(-1, 0, -1, 0, -1, 0);
    // hagl_draw_rectangle_xywh(hagl_backend, DEMO.x, DEMO.y, DEMO.w, DEMO.h, COLORS - 1);
    return true;
}

void scroller_draw()
{
    stars_draw();
    scroller_draw_one(s1);
    scroller_draw_one(s2);
    scroller_draw_one(s3);
}

// EOF
