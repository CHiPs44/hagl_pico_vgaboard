/*

MIT No Attribution

Copyright (c) 2018-2021 Mika Tuupola

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

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "hagl_hal.h"
#include "hagl.h"
#include "external/embedded-fonts/X11/include/font8x13.h"
#include "external/embedded-fonts/X11/include/font10x20.h"
#include "external/embedded-fonts/misc/viznut/include/unscii-8-fantasy.h"

int main()
{
    color_t color1 = hagl_color(128, 255, 255);
    color_t color2 = hagl_color(255, 128, 255);
    color_t color3 = hagl_color(255, 255, 128);
    uint16_t x, y;
    clock_t start, end;
    double time_spent;
    size_t bytes;

    wchar_t *text = L"The Quick Brown Fox Jumps Over The Lazy Dog 0123456789 Times!";

    hagl_init();
    start = clock();

    x = (DISPLAY_WIDTH  / 2) - wcslen(text) * 8 / 2;
    y = (DISPLAY_HEIGHT / 2) - 13 * 4;
    hagl_draw_rectangle(x - 2, y - 2, x + wcslen(text) * 8 + 2, y + 13 + 2, color1);
    hagl_put_text(text, x, y, color1, font8x13);

    x = (DISPLAY_WIDTH  / 2) - wcslen(text) * 10 / 2;
    y = (DISPLAY_HEIGHT / 2) - 20 / 2;
    hagl_draw_rectangle(x - 2, y - 2, x + wcslen(text) * 10 + 2, y + 20 + 2, color2);
    hagl_put_text(text, x, y, color2, font10x20);

    x = (DISPLAY_WIDTH  / 2) - wcslen(text) * 8 / 2;
    y = (DISPLAY_HEIGHT / 2) + 8 * 4;
    hagl_draw_rectangle(x - 2, y - 2, x + wcslen(text) * 8 + 2, y + 8 + 2, color3);
    hagl_put_text(text, x, y, color3, unscii_8_fantasy);

    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    bytes = hagl_flush();
    printf("\nGenerated %zu bytes in %g seconds.\n\n", bytes, time_spent);
    hagl_close();

    return 0;
}

