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

uint8_t rects_columns;
uint8_t rects_lines;
uint16_t rects_width;
uint16_t rects_height;

bool rects_init()
{
    rects_columns = demo_window.w % 10 == 0 ? 10 : 8;
    rects_lines = demo_window.h % 10 == 0 ? 10 : 8;
    rects_width = demo_window.w / rects_columns;
    rects_height = demo_window.h / rects_lines;
    return true;
}

/**
 * @brief Draw framed filled rects in a matrix
 */
void rects_draw()
{
    uint16_t x = rects_width * (rand() % rects_columns);
    uint16_t y = rects_height * (rand() % rects_lines);
    uint8_t c = 1 + (rand() % (COLORS - 1));
    hagl_fill_rectangle_xywh(hagl_backend, demo_window.x + x, demo_window.y + y, rects_width, rects_height, c);
    hagl_draw_rectangle_xywh(hagl_backend, demo_window.x + x, demo_window.y + y, rects_width, rects_height, ~c);
}

/* EOF */
