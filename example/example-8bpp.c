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

    printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, FREQ_HZ);
    draw_borders_and_axis(9, 11, 13);
    draw_title(0xff, 0xff, 0xff);
    draw_palette(0xff, 8, 24, WIDTH < 320 ? 8 : 12, HEIGHT < 240 ? 8 : 12);

    start_time();
    while (true)
    {
        scanvideo_wait_for_vblank();

        /*
         * Palette swap test
         * TODO /!\ gets darker and darker with time /!\
         */
        // uint8_t i = rand() % 256;
        // uint8_t j = rand() % 256;
        // uint16_t tmp = vgaboard->palette[i];
        // vgaboard->palette[i] = vgaboard->palette[j];
        // vgaboard->palette[j] = tmp;

        // /*
        //  * Random lines
        //  */
        // x = (width / 2) + rand() % (width / 2);
        // y = 32 + rand() % (height - 32);
        // w = rand() % (width/2);
        // h = rand() % (height/2);
        // hagl_draw_line(hagl_backend, x, y, x + w - 1, y + h - 1, counter % colors);
        // // hagl_draw_hline_xyw(hagl_backend, x, y, w, counter % colors);
        // // hagl_draw_vline_xyh(hagl_backend, x, y, h, colors - counter % (colors-1));

        cycle_time(0xff);
    }
}

/* EOF */
