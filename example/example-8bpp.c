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

void example_8bpp()
{
    uint16_t x, y, w, h;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);
    // draw_borders_and_axis(0xff, 0x55, 0xaa);
    // draw_title(0xff, 0xff, 0xff);
    // 16x16 matrix in top left quarter
    draw_palette(
        0xff, 
        0x80, 
        0, //(WIDTH  / 2 - 16 * (w + 1)) / 2, 
        0, //(HEIGHT / 2 - 16 * (h + 1)) / 2 + 4, 
        WIDTH  / 2 / 16 - 1, 
        HEIGHT / 2 / 16 - 1
    );
    draw_specs(0xaa, 0xff, 0x55);

    init_scroller();
    start_time();
    while (true)
    {
        scanvideo_wait_for_vblank();

        // sleep_ms(1000);
        /*
         * Palette swap test
         * TODO /!\ gets darker and darker with time /!\
         * Beware of double palette!
         */
        // uint8_t i = rand() % 256;
        // uint8_t j = rand() % 256;
        // uint16_t tmp = vgaboard->palette[i];
        // vgaboard->palette[i] = vgaboard->palette[j];
        // vgaboard->palette[j] = tmp;

        // draw_figures();
        draw_rects(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);
        draw_scroller();

        cycle_time(0xff);
    }
}

/* EOF */
