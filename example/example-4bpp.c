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
    draw_borders_and_axis(9, 11, 13);
    draw_title(15, 8, 7);
    draw_palette(15, 8, 8, 24, WIDTH < 320 ? 8 : 12, HEIGHT < 240 ? 8 : 12);
    draw_specs(13, 11, 9);

    x = 0;
    int16_t bars[16];
    int16_t dirs[16];
    for (uint8_t c = 1; c < 16; c++)
    {
        bars[c] = rand() % (WIDTH / 2 - 8);
        dirs[c] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 4);
    }
    //                            123456789012345
    hagl_put_text(hagl_backend, L"Foo Bar Baz #01", 4, HEIGHT / 2 + 2, 11, font5x7);
    hagl_draw_rectangle_xywh(hagl_backend, 4, HEIGHT / 2 + 11, WIDTH / 2 - 8, HEIGHT / 2 - 16, 9);
    hagl_put_text(hagl_backend, L"Foo Bar Baz #02", WIDTH / 2 + 4, HEIGHT / 2 + 2, 14, font5x7);
    hagl_draw_rectangle_xywh(hagl_backend, WIDTH / 2 + 4, HEIGHT / 2 + 11, WIDTH / 2 - 8, HEIGHT / 2 - 16, 9);

    start_time();
    while (true)
    {
        scanvideo_wait_for_vblank();

// if (false) {

        // Draw bars
        hagl_set_clip(hagl_backend, 4, HEIGHT / 2 + 10, 4 + WIDTH / 2 - 8 -1, HEIGHT / 2 + 10 + HEIGHT / 2 - 16 - 1);
        // x = 4;
        // y = HEIGHT / 2 + 8;
        // w = WIDTH / 2 - 8;
        // h = 5;
        // hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        // hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        for (uint8_t c = 1; c < 16; c++)
        {
            x = 4;
            h = 4;
            y = HEIGHT / 2 + 6 + (h + 1) * c;
            bars[c] += dirs[c];
            if (bars[c] < 0)
            {
                dirs[c] = 1 + rand() % 8;
                bars[c] = 0;
            }
            else
            {
                if (bars[c] > WIDTH / 2 - 8)
                {
                    dirs[c] = -(1 + rand() % 8);
                    bars[c] = WIDTH / 2 - 8;
                }
            }
            w = bars[c];
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            hagl_fill_rectangle_xywh(hagl_backend, x + w, y, WIDTH / 2 - 8 - w - 1, h, 0); // c == 15 ? 0 : 15);
        }
        hagl_set_clip(hagl_backend, 0, 0, WIDTH -1, HEIGHT - 1);

        // Draw lines
        x0 = WIDTH / 2 + 4;
        y0 = HEIGHT / 2 + 12;
        x1 = rand() % (WIDTH / 2 - 8);
        y1 = rand() % (HEIGHT / 2 - 8);
        w = rand() % WIDTH / 2 - 16;
        h = rand() % HEIGHT / 2 - 16;
        uint8_t c = 1 + rand() % 15;
        switch (rand() % 4)
        {
        case 0:
            hagl_draw_line(hagl_backend, x0 + x1, y0 + y1, x0 + w - 1, y0 + h - 1, c);            
            break;    
        case 1:
            hagl_draw_hline_xyw(hagl_backend, x0 + x1, y0 + y1, w, c);            
            break;    
        case 2:
            hagl_draw_vline_xyh(hagl_backend, x0 + x1, y0 + y1, h, c);            
            break;
        case 3:
            hagl_draw_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);            
            break;
        // case 4:
        //     hagl_fill_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);            
        //     break;
        }

// } // false

        cycle_time(9);
    }
}

/* EOF */
