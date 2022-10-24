/*

MIT No Attribution

Copyright (c) 2021-2022 Christophe Petit

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

#define HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "pico.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "pico-vgaboard-framebuffer.h"
#include "pico-vgaboard-modes.h"

#include "hagl_hal.h"
#include "hagl.h"
#include "font5x7.h"
// #include "./external/embedded-fonts/X11/include/font8x13.h"
// #include "./external/embedded-fonts/X11/include/font8x13B.h"
// #include "./external/embedded-fonts/X11/include/font6x10.h"
// #include "./external/embedded-fonts/misc/viznut/include/unscii-8.h"

hagl_backend_t *hagl_backend = NULL;

void example_320x240x4()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;
    uint16_t counter = 0;
    uint16_t counter2 = 0;
    wchar_t text[80];
    uint16_t x0, y0, x1, y1;
    uint16_t x;
    uint16_t y;
    int8_t dx = 1;
    uint16_t w;
    uint16_t y2;
    wchar_t demo[80];

    printf("*** EXAMPLE_320X240X4 ***\n");

    // if (false)
    // {
    /* Borders & axis */
    hagl_draw_rectangle(hagl_backend, 0, 0, vgaboard->width - 1, vgaboard->height - 1, 9);
    // hagl_put_pixel(hagl_backend, vgaboard->width - 1, vgaboard->height - 1, 9);
    hagl_draw_hline(hagl_backend, 0, half_height - 1, vgaboard->width - 1, 10);
    hagl_draw_vline(hagl_backend, half_width - 1, 0, vgaboard->height - 1, 12);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        // 0123465789012346578901234657890123465789
        L"Raspberry Pi Pico VGA board: HAGL HAL");
    x = half_width - wcslen(demo) * 8 / 2;
    y = half_height / 8 - 13 / 2 - 1;
    w = vgaboard->width - 1 - 2 * x;
    hagl_draw_rounded_rectangle(hagl_backend, x - 4, y - 4, x + w + 4, y + 13 + 4 - 2, 3, 13);
    hagl_put_text(hagl_backend, demo, x, y, 11, font5x7); // font8x13B);

    /* Draw palette */
    x = 8;
    y += 20;
    for (uint8_t c = 0; c < 16; c++)
    {
        /* Framed tile + value for each color in the palette */
        x0 = x;
        x1 = x0 + 20;
        y0 = y + c * 12;
        y1 = y0 + 10;
        hagl_fill_rounded_rectangle(hagl_backend, x0, y0, x1, y1, 3, c);
        hagl_draw_rounded_rectangle(hagl_backend, x0, y0, x1, y1, 3, c == 15 ? 8 : 15);
        swprintf(text, sizeof(text), L"%02d %04X", c, vgaboard_get_color(c));
        hagl_put_text(hagl_backend, text, x0 + 26, y0 + 1, 15, font5x7); // font6x10);
    }
    // }

    int led = 0;
    x = 0;
    while (true)
    {
        // if (false)
        // {

        // // Draw lines
        // y2 = half_height + 16 * 2 - 10;
        // for (uint16_t c = 0; c < 16; c++)
        // {
        //     w = counter % (vgaboard->width / 4) + c * 4;
        //     hagl_draw_hline(half_width - w, y2 + c * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, y2 + c * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width - w, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        // }

        // Draw text
        swprintf(
            text, sizeof(text),
            L"[%04d] %dx%d %d colors (%d bpp) [%04d]",
            counter % 10000,
            vgaboard->width, vgaboard->height,
            vgaboard->colors, vgaboard->depth,
            counter % 10000);
        hagl_put_text(
            hagl_backend,
            text,
            half_width - wcslen(text) * 8 / 2,
            vgaboard->height - 16,
            15,       // 15 - (counter % 8),
            font5x7); // font8x13B);

        swprintf(text, sizeof(text), L"<%04d>", counter);
        for (uint8_t c = 1; c < 16; c++)
        {
            hagl_put_text(hagl_backend, text, 80 + x, 15 + c * 13, 16 - c, font5x7); // font8x13);
        }
        x += dx;
        if (x + 15 * wcslen(text) + 40 > vgaboard->width)
        {
            dx = -dx;
        }
        // }
        counter += 1;
        if (counter % 100 == 0)
        {
            gpio_put(PICO_DEFAULT_LED_PIN, led);
            // sleep_ms(250);
            led = 1 - led;
        }
    }
}

void flash_led()
{
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(250);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(250);
}

void scanvideo_dump(scanvideo_mode_t *scanvideo_mode)
{
    printf("*** SCANVIDEO_MODE ***\n");
    printf("\tW: %d\tH: %d\tX: %d\tY: %d\tD: %d\n",
           scanvideo_mode->width, scanvideo_mode->height,
           scanvideo_mode->xscale, scanvideo_mode->yscale,
           scanvideo_mode->yscale_denominator);
}

void init(vgaboard_t *vgaboard1)
{
    gpio_init(PICO_DEFAULT_LED_PIN);
    flash_led();
    printf("SYSCLOCK: SETUP INIT\n");
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    bool ok = set_sys_clock_khz(vgaboard1->sys_clock_khz, true);
    flash_led();
    stdio_init_all();
    flash_led();
    flash_led();

    printf("*** System clock speed %d kHz (asked %d kHz : %s) ***\n",
           clock_get_hz(clk_sys) / 1000,
           vgaboard1->sys_clock_khz,
           ok ? "OK" : "KO");
    printf("SYSCLOCK: SETUP DONE\n");
    flash_led();

    printf("VGABOARD: SETUP INIT\n");
    vgaboard_init();
    vgaboard_setup(vgaboard1->scanvideo_mode, vgaboard1->depth, vgaboard1->palette);
    printf("VGABOARD: SETUP DONE\n");
    flash_led();

    printf("HAGL: SETUP INIT\n");
    hagl_backend = hagl_init();
    hagl_hal_set_width(vgaboard->width);
    hagl_hal_set_height(vgaboard->height);
    hagl_hal_set_depth(vgaboard->depth);
    hagl_set_clip(hagl_backend, 0, 0, hagl_hal_get_width() - 1, hagl_hal_get_height() - 1);
    printf("HAGL: SETUP DONE\n");
    flash_led();
}

int main(void)
{
    init((vgaboard_t *)(&vgaboard_320x240x4));

    /** HELP! vgaboard_render_loop should work on core1 */
    // printf("*** RENDER LOOP ***\n");
    // multicore_launch_core1(vgaboard_render_loop);
    // printf("*** EXAMPLE ***\n");
    // example_320x240x4();

    printf("*** EXAMPLE ***\n");
    multicore_launch_core1(example_320x240x4);
    printf("*** RENDER LOOP ***\n");
    vgaboard_render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
}

// EOF
