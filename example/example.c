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
// #include "pico-vgaboard-modes-640x480.h"
#include "pico-vgaboard-modes-640x400.h"

#define HAGL_HAL_DEBUG 1
#include "hagl_hal.h"
#include "hagl.h"
#include "./external/embedded-fonts/X11/include/font5x7.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"
// #include "./external/embedded-fonts/X11/include/font8x13B.h"
// #include "./external/embedded-fonts/X11/include/font6x10.h"
// #include "unscii-8.h"

hagl_backend_t *hagl_backend = NULL;

void example_4bpp()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t depth = vgaboard->depth;
    const uint16_t colors = vgaboard->colors;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;

    uint32_t counter = 0;
    wchar_t text[80];
    uint16_t x0, y0, x1, y1, y2;
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
        L"Raspberry Pi Pico VGA board: HAGL HAL");
    // wprintf(L"demo2: ls[%ls] %d\n", &demo, wcslen(demo));
    w = wcslen(demo) * 8 - 1;
    h = 13;
    x = half_width - w / 2;
    y = 8;
    hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 4, y - 4, w + 4, h + 4, 3, 13);
    hagl_put_text(hagl_backend, demo, x, y, 11, font8x13);

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
        hagl_put_text(hagl_backend, text, x0 + 26, y0 + 1, 15, font5x7);
        // wprintf(L"text: %ls\n", &text);
    }

    int led = 0;
    x = 0;
    while (true)
    {
        // // // Draw lines
        // // y2 = half_height + 16 * 2 - 10;
        // // for (uint16_t c = 0; c < 16; c++)
        // // {
        // //     w = counter % (vgaboard->width / 4) + c * 4;
        // //     hagl_draw_hline(half_width - w, y2 + c * 2, w, c % 8 + counter % 8);
        // //     hagl_draw_hline(half_width, y2 + c * 2, w, c % 8 + counter % 8);
        // //     hagl_draw_hline(half_width - w, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        // //     hagl_draw_hline(half_width, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        // // }

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
        sleep_ms(100);
        // printf(".");
        // if (counter % 100 == 0)
        // {
        //     printf("\n");
        // }
        led = 1 - led;
    }
}

