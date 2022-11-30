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
    uint16_t x0, y0, x1, y1;//, x2, y2;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);
    // draw_borders_and_axis(2, 3, 4);
    // draw_title(14, 8, 7);
    draw_palette(6, 4, 0, 0, WIDTH < 320 ? 8 : 12, HEIGHT < 240 ? 8 : 12);
    draw_specs(13, 10, 9);

    int16_t bars[16];
    int16_t dirs[16];
    for (uint8_t c = 1; c < 16; c++)
    {
        bars[c] = rand() % (WIDTH / 2);
        dirs[c] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 8);
    }
    //                            123456789012345
    // hagl_put_text(hagl_backend, L"Foo Bar Baz #01", 4, HEIGHT / 2 + 2, 11, font5x7);
    // hagl_draw_rectangle_xywh(hagl_backend, 4, HEIGHT / 2 + 11, WIDTH / 2 - 8, HEIGHT / 2 - 16, 9);
    // hagl_put_text(hagl_backend, L"Foo Bar Baz #02", WIDTH / 2 + 4, HEIGHT / 2 + 2, 14, font5x7);
    // hagl_draw_rectangle_xywh(hagl_backend, WIDTH / 2 + 4, HEIGHT / 2 + 11, WIDTH / 2 - 8, HEIGHT / 2 - 16, 9);

    scroller_init(scroller);
    start_time();
    while (true)
    {
        startTime2 = get_time();
        scanvideo_wait_for_vblank();
        endTime2 = get_time();
        elapsedTime2 = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
// if (false) {
        // Draw bars
        hagl_set_clip(hagl_backend, 0, HEIGHT / 2, WIDTH / 2 - 1, HEIGHT - 1);
        // x = 4;
        // y = HEIGHT / 2 + 8;
        // w = WIDTH / 2 - 8;
        // h = 5;
        // hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        // hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        for (uint8_t c = 1; c < 16; c++)
        {
            x = 0;
            h = HEIGHT / 2 / 16 - 1; //4;
            y = HEIGHT / 2 + (h + 1) * (c - 1);
            bars[c] += dirs[c];
            if (bars[c] < 0)
            {
                dirs[c] = 1 + rand() % 8;
                bars[c] = 0;
            }
            else
            {
                if (bars[c] > WIDTH / 2)
                {
                    dirs[c] = -(1 + rand() % 8);
                    bars[c] = WIDTH / 2;
                }
            }
            w = bars[c];
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            hagl_fill_rectangle_xywh(hagl_backend, x + w, y, WIDTH / 2 - w - 1, h, 0);
        }
        hagl_set_clip(hagl_backend, 0, 0, WIDTH - 1, HEIGHT - 1);
// } // false
        // draw_figures();
        draw_rects(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        scroller_draw(scroller);
        cycle_time(9);
    }
}

/* EOF */
