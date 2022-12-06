/* SPDX-License-Identifier: MIT-0 */

void example_8bpp()
{
    // uint16_t x, y, w, h;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);

    init_rects();
    // draw_borders_and_axis(&FULL_SCREEN, 0xff, 0x55, 0xaa);
    // title(&FULL_SCREEN, 0xff, 0xff, 0xff);
    draw_palette(&TOP_LEFT, 0xff, 0x80);
    specs(&TOP_RIGHT, 0x80 | rand() % 0x80, 0x80 | rand() % 0x80, 0x80 | rand() % 0x80);

    scroller_init(scroller);
    // scroller->y = HEIGHT / 2 - scroller->font_h;
    rect_t RECTS = { .x = BOTTOM_RIGHT.x, .y = BOTTOM_RIGHT.y, .w = BOTTOM_RIGHT.w, .h = BOTTOM_RIGHT.h - scroller->font_h - 1 };
    start_time();
    while (true)
    {
        wait_for_vblank();

        // draw_figures();
        draw_rects(&RECTS);
        scroller_draw(scroller);

        cycle_time(0, scroller->y - FONT5X7.h, 0xff);
    }
}

/* EOF */
