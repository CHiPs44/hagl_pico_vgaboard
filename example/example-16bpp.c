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

void example_16bpp()
{
    // uint16_t x, y, w, h;
    // uint16_t x0, y0, x1, y1, x2, y2;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);

    init_windows(FONT8X13B.h, FONT8X13B.h);
    draw_borders_and_axis(&FULL_SCREEN, BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);
    title_draw(&FULL_SCREEN, L"Pico VGA HAGL", BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);
    palette_draw(&TOP_LEFT, BGAR5515_WHITE, BGAR5515_WHITE);
    specs_draw(&TOP_RIGHT, BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);

    start_time();
    while (true)
    {
        scanvideo_wait_for_vblank();
        // TODO!
        cycle_time(0, 0, BGAR5515_WHITE);
    }
}

/* EOF */