void example_8bpp()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t depth = vgaboard->depth;
    const uint16_t colors = vgaboard->colors;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;

    uint16_t x, y, w, h;
    wchar_t demo[40];

    printf("*** EXAMPLE_%dX%dX%d ***\n", width, height, depth);

    hagl_set_clip(hagl_backend, 0, 0, width - 1, height - 1);

    /* Borders */
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, width, height, 0x7f);
    // Missing pixel at bottom right corner (HAGL bug?)
    hagl_put_pixel(hagl_backend, vgaboard->width - 1, vgaboard->height - 1, 0x7f);
    // X axis
    // hagl_draw_hline(hagl_backend, 0, half_height - 1, vgaboard->width - 1, 0x7f);
    // Y axis
    // hagl_draw_vline(hagl_backend, half_width - 1, 0, vgaboard->height - 1, 0x7f);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        L"VGA: %dx%dx%d/%d@%dMHz",
        width, height, depth, colors,
        clock_get_hz(clk_sys) / 1000000);
    wprintf(L"demo: %ls (%d)\n", &demo, wcslen(demo));
    w = wcslen(demo) * 8 - 1;
    h = 13;
    x = half_width - w / 2;
    y = 8;
    // hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 4, y - 4, w + 4, h + 4, 3, 0x42);
    hagl_put_text(hagl_backend, demo, x, y, 0xff, font8x13);

    w = 15;
    h = 9;
    for (uint16_t c = 0; c < 256; c += 1)
    {
        x = 8 + (c % 16) * (w + 1);
        y = 8 * 3 + (c / 16) * (h + 1);
        hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
        // hagl_draw_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 1, h + 1, c == 0xff ? 0x7f : 0xff);
    }

    uint32_t counter = 0;
    int led = 0;
    while (true)
    {
        /*
         * Palette swap test
         * TODO /!\ gets darker and darker with time /!\
         */
        // uint8_t i = rand() % 256;
        // uint8_t j = rand() % 256;
        // uint16_t tmp = vgaboard->palette[i];
        // vgaboard->palette[i] = vgaboard->palette[j];
        // vgaboard->palette[j] = tmp;

        /*
         * Random lines
         */
        // x = rand() % width;
        // y = rand() % height;
        // w = rand() % width;
        // h = rand() % height;
        // hagl_draw_line(hagl_backend, x, y, x + w - 1, y + h - 1, counter % colors);
        // // hagl_draw_hline_xyw(hagl_backend, x, y, w, counter % colors);
        // // hagl_draw_vline_xyh(hagl_backend, x, y, h, colors - counter % (colors-1));

        /*
         * Blink LED to show activity and wait a bit
         */
        counter += 1;
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        sleep_ms(100);
        led = 1 - led;
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

/**
 * @brief Initialize hardware & HAGL
 *
 * @param vgaboard_model
 */
void init(const vgaboard_t *vgaboard_model)
{
    // We use the LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    // Set system clock
    printf("SYSCLOCK: SETUP INIT\n");
    bool ok = set_sys_clock_khz(vgaboard_model->sys_clock_khz, true);
    flash_led();
    stdio_init_all();
    flash_led();
    printf("*** System clock speed %d kHz (asked %d kHz : %s) ***\n",
           clock_get_hz(clk_sys) / 1000,
           vgaboard_model->sys_clock_khz,
           ok ? "OK" : "KO");
    printf("SYSCLOCK: SETUP DONE\n");
    flash_led();

    printf("VGABOARD: SETUP INIT\n");
    vgaboard_init();
    vgaboard_setup(vgaboard_model);
        // vgaboard_model->scanvideo_mode,
        // vgaboard_model->depth,
        // vgaboard_model->palette);
    // // Fill framebuffer with non zero values
    // for (int i = 0; i < vgaboard->framebuffer_size; i++)
    // {
    //     vgaboard->framebuffer[i] = i & 0xff;
    // }
    vgaboard_dump(vgaboard);
    printf("VGABOARD: SETUP DONE\n");
    flash_led();

    printf("HAGL: SETUP INIT\n");
    hagl_backend = hagl_init();
    hagl_backend->width = vgaboard->width;
    hagl_backend->height = vgaboard->height;
    hagl_backend->depth = vgaboard->depth;
    hagl_set_clip(hagl_backend, 0, 0, hagl_backend->width - 1, hagl_backend->height - 1);
    hagl_hal_dump(hagl_backend);
    printf("HAGL: SETUP DONE\n");
    flash_led();
}

// // Testing 1bpp with a known working mode
// const vgaboard_t vgaboard_320x240x1bpp = {
//     .scanvideo_mode = &vga_mode_320x240_60_chips44,
//     .width = 320,
//     .height = 240,
//     .depth = 1,
//     .colors = 2,
//     .palette = ((uint16_t *)(&vgaboard_default_palette_1bpp)),
//     .sys_clock_khz = 250000L,
// };

int main(void)
{
    // init(&vgaboard_320x240x4bpp); // OK
    // init(&vgaboard_320x120x8bpp); // quite OK, some quirks with text & lines, "blocks" OK
    // init(&vgaboard_640x480x1bpp); // KO, timing issues, optimization required
    // init(&vgaboard_320x240x1bpp); // KO, idem above
    // init(&vgaboard_320x200x4bpp); // OK
    init(&vgaboard_320x200x8bpp); // Same as other 8bpp mode

    /** HELP! vgaboard_render_loop should work on core1 */
    //  NB: from pico-extras/src/common/pico_scanvideo/README.adoc (line 220)
    //      You should call `scanvideo_setup` and `scanvideo_timing_enable`
    //      from the core you wish to use for IRQs (it doesn't matter which
    //      of, or if, both cores are being used for scanline generation).
    // printf("*** CORE1 => RENDER LOOP ***\n");
    // multicore_launch_core1(vgaboard_render_loop);
    // printf("*** CORE0 => EXAMPLE ***\n");
    // example_4bpp();
    // // example_8bpp();

    // printf("*** EXAMPLE ***\n");
    // multicore_launch_core1(example_4bpp);
    multicore_launch_core1(example_8bpp);
    printf("*** RENDER LOOP ***\n");
    vgaboard_render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
}

// EOF
