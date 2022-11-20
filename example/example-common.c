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

uint32_t counter = 0;
int led = 0;
clock_t startTime, endTime, elapsedTime;
int hours, minutes, seconds, milliseconds;

#define WIDTH   (hagl_backend->width)
#define HEIGHT  (hagl_backend->height)
#define DEPTH   (hagl_backend->depth)
#define COLORS  (vgaboard->colors)
#define FREQ_HZ (vgaboard->freq_hz)

clock_t get_time()
{
    /* cf. https://lindevs.com/measure-execution-time-of-code-using-raspberry-pi-pico */
    return (clock_t) time_us_64() / 10000;
}

void start_time()
{
    counter = 0;
    led = 0;
    startTime = get_time();
}

void cycle_time(color_t color)
{
    wchar_t text[80];

    // Draw counter & elapsed time HH:MM:SS.mmm
    endTime = get_time();
    elapsedTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
    hours = elapsedTime / 1000 / 60 / 60;
    minutes = (elapsedTime / 1000 / 60) % 60;
    seconds = (elapsedTime / 1000) % 60;
    milliseconds = elapsedTime % 1000;
    swprintf(
        text, sizeof(text), 
        L"%06d %02d:%02d:%02d.%03d", 
        counter % 1000000, hours, minutes, seconds, milliseconds
    );
    hagl_put_text(hagl_backend, text, WIDTH / 2 - wcslen(text) * 8 / 2, HEIGHT - 13, color, font8x13B);
    // Next cycle
    counter += 1;
    gpio_put(PICO_DEFAULT_LED_PIN, led);
    // sleep_ms(1);
    led = 1 - led;
}

void draw_borders_and_axis(color_t color1, color_t color2, color_t color3)
{
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, WIDTH, HEIGHT, color1);
    // Missing pixel at bottom right corner (HAGL bug?)
    // hagl_put_pixel(hagl_backend, WIDTH - 1, HEIGHT - 1, 9);
    // X axis
    hagl_draw_hline(hagl_backend, 0, HEIGHT / 2 - 1, WIDTH - 1, color2);
    // Y axis
    hagl_draw_vline(hagl_backend, WIDTH / 2 - 1, 0, HEIGHT - 1, color3);
}

void draw_title(color_t color)
{
    wchar_t title[80];
    uint16_t x, y, w, h;
    swprintf(
        title, sizeof(title),
        L"VGA %dx%dx%dbpp@%dhz", 
        WIDTH, HEIGHT, DEPTH, FREQ_HZ);
    w = wcslen(title) * 8;
    h = 13;
    x = WIDTH / 2 - w / 2;
    y = 2;
    hagl_draw_rounded_rectangle_xywh(hagl_backend, x - 4, y - 2, w + 8, h + 4, 2, color);
    hagl_put_text(hagl_backend, title, x, y, 11, font8x13B);
}

void draw_palette(color_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    wchar_t text[80];

    /* Framed tile + index + value for each color in the palette */
    for (uint8_t c = 0; c < vgaboard->colors; c++)
    {
        uint16_t x0 = x + (c / 8) * (WIDTH / 2 / 2);
        uint16_t y0 = y + (c % 8) * (h + 2);
        hagl_fill_rectangle_xywh(hagl_backend, x0, y0, w, h, c);
        hagl_draw_rectangle_xywh(hagl_backend, x0, y0, w, h, c); // == 15 ? 8 : 15);
        swprintf(text, sizeof(text), L"%02d\u2192%04X", c, vgaboard_get_palette_color(c));
        hagl_put_text(hagl_backend, text, x0 + w + 5, y0 + 1, color, font5x7);
    }
}

/* EOF */
