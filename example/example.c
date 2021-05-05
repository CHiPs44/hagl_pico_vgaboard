/*

MIT No Attribution

Copyright (c) 2021 CHiPs44

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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vga-framebuffer.h"
#include "../external/embedded-fonts/X11/include/font8x13.h"

void example()
{
    uint16_t counter = 0;
    uint16_t counter2 = 0;
    wchar_t text[80];
    uint16_t x0, y0, x1, y1;
    uint16_t x;
    uint16_t y;
    int8_t dx = 1;
    uint16_t w;
    uint8_t col;
    uint8_t row;
    uint16_t y2 = DISPLAY_HEIGHT / 2 + 16;
    wchar_t *demo = L"HAGL RASPBERRY PI PICO 640x480x4 DEMO";

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    hagl_draw_vline(DISPLAY_WIDTH / 2 - 1, 0, DISPLAY_HEIGHT, 15);

    x = 32;
    y = 36;
    hagl_draw_hline(x, y -  2, DISPLAY_WIDTH - 1 - 2 * x, 15);
    hagl_put_text(demo, (DISPLAY_WIDTH / 2) - wcslen(demo) * 8 / 2, y, 15, font8x13);
    hagl_draw_hline(x, y + 14, DISPLAY_WIDTH - 1 - 2 * x, 15);

    x += 24;
    y += 24;
    for (uint8_t c = 0; c < 16; c++) {
        // "Nice" coloured frames around screen
        x0 = c * 2; x1 = DISPLAY_WIDTH  - 1 - c * 2;
        y0 = c * 2; y1 = DISPLAY_HEIGHT - 1 - c * 2;
        hagl_draw_rectangle(x0, y0, x1, y1, 15 - c);
        // Framed tile + value for each color in the palette
        col = c >= 8 ? 1 : 0;
        row = c % 8;
        x0 = x + col * (DISPLAY_WIDTH / 2 + x / 2) - col * (x - 4); x1 = x0 + 16;
        y0 = y + row * 20                                         ; y1 = y0 + 16;
        hagl_fill_rectangle(x0 + 1, y0 + 1, x1 - 1, y1 - 1, c);
        hagl_draw_rectangle(x0, y0, x1, y1, 15);
        /*
        TODO
        #define PICO_SCANVIDEO_PIXEL_RSHIFT 0
        #define PICO_SCANVIDEO_PIXEL_GSHIFT 6
        #define PICO_SCANVIDEO_PIXEL_BSHIFT 11
        #define PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b) (
            (((b)>>3)<<PICO_SCANVIDEO_PIXEL_BSHIFT)|
            (((g)>>3)<<PICO_SCANVIDEO_PIXEL_GSHIFT)|
            (((r)>>3)<<PICO_SCANVIDEO_PIXEL_RSHIFT))
        */
        uint8_t r = 0; //palette[c] & 0x
        uint8_t g = 0; //palette[c] & 0x
        uint8_t b = 0; //palette[c] & 0x
        swprintf(text, sizeof(text), L"#%02d => %04x => r%02xg%02xb%02x", c, palette[c], r, g, b);
        hagl_put_text(text, x0 + 24, y0 + 3, 15, font8x13);
        // Nice?
        w = (DISPLAY_WIDTH / 3) + c * 4;
        hagl_draw_hline(DISPLAY_WIDTH / 2 - w,      y2 + c * 4       , w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2    ,      y2 + c * 4       , w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2 - w, 80 + y2 + (16 - c) * 4, w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2    , 80 + y2 + (16 - c) * 4, w, c % 8);
    }

    x = 0;
    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, counter % 2);
        for (uint16_t c = 0; c < 16; c++)
        {
            w = counter % (DISPLAY_WIDTH / 3) + c * 4;
            hagl_draw_hline(DISPLAY_WIDTH / 2 - w,      y2 + c * 4       , w, c % 8 + counter % 8);
            hagl_draw_hline(DISPLAY_WIDTH / 2    ,      y2 + c * 4       , w, c % 8 + counter % 8);
            hagl_draw_hline(DISPLAY_WIDTH / 2 - w, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
            hagl_draw_hline(DISPLAY_WIDTH / 2    , 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        }
        hagl_put_text(
            demo, 
            (DISPLAY_WIDTH / 2) - wcslen(demo) * 8 / 2, 
            66 + y2, 
            15 - (counter % 8), 
            font8x13
        );
        // swprintf(text, sizeof(text), L" -<[%06d]>- ", counter);
        // for (uint8_t c = 1; c < NCLR; c++)
        // {
        //     hagl_put_text(text, 16 + 16 + x, 32 + 16 + (DISPLAY_HEIGHT / 2) + c * 10,  c, font8x13);
        // }
        // x += dx;
        // if (x > DISPLAY_WIDTH - 16 - (10 * 9)) {
        //     dx = -dx;
        // }
        sleep_ms(10);
        counter++;
    }
}

void core1_entry()
{
    // render_loop();
    example();
}

int main (void)
{
    set_sys_clock_khz(250000, true);
    hagl_init();
    sleep_ms(500);
    multicore_launch_core1(core1_entry);
    render_loop();
    example();
    // hagl_close();
}

// EOF
