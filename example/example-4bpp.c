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

void example_4bpp()
{
    uint16_t x, y, w, h;
    uint16_t x0, y0, w0, h0, x1, y1;//, x2, y2;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);

    init_windows();
    draw_borders_and_axis(
        &FULL_SCREEN, 
        1 + rand() % (COLORS - 1), 
        1 + rand() % (COLORS - 1), 
        1 + rand() % (COLORS - 1)
    );
    // title_draw(&FULL_SCREEN, 
    //     1 + rand() % (COLORS - 1), 
    //     1 + rand() % (COLORS - 1), 
    //     1 + rand() % (COLORS - 1)
    // );
    // palette_draw(&TOP_LEFT, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    // specs_draw(&TOP_RIGHT, 
    //     1 + rand() % (COLORS - 1), 
    //     1 + rand() % (COLORS - 1), 
    //     1 + rand() % (COLORS - 1)
    // );
    rect_t DEMO = { .x = 8, .y = 20, .w = WIDTH - 16, .h = HEIGHT - 40 };
    font_draw(&DEMO);

    int16_t bars[15];
    int16_t dirs[15];
    color_t cols[15];
    for (uint8_t bar = 0; bar < 15; bar++)
    {
        bars[bar] = rand() % (WIDTH / 2);
        dirs[bar] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 8);
        cols[bar] = 1 + bar % (COLORS  - 1);
    }

    scroller_init(scroller);
    scroller->y = HEIGHT - scroller->font->h;
    rect_t RECTS = { .x = BOTTOM_RIGHT.x, .y = BOTTOM_RIGHT.y, .w = BOTTOM_RIGHT.w, .x = BOTTOM_RIGHT.x - scroller->font->h - 1 };
    start_time();
    while (true)
    {
        wait_for_vblank();
if (false) {
        // Draw bars
        x0 = 0;
        y0 = HEIGHT / 2;
        w0 = WIDTH / 2;
        h0 = HEIGHT / 2 - scroller->font->h;
        hagl_set_clip(hagl_backend, x0, y0, x0 + w0 -1, y0 + h0 - 1);
        for (uint8_t bar = 0; bar < 15; bar++)
        {
            x = x0;
            h = h0 / 15 - 1; //4;
            y = y0 + (h + 1) * bar;
            bars[bar] += dirs[bar];
            if (bars[bar] < x0)
            {
                dirs[bar] = 1 + rand() % 8;
                bars[bar] = x0 - dirs[bar];
            }
            else
            {
                if (bars[bar] > x0 + w0)
                {
                    dirs[bar] = -(1 + rand() % 8);
                    bars[bar] = x0 + w0 - dirs[bar];
                }
            }
            w = bars[bar];
            hagl_fill_rectangle_xywh(hagl_backend, x    , y, w     , h, cols[bar]);
            hagl_fill_rectangle_xywh(hagl_backend, x + w, y, w0 - w, h, 0        );
        }
        hagl_set_clip(hagl_backend, 0, 0, WIDTH - 1, HEIGHT - 1);
} // false
        // draw_figures();
        // rects_draw(&RECTS);
        scroller_draw(scroller);
        cycle_time(0, 0, COLORS - 1);
    }
}

/* EOF */
