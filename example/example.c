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
#include "font6x9.h"

#define LED_PIN PICO_DEFAULT_LED_PIN

void example()
{
    int16_t counter = 0;
    wchar_t text[80];

    // for ( int x = 0; x < DISPLAY_WIDTH; ++x ) {
    //     hagl_put_pixel(x,                  0, RGB565_WHITE);
    //     hagl_put_pixel(x, DISPLAY_HEIGHT - 1, RGB565_WHITE);
    // }
    // for ( int y = 0; y < DISPLAY_HEIGHT; ++y) {
    //     hagl_put_pixel(                0, y, RGB565_WHITE);
    //     hagl_put_pixel(DISPLAY_WIDTH - 1, y, RGB565_WHITE);
    // }
    hagl_draw_rectangle(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, palette[ 3]);
    hagl_draw_rectangle(8, 8, DISPLAY_WIDTH - 8, DISPLAY_HEIGHT - 8, palette[11]);
    for (uint8_t i = 0; i < NCLR; i++)
    {
        swprintf(text, sizeof(text), L" #%02d => %04x", i, palette[i]);
        hagl_put_text(text, 16, 16 + (i * 8), palette[15], font6x9);
    }

    for(;;)
    {
        gpio_put(LED_PIN, counter % 2);
        // "1234567890"
        // " [123456] "
        swprintf(text, sizeof(text), L" -<[%06d]>- ", counter);
        hagl_put_text(text, 16 + (counter % (DISPLAY_WIDTH / 2)), 16 + (DISPLAY_HEIGHT / 2), RGB565_RED   , font6x9);
        hagl_put_text(text, 32 + (counter % (DISPLAY_WIDTH / 2)), 32 + (DISPLAY_HEIGHT / 2), RGB565_GREEN , font6x9);
        hagl_put_text(text, 48 + (counter % (DISPLAY_WIDTH / 2)), 48 + (DISPLAY_HEIGHT / 2), RGB565_BLUE  , font6x9);
        hagl_put_text(text, 64 + (counter % (DISPLAY_WIDTH / 2)), 64 + (DISPLAY_HEIGHT / 2), RGB565_CYAN  , font6x9);
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
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    example();
    // hagl_close();
}

// EOF
