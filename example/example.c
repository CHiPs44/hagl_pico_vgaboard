/*

MIT No Attribution

Copyright (c) 2021 Christophe Petit

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

// #include "vga_1024x768.c"

#define VGA_MODE (&vga_mode_640x480_60)
#define DISPLAY_WIDTH (640)
#define DISPLAY_HEIGHT (480)
#define SYS_CLOCK_MHZ (250)

// #define VGA_MODE (&vga_mode_320x240_60)
// #define DISPLAY_WIDTH (320)
// #define DISPLAY_HEIGHT (240)
// #define SYS_CLOCK_MHZ (250)

// #define VGA_MODE (&vga_mode_160x120_60)
// #define DISPLAY_WIDTH (160)
// #define DISPLAY_HEIGHT (120)
// #define SYS_CLOCK_MHZ (250)

#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vga-framebuffer.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"

void example()
{
    const uint16_t HALF_WIDTH = DISPLAY_WIDTH / 2;
    const uint16_t HALF_HEIGHT = DISPLAY_HEIGHT / 2;
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
    uint16_t y2 = HALF_HEIGHT + 16;
    wchar_t demo[80];

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

    hagl_draw_rectangle (0             , 0              , DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, 15);
    hagl_draw_vline     (HALF_WIDTH - 1, 0              , DISPLAY_HEIGHT - 1, 15);
    hagl_draw_hline     (0             , HALF_HEIGHT - 1, DISPLAY_WIDTH  - 1, 15);

    /* Title */
    x = 32;
    y = 36;
    swprintf(
        demo, sizeof(demo), 
        L"HAGL RASPBERRY PI PICO %dx%dx16 COLORS VGA DEMO", 
        DISPLAY_WIDTH, DISPLAY_HEIGHT
    );
    hagl_draw_hline(x, y -  2, DISPLAY_WIDTH - 1 - 2 * x, 15);
    hagl_put_text(demo, HALF_WIDTH - wcslen(demo) * 8 / 2, y, 15, font8x13);
    hagl_draw_hline(x, y + 14, DISPLAY_WIDTH - 1 - 2 * x, 15);

    // x += 24;
    y += 24;
    for (uint8_t c = 0; c < 16; c++)
    {
        /* Nice? coloured frames around screen */
        // x0 = c * 2;
        // x1 = DISPLAY_WIDTH - 1 - c * 2;
        // y0 = c * 2;
        // y1 = DISPLAY_HEIGHT - 1 - c * 2;
        // hagl_draw_rectangle(x0, y0, x1, y1, 15 - c);

        /* Framed tile + value for each color in the palette */
        col = c >= 8 ? 1 : 0;
        row = c % 8;
        x0 = x + col * (HALF_WIDTH + x / 2) - col * (x - 4);
        x1 = x0 + 16;
        y0 = y + row * 20;
        y1 = y0 + 16;
        hagl_fill_rectangle(x0, y0, x1, y1, c);
        hagl_draw_rectangle(x0, y0, x1, y1, 15);
        swprintf(text, sizeof(text), L"#%02d => %04x", c, hagl_hal_get_color(c));
        hagl_put_text(text, x0 + 24, y0 + 3, 15, font8x13);

        /* Prepare nice? animation */
        // w = (DISPLAY_WIDTH / 3) + c * 4;
        // hagl_draw_hline(HALF_WIDTH - w, y2 + c * 4, w, c % 8);
        // hagl_draw_hline(HALF_WIDTH    , y2 + c * 4, w, c % 8);
        // hagl_draw_hline(HALF_WIDTH - w, 80 + y2 + (16 - c) * 4, w, c % 8);
        // hagl_draw_hline(HALF_WIDTH    , 80 + y2 + (16 - c) * 4, w, c % 8);
    }

    x = 0;
    for (;;)
    {
        /* Make LED blink */
        gpio_put(PICO_DEFAULT_LED_PIN, counter % 2);
        // for (uint16_t c = 0; c < 16; c++)
        // {
        //     w = counter % (DISPLAY_WIDTH / 3) + c * 4;
        //     hagl_draw_hline(HALF_WIDTH - w, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(HALF_WIDTH, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(HALF_WIDTH - w, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(HALF_WIDTH, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        // }
        swprintf(text, sizeof(text), L"[%06d] %ls [%06d]", counter, demo, counter);
        hagl_put_text(
            text,
            HALF_WIDTH - wcslen(text) * 8 / 2,
            66 + y2,
            15 - (counter % 8),
            font8x13);
        // swprintf(text, sizeof(text), L" -<[%06d]>- ", counter);
        // for (uint8_t c = 1; c < NCLR; c++)
        // {
        //     hagl_put_text(text, 16 + 16 + x, 32 + 16 + (HALF_HEIGHT) + c * 10,  c, font8x13);
        // }
        // x += dx;
        // if (x > DISPLAY_WIDTH - 16 - (10 * 9)) {
        //     dx = -dx;
        // }
        // if (counter % 10 == 0)
        // {
        //     printf("Counter %d\n", counter);
        // }
        sleep_ms(250);
        counter++;
    }
}

int main(void)
{
    bool ok = true;
#ifdef SYS_CLOCK_MHZ
    ok = set_sys_clock_khz(SYS_CLOCK_MHZ * 1000, true);
#endif
    stdio_init_all();
    sleep_ms(1000);

    printf("*** INITIALIZATION (clock: %s) ***\n", ok ? "OK" : "KO");
    hagl_hal_set_vga_mode(VGA_MODE);
    hagl_init();

    printf("*** EXAMPLE ***\n");
    multicore_launch_core1(example);

    printf("*** RENDER LOOP ***\n");
    render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close();
}

// EOF
