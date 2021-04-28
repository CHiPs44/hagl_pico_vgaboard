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
#include "pico/multicore.h"
#include "hardware/clocks.h"

#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vga-framebuffer.h"

void core1_entry()
{
    int counter = 0;
    char text[80];

    // const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);

    for ( int x = 0; x < DISPLAY_WIDTH; ++x ) {
        hagl_put_pixel(x,                  0, 15);
        hagl_put_pixel(x, DISPLAY_HEIGHT - 1, 15);
    }
    for ( int y = 0; y < DISPLAY_HEIGHT; ++y) {
        hagl_put_pixel(                0, y, 15);
        hagl_put_pixel(DISPLAY_WIDTH - 1, y, 15);
    }

    for(;;) {
        // gpio_put(LED_PIN, counter % 2);
        // "1234567890"
        // " [123456] "
        sprintf(text, " [%06d] ", counter++);
    }
}

int main (void)
{
    hagl_init();

    multicore_launch_core1 (core1_entry);

    hagl_close();
}

// EOF