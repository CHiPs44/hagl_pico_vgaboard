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

void core1_entry()
{
    int16_t counter = 0;
    wchar_t text[80];

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    for ( int x = 0; x < DISPLAY_WIDTH; ++x ) {
        hagl_put_pixel(x,                  0, RGB_WHITE);
        hagl_put_pixel(x, DISPLAY_HEIGHT - 1, RGB_WHITE);
    }
    for ( int y = 0; y < DISPLAY_HEIGHT; ++y) {
        hagl_put_pixel(                0, y, RGB_WHITE);
        hagl_put_pixel(DISPLAY_WIDTH - 1, y, RGB_WHITE);
    }

    for(;;) {
        gpio_put(LED_PIN, counter % 2);
        // "1234567890"
        // " [123456] "
        swprintf(text, sizeof(text), L" [%06d] ", counter);
        hagl_put_text(text, 16, 16, RGB_RED, font6x9);
        hagl_put_text(text, 32, 32, RGB_GREEN, font6x9);
        hagl_put_text(text, 48, 48, RGB_BLUE, font6x9);
        hagl_put_text(text, 64, 64, RGB_CYAN, font6x9);
        hagl_put_text(text, 80, 80, RGB_YELLOW, font6x9);
        // sleep_ms(100);
        counter++;
    }
}

int main (void)
{
    // hagl_init();

    setup_video();
    sleep_ms(500);

    multicore_launch_core1(core1_entry);
    render_loop();

    // hagl_close();
}

// EOF
