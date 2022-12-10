/* SPDX-License-Identifier: MIT-0 */

void example_8bpp()
{
    // uint16_t x, y, w, h;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);

    init_windows();
    // draw_borders_and_axis(&FULL_SCREEN, 0xff, 0x55, 0xaa);
    // title_draw(&FULL_SCREEN, 0xff, 0xff, 0xff);
    palette_draw(&TOP_LEFT, 0xff, 0x80);
    specs_draw(&TOP_RIGHT, 0x80 | rand() % 0x80, 0x80 | rand() % 0x80, 0x80 | rand() % 0x80);

    scroller_init(scroller);
    // scroller->y = HEIGHT / 2 - scroller->font->h;
    rect_t RECTS = { 
        .x = BOTTOM_RIGHT.x, 
        .y = BOTTOM_RIGHT.y, 
        .w = BOTTOM_RIGHT.w, 
        .h = BOTTOM_RIGHT.h - scroller->font->h - 1
    };
    start_time();
    while (true)
    {
        wait_for_vblank();

        // draw_figures();
        rects_draw(&RECTS);
        scroller_draw(scroller);

        cycle_time(0, scroller->y - 8, 0xff);
    }
}

/* EOF */
