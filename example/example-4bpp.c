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

    printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, vgaboard->freq_hz);

    // init_windows(FONT8X13B.h, FONT8X13B.h);
    init_windows(FONT8X8.h * 2, FONT8X8.h * 3 / 2);
    // draw_borders_and_axis(&FULL_SCREEN, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    scroller_init(scroller);
    scroller->font = &FONT8X8;

    rect_copy(&DEMO, &window);
    demo = 0;
    while (true)
    {
        wprintf(L"Lauching #%d: %ls\r\n", demo, demos[demo].name);
        scroller->color = scroller_get_color();
        /**********************************************************************/
        clip(&TITLE);
        hagl_fill_rectangle_xywh(hagl_backend, TITLE.x, TITLE.y, TITLE.w, TITLE.h, 1 + rand() % (COLORS - 1));
        swprintf(title, sizeof(title), L" %d/%d %ls ", demo + 1, NDEMOS, demos[demo].name);
        title_draw(&TITLE, title);
        /**********************************************************************/
        clip(&DEMO);
        hagl_fill_rectangle_xywh(hagl_backend, DEMO.x, DEMO.y, DEMO.w, DEMO.h, 1 + rand() % (COLORS - 1));
        // rect_dump("WINDOW  ", &window);
        demos[demo].init();
        clock_t demo_end = get_time_ms() + demos[demo].duration_s * 1000;
        /**********************************************************************/
        while (get_time_ms() < demo_end)
        {
            wait_for_vblank();
            clip(&DEMO);
            demos[demo].draw();
            // scroller_draw(scroller);
            clip(&FULL_SCREEN);
            //    1234567890123456789012345678901234567890
            hagl_put_text(
                hagl_backend, 
                L"____There should be a scroller here!____", 
                SCROLLER.x, SCROLLER.y, 
                scroller->color, scroller->font->fontx
            );
            cycle_time(0, SCROLLER.y - FONT5X7.h - 2, COLORS - 1);
        }
        /**********************************************************************/
        demo = (demo + 1) % NDEMOS;
    }
}

/* EOF */
