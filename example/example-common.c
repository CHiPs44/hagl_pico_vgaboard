/*

MIT No Attribution

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

#define WIDTH       (hagl_backend->width)
#define HEIGHT      (hagl_backend->height)
#define DEPTH       (hagl_backend->depth)
#define COLORS      (vgaboard->colors)
#define FREQ_HZ     (vgaboard->freq_hz)
#define FRAMEBUFFER (vgaboard->framebuffer)

typedef struct {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} rect_t;

rect_t FULL_SCREEN;
rect_t TOP_LEFT;
rect_t TOP_RIGHT;
rect_t BOTTOM_LEFT;
rect_t BOTTOM_RIGHT;

void dump_rect(rect_t *rect)
{
    printf("RECT %p: x=%d, y=%d, w=%d, h=%d\r\n", rect, rect->x, rect->y, rect->w, rect->h);
}

void init_rect(rect_t *rect, int16_t x, int16_t y, int16_t w, int16_t h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    // dump_rect(rect);
}

void init_rects()
{
    init_rect(&FULL_SCREEN , 0        , 0         , WIDTH    , HEIGHT    );
    init_rect(&TOP_LEFT    , 0        , 0         , WIDTH / 2, HEIGHT / 2);
    init_rect(&TOP_RIGHT   , WIDTH / 2, 0         , WIDTH / 2, HEIGHT / 2);
    init_rect(&BOTTOM_LEFT , 0        , HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
    init_rect(&BOTTOM_RIGHT, WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
}

void clip(rect_t *rect)
{
    hagl_set_clip(hagl_backend, rect->x, rect->y, rect->x + rect->w - 1, rect->y + rect->h - 1);
}

typedef struct {
    const uint8_t *fontx;
    const uint8_t w;
    const uint8_t h;
} font_t;

font_t BIOS_F08  = { .fontx = BIOS_F08_fnt, 8,  8 };
font_t FONT5X7   = { .fontx = font5x7     , 5,  7 };
font_t FONT8X13  = { .fontx = font8x13    , 8, 13 };
font_t FONT8X13B = { .fontx = font8x13B   , 8, 13 };

font_t *get_small_font(rect_t *window)
{
    return window->w <= 160 ? &FONT5X7 : &BIOS_F08;
}

font_t *get_text_font(rect_t *window)
{
    return window->w <= 80 ? &BIOS_F08 : &FONT8X13;
}

font_t *get_title_font(rect_t *window)
{
    return window->w <= 80 ? &BIOS_F08 : &FONT8X13B;
}

void draw_figures()
{
    uint16_t x0 = WIDTH / 2;
    uint16_t y0 = HEIGHT / 2;
    hagl_set_clip(hagl_backend, x0, y0, x0 + WIDTH / 2 - 1, y0 + HEIGHT / 2 - 1);
    uint16_t x1 = rand() % (WIDTH / 2);
    uint16_t y1 = rand() % (HEIGHT / 2);
    uint16_t w  = rand() % (WIDTH / 2);
    uint16_t h  = rand() % (HEIGHT / 2);
    uint8_t  c  = 1 + rand() % (COLORS  - 1);
    switch (rand() % 5)
    {
        case 0:
            hagl_draw_line(hagl_backend, x0 + x1, y0 + y1, x0 + w - 1, y0 + h - 1, c);            
            break;    
        case 1:
            hagl_draw_hline_xyw(hagl_backend, x0 + x1, y0 + y1, w, c);
            break;    
        case 2:
            hagl_draw_vline_xyh(hagl_backend, x0 + x1, y0 + y1, h, c);
            break;
        case 3:
            hagl_draw_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);
            break;
        case 4:
            hagl_fill_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);
            break;
    }
    hagl_set_clip(hagl_backend, 0, 0, WIDTH - 1, HEIGHT - 1);
}

void draw_rects(rect_t *window)
{
    uint8_t columns = 8;
    uint8_t lines   = window->h % 10 == 0 ? 10 : 8;
    uint16_t w = window->w / columns;
    uint16_t h = window->h / lines;
    uint16_t x1 = w * (rand() % columns);
    uint16_t y1 = h * (rand() % lines  );
    uint8_t  c  = 1 + rand() % (COLORS  - 1);
    hagl_fill_rectangle_xywh(hagl_backend, window->x + x1, window->y + y1, w, h, c);
    hagl_draw_rectangle_xywh(hagl_backend, window->x + x1, window->y + y1, w, h, ~c);
}

/* EOF */
