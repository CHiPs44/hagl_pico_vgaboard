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

// #include "vga_1024x768.c"

// scanvideo_mode_t vga_mode =   (&vga_mode_1024x768_60_chips44)
// uint16_t display_width =  (1024)
// uint16_t display_height =  (768)
// uint16_t display_colors =  (2)
// uint16_t sys_clock_mhz =  (260)

// scanvideo_mode_t vga_mode =   (&vga_mode_512x384_60_chips44)
// uint16_t display_width =  (512)
// uint16_t display_height =  (384)
// uint16_t display_colors =  (16)
// uint16_t sys_clock_mhz =  (260)

const scanvideo_mode_t vga_mode_640x240_60_chips44 = {
    .default_timing = &vga_timing_640x480_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 640,
    .height = 480,
    .xscale = 1,
    .yscale = 2,
};
const scanvideo_mode_t vga_mode_320x240_60_chips44 = {
    .default_timing = &vga_timing_640x480_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 640,
    .height = 480,
    .xscale = 2,
    .yscale = 2,
};
const scanvideo_mode_t display_mode = vga_mode_320x240_60_chips44;
uint16_t display_width = 320;
uint16_t display_height = 240;
uint16_t display_depth = 4;
uint16_t display_colors = 16;
uint16_t *display_palette = ((uint16_t *)(&vgaboard_default_palette_4bpp));
uint16_t sys_clock_mhz = 250;

// scanvideo_mode_t vga_mode =   (&vga_mode_320x240_60)
// uint16_t display_width =  (320)
// uint16_t display_height =  (240)
// uint16_t display_colors =  (16)
// uint16_t sys_clock_mhz =  (250)

// scanvideo_mode_t vga_mode =   (&vga_mode_160x120_60)
// uint16_t display_width =  (160)
// uint16_t display_height =  (120)
// uint16_t display_colors =  (16)
// uint16_t sys_clock_mhz =  (250)

// scanvideo_mode_t vga_mode =   (&vga_mode_320x240_60)
// uint16_t display_width =  (320)
// uint16_t display_height =  (240)
// uint16_t display_depth =  (8)
// uint16_t display_colors =  (256)
// uint16_t *display_palette =  (&vgaboard_default_palette_8)
// uint16_t sys_clock_mhz =  (250)

#include "hagl_hal.h"
#include "hagl.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"
#include "./external/embedded-fonts/X11/include/font8x13B.h"
#include "./external/embedded-fonts/X11/include/font6x10.h"
// #include "./external/embedded-fonts/misc/viznut/include/unscii-8.h"

void example(/*bool loop*/)
{
    bool loop = true;
    const uint16_t half_width = display_width / 2;
    const uint16_t half_height = display_height / 2;
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

    hagl_set_clip_window(0, 0, display_width - 1, display_height - 1);

    /* Borders & axis */
    hagl_draw_rectangle(0, 0, display_width - 1, display_height - 1, 9);
    hagl_draw_hline(0, half_height - 1, display_width - 1, 10);
    hagl_draw_vline(half_width - 1, 0, display_height - 1, 12);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        L"VGA Raspberry Pi Pico HAGL");
    // printf("demo:%s\n", demo);
    x = half_width - wcslen(demo) * 8 / 2;
    y = half_height / 8 - 13 / 2 - 1;
    w = display_width - 1 - 2 * x;
    // hagl_draw_hline(x, y - 2, w, 15);
    // hagl_fill_rectangle(x, y - 1, x + w - 1, y + 13, 10);
    hagl_draw_rounded_rectangle(x - 4, y - 4, x + w + 4, y + 13 + 4 - 2, 3, 13);
    hagl_put_text(demo, x, y, 11, font8x13B);
    // hagl_put_text(demo, x, y + 16, 15, unscii_8);
    // hagl_draw_hline(x, y + 14, w, 15);

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
        hagl_fill_rounded_rectangle(x0, y0, x1, y1, 3, c);
        hagl_draw_rounded_rectangle(x0, y0, x1, y1, 3, c == 15 ? 8 : 15);
        swprintf(text, sizeof(text), L"%02d %04X", c, vgaboard_get_color(c));
        hagl_put_text(text, x0 + 26, y0 + 1, 15, font6x10);
    }

    x = 0;
    while (loop)
    {
        /* Make LED blink */
        gpio_put(PICO_DEFAULT_LED_PIN, counter % 2);

        // // Draw lines
        // y2 = half_height + 16 * 2 - 10;
        // for (uint16_t c = 0; c < 16; c++)
        // {
        //     w = counter % (display_width / 4) + c * 4;
        //     hagl_draw_hline(half_width - w, y2 + c * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, y2 + c * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width - w, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, 40 + y2 + (16 - c) * 2, w, c % 8 + counter % 8);
        // }

        // Draw text
        swprintf(
            text, sizeof(text),
            L"[%04d] %dx%dx%d %d colors [%04d]",
            counter, display_width, display_height, display_depth, display_colors, counter);
        hagl_put_text(
            text,
            half_width - wcslen(text) * 8 / 2,
            display_height - 16,
            15, // 15 - (counter % 8),
            font8x13B);

        swprintf(text, sizeof(text), L"<%04d>", counter);
        for (uint8_t c = 1; c < 16; c++)
        {
            hagl_put_text(text, 80 + x, 15 + c * 13, 16 - c, font8x13);
        }
        x += dx;
        if (x + 15 * wcslen(text) + 40 > display_width)
        {
            dx = -dx;
        }

        counter = (counter + 1) % 10000;
    }
}

int main(void)
{
    sleep_ms(500);
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    bool ok = set_sys_clock_khz(sys_clock_mhz * 1000, true);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    stdio_init_all();
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("*** INITIALIZATION (clock %d : %s) ***\n", sys_clock_mhz * 1000, ok ? "OK" : "KO");
    printf("System clock speed %d Hz\n", clock_get_hz(clk_sys));

    vgaboard_setup(&display_mode, display_depth, display_palette);
    printf("VGABOARD: SETUP DONE\n");

    hagl_init();
    printf("HAGL_INIT DONE\n");

    printf("*** EXAMPLE ***\n");
    multicore_launch_core1(example);

    printf("*** RENDER LOOP ***\n");
    // multicore_launch_core1(vgaboard_render_loop);
    vgaboard_render_loop();

    // printf("*** EXAMPLE ***\n");
    // example();

    printf("*** UNREACHABLE ***\n");
    hagl_close();
}

// EOF
