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

bool rects_init()
{
    // Nothing!
    return true;
}

/**
 * @brief Draw framed filled rects in a matrix 
 */
void rects_draw()
{
    uint8_t  columns = 8;
    uint8_t  lines   = window.h % 10 == 0 ? 10 : 8;
    uint16_t w       = window.w / columns;
    uint16_t h       = window.h / lines;
    uint16_t x       = w * (get_rand_32() % columns);
    uint16_t y       = h * (get_rand_32() % lines  );
    uint8_t  c       = 1 + (get_rand_32() % (COLORS  - 1));
    hagl_fill_rectangle_xywh(hagl_backend, window.x + x, window.y + y, w, h,  c);
    hagl_draw_rectangle_xywh(hagl_backend, window.x + x, window.y + y, w, h, ~c);
}

/* EOF */
