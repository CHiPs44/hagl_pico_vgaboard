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
    wchar_t title[40];

    init_windows(FONT8X13B.h, FONT8X13B.h);
    draw_borders_and_axis(&FULL_SCREEN, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    // scroller_init(&SCROLLER, scroller);

    demo = 0;
    while (true)
    {
        clip(&FULL_SCREEN);
        // clip(&TITLE);
        hagl_fill_rectangle(hagl_backend, TITLE.x, TITLE.y, TITLE.w, TITLE.h, 0);
        swprintf(title, sizeof(title), L" %ls ", demos[demo].name);
        title_draw(&TITLE, title, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
        clock_t demo_end = get_time_ms() + demos[demo].duration_s * 1000;
        // clip(&DEMO);
        rect_copy(&DEMO, &window);
        hagl_fill_rectangle(hagl_backend, DEMO.x, DEMO.y, DEMO.w, DEMO.h, 0);
        rect_dump("WINDOW  ", &window);
        demos[demo].init();
        while (true)//get_time_ms() < demo_end)
        {
            wait_for_vblank();
            clip(&FULL_SCREEN);
            // clip(&DEMO);
            demos[demo].draw();
            // scroller_draw(scroller);
            cycle_time(0, HEIGHT - 8, COLORS - 1);
        }
        demo = (demo + 1) % NDEMOS;
    }
}

/*
void example_4bpp()
{
    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);

    init_windows(FONT8X13B.h, FONT8X13B.h);
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
    palette_draw(&TOP_LEFT, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    specs_draw(&TOP_RIGHT, 
        1 + rand() % (COLORS - 1), 
        1 + rand() % (COLORS - 1), 
        1 + rand() % (COLORS - 1)
    );
    // rect_t DEMO = { .x = 8, .y = 20, .w = WIDTH - 16, .h = HEIGHT - 40 };
    // font_draw(&DEMO);

    scroller_init(scroller);
    scroller->y = HEIGHT - scroller->font->h;
    // rect_t BARS  = { .x = 0        , .y = HEIGHT / 2, .w = WIDTH / 2, .h = HEIGHT / 2 - scroller->font->h - 1 };
    // bars_init(&BARS);
    // rect_t RECTS = { .x = WIDTH / 2, .y = HEIGHT / 2, .w = WIDTH / 2, .h = HEIGHT / 2 - scroller->font->h - 1 };
    start_time();
    while (true)
    {
        wait_for_vblank();
        // bars_draw();
        // hagl_set_clip(hagl_backend, 0, 0, WIDTH - 1, HEIGHT - 1);
        // figures_draw();
        // rects_draw(&RECTS);
        scroller_draw(scroller);
        cycle_time(0, HEIGHT / 2 - 8, COLORS - 1);
    }
}
*/

/* EOF */