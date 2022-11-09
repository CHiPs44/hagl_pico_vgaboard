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

#include "sprites-4bpp.h"

void example_4bpp()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t depth = hagl_backend->depth;
    const uint16_t colors = vgaboard->colors;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;

    wchar_t text[80];
    uint16_t x0, y0, x1, y1, x2, y2;
    uint16_t x, y, w, h;
    int8_t dx = 1;
    wchar_t demo[80];

    printf("*** EXAMPLE_%dX%dX%d ***\n", width, height, depth);

    hagl_set_clip(hagl_backend, 0, 0, width - 1, height - 1);

    /* Borders */
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, width, height, 9);
    // Missing pixel at bottom right corner (HAGL bug?)
    hagl_put_pixel(hagl_backend, vgaboard->width - 1, vgaboard->height - 1, 9);
    // X axis
    hagl_draw_hline(hagl_backend, 0, half_height - 1, vgaboard->width - 1, 9);
    // Y axis
    hagl_draw_vline(hagl_backend, half_width - 1, 0, vgaboard->height - 1, 9);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        // 1234657890123465789012346578901234657890
        L"HAGL HAL for Raspberry Pi Pico VGA");
    // L"Raspberry Pi Pico VGA board: HAGL HAL");
    // wprintf(L"demo2: ls[%ls] %d\n", &demo, wcslen(demo));
    w = wcslen(demo) * 8;
    h = 13;
    x = half_width - w / 2;
    y = 8;
    hagl_draw_rounded_rectangle_xywh(hagl_backend, x - 4, y - 2, w + 8, h + 4, 3, 3);
    hagl_put_text(hagl_backend, demo, x, y, 11, font8x13B);

    // /* Draw palette - large */
    // x = 8;
    // y += 32;
    // for (uint8_t c = 0; c < 16; c++)
    // {
    //     /* Framed tile + value for each color in the palette */
    //     x0 = 8 + (c / 4) * (width / 4);
    //     y0 = y + (c % 4) * 16;
    //     x1 = x0 + 14;
    //     y1 = y0 + 14;
    //     hagl_fill_rounded_rectangle(hagl_backend, x0, y0, x1, y1, 3, c);
    //     hagl_draw_rounded_rectangle(hagl_backend, x0, y0, x1, y1, 3, c == 15 ? 8 : 15);
    //     swprintf(text, sizeof(text), L"%02d %04X", c, vgaboard_get_palette_color(c));
    //     hagl_put_text(hagl_backend, text, x0 + 20, y0 + 4, 15, font5x7);
    //     // wprintf(L"text: %ls\n", &text);
    // }

    /* Draw palette - small */
    x = 4;
    y += 20;
    for (uint8_t c = 0; c < 16; c++)
    {
        /* Framed tile + value for each color in the palette */
        x0 = x + (c / 8) * (half_width / 2);
        y0 = y + (c % 8) * 11;
        x1 = x0 + 9;
        y1 = y0 + 9;
        hagl_fill_rectangle(hagl_backend, x0, y0, x1, y1, c);
        hagl_draw_rectangle(hagl_backend, x0, y0, x1, y1, c == 15 ? 8 : 15);
        swprintf(text, sizeof(text), L"%02d %04X", c, vgaboard_get_palette_color(c));
        hagl_put_text(hagl_backend, text, x0 + 12, y0 + 2, 15, font5x7);
        // wprintf(L"text: %ls\n", &text);
    }

    /* Draw tiles */
    x = half_width;
    for (uint8_t column = 0; column < 2 /*half_width / 8*/; column += 1)
    {
        for (uint8_t line = 0; line < 2 /*(half_height - y) / 8*/; line += 1)
        {
            // hagl_blit_xy(hagl_backend, x + column * 8, y + line * 8, &tile_8x8x4_1);
            hagl_blit_xywh(hagl_backend, x + column * 8, y + line * 8, 8 * 4, 8 * 4, &tile_8x8x4_1);
        }
    }

    uint8_t buffer[8 * 13 * sizeof(color_t)];
    hagl_bitmap_t bitmap;
    bitmap.size = sizeof(buffer);
    bitmap.buffer = buffer;
    uint8_t glyph = hagl_get_glyph(hagl_backend, L'W', 13, &bitmap, font8x13B);
    printf("glyph   %d\n", glyph);
    printf("width   %d\n", bitmap.width);
    printf("height  %d\n", bitmap.height);
    printf("depth   %d\n", bitmap.depth);
    printf("pitch   %d\n", bitmap.pitch);
    printf("size    %d\n", bitmap.size);
    printf("buffer! %d\n", sizeof(buffer));
    for (int i = 0; i < sizeof(buffer); i += 1)
    {
        printf("%04x ", buffer[i]);
        if (i % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    hagl_blit_xywh(hagl_backend, half_width * 3 / 2, y, 8 * 4, 13 * 4, &bitmap);

    uint32_t counter = 0;
    int led = 0;
    x = 0;
    int16_t bars[16];
    int16_t dirs[16];
    for (uint8_t c = 1; c < 16; c++)
    {
        bars[c] = rand() % (half_width - 8);
        dirs[c] = (rand() % 2 == 0 ? 1 : -1) * (1 + rand() % 4);
    }
    //                            123456789012345
    hagl_put_text(hagl_backend, L"Foo Bar Baz #01", 4, half_height + 2, 11, font5x7);
    hagl_draw_rectangle_xywh(hagl_backend, 4, half_height + 11, half_width - 8, half_height - 16, 9);
    hagl_put_text(hagl_backend, L"Foo Bar Baz #02", half_width + 4, half_height + 2, 14, font5x7);
    hagl_draw_rectangle_xywh(hagl_backend, half_width + 4, half_height + 11, half_width - 8, half_height - 16, 9);
    while (true)
    {
        // Draw bars
        // x = 4;
        // y = half_height + 8;
        // w = half_width - 8;
        // h = 5;
        // hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        // hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, 15);
        for (uint8_t c = 1; c < 16; c++)
        {
            x = 4;
            h = 5;
            y = half_height + 6 + (h + 2) * c;
            // w = (counter + c * 16) % (half_width - 8);
            bars[c] += dirs[c];
            if (bars[c] < 0)
            {
                dirs[c] = 1 + rand() % 4;
                bars[c] = 0;
            }
            else
            {
                if (bars[c] > half_width - 8)
                {
                    dirs[c] = -(1 + rand() % 4);
                    bars[c] = half_width - 8;
                }
            }
            w = bars[c];
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            hagl_fill_rectangle_xywh(hagl_backend, x + w, y, half_width - 8 - w - 1, h, 0); // c == 15 ? 0 : 15);
        }

        // Draw lines
        x0 = half_width + 4;
        y0 = half_height + 12;
        x1 = rand() % (half_width - 8);
        y1 = rand() % (half_height - 8);
        w = rand() % half_width - 16;
        h = rand() % half_height - 16;
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

        // // Draw text
        // swprintf(
        //     text, sizeof(text),
        //     L"[%04d] %dx%d %d colors (%d bpp) [%04d]",
        //     counter % 10000,
        //     width, height,
        //     colors, depth,
        //     counter % 10000);
        // hagl_put_text(
        //     hagl_backend,
        //     text,
        //     half_width - wcslen(text) * 8 / 2,
        //     height - 16,
        //     15, // 15 - (counter % 8),
        //     font5x7);

        // swprintf(text, sizeof(text), L"<%04d>", counter);
        // for (uint8_t c = 1; c < 16; c++)
        // {
        //     hagl_put_text(hagl_backend, text, 80 + x + c, 15 + c * 13, 16 - c, font5x7);
        // }
        // x += dx;
        // if (x + 15 * wcslen(text) + 40 > width)
        // {
        //     dx = -dx;
        // }

        counter += 1;
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        sleep_ms(20);
        led = 1 - led;
    }
}

/* EOF */
