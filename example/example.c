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


// cf. https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=305712&start=50#p1864466 (from @kilograham)
// NB: according to http://tinyvga.com/vga-timing/1024x768@60Hz, h-sync and v-sync polarities are negative
const scanvideo_timing_t vga_timing_1024x768_60_default = {
    .clock_freq = 65000000,

    .h_active = 1024,
    .v_active = 768,

    .h_front_porch = 24,
    .h_pulse = 136,
    .h_total = 1344,
    .h_sync_polarity = 0,

    .v_front_porch = 3,
    .v_pulse = 6,
    .v_total = 806,
    // KG .v_sync_polarity = 1,
    .v_sync_polarity = 0,

    .enable_clock = 0,
    .clock_polarity = 0,

    .enable_den = 0,
};

const scanvideo_mode_t vga_mode_1024x768_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    // .default_timing = &vga_timing_1024x768_60,
    .pio_program = &video_24mhz_composable,
    .width = 1024,
    .height = 768,
    .xscale = 1,
    .yscale = 1,
};

const scanvideo_mode_t vga_mode_512x384_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 512,
    .height = 384,
    .xscale = 2,
    .yscale = 2,
};

const scanvideo_mode_t vga_mode_256x192_60 = {
    .default_timing = &vga_timing_1024x768_60_default,
    .pio_program = &video_24mhz_composable,
    .width = 256,
    .height = 192,
    .xscale = 4,
    .yscale = 4,
};

// #define VGA_MODE (&vga_mode_1024x768_60)
// #define DISPLAY_WIDTH (1024)
// #define DISPLAY_HEIGHT (768)
// #define SYS_CLOCK_MHZ (260)

#define VGA_MODE (&vga_mode_512x384_60)
#define DISPLAY_WIDTH (512)
#define DISPLAY_HEIGHT (384)
#define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_256x192_60)
// #define DISPLAY_WIDTH (256)
// #define DISPLAY_HEIGHT (192)
// #define SYS_CLOCK_MHZ (260)

// #define VGA_MODE (&vga_mode_640x480_60)
// #define DISPLAY_WIDTH (640)
// #define DISPLAY_HEIGHT (480)
// #define SYS_CLOCK_MHZ (250)

// #define VGA_MODE (&vga_mode_320x240_60)
// #define DISPLAY_WIDTH (320)
// #define DISPLAY_HEIGHT (240)
// #define SYS_CLOCK_MHZ (250)

// #define VGA_MODE (&vga_mode_160x120_60)
// #define DISPLAY_WIDTH (160)
// #define DISPLAY_HEIGHT (120)
// #define SYS_CLOCK_MHZ (250)

#include "pico-vga-framebuffer.h"
#include "hagl_hal.h"
#include "hagl.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"

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
    wchar_t *demo = L"HAGL RASPBERRY PI PICO 16 COLORS VGA DEMO";

    printf("*** EXAMPLE ***\n");
    printf("Width: %d Height: %d\n", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    printf("Width: %d Height: %d\n", VGA_MODE->width, VGA_MODE->height);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    hagl_draw_vline(DISPLAY_WIDTH / 2 - 1, 0                     , DISPLAY_HEIGHT - 1, 7);
    hagl_draw_hline(0                    , DISPLAY_HEIGHT / 2 - 1, DISPLAY_WIDTH  - 1, 8);

    x = 32;
    y = 36;
    hagl_draw_hline(x, y - 2, DISPLAY_WIDTH - 1 - 2 * x, 15);
    hagl_put_text(demo, (DISPLAY_WIDTH / 2) - wcslen(demo) * 8 / 2, y, 15, font8x13);
    hagl_draw_hline(x, y + 14, DISPLAY_WIDTH - 1 - 2 * x, 15);

    x += 24;
    y += 24;
    for (uint8_t c = 0; c < 16; c++)
    {
        // "Nice" coloured frames around screen
        x0 = c * 2;
        x1 = DISPLAY_WIDTH - 1 - c * 2;
        y0 = c * 2;
        y1 = DISPLAY_HEIGHT - 1 - c * 2;
        hagl_draw_rectangle(x0, y0, x1, y1, 15 - c);
        // Framed tile + value for each color in the palette
        col = c >= 8 ? 1 : 0;
        row = c % 8;
        x0 = x + col * (DISPLAY_WIDTH / 2 + x / 2) - col * (x - 4);
        x1 = x0 + 16;
        y0 = y + row * 20;
        y1 = y0 + 16;
        hagl_fill_rectangle(x0 + 1, y0 + 1, x1 - 1, y1 - 1, c);
        hagl_draw_rectangle(x0, y0, x1, y1, 15);
        swprintf(text, sizeof(text), L"#%02d => %04x", c, hagl_hal_get_color(c));
        hagl_put_text(text, x0 + 24, y0 + 3, 15, font8x13);
        // Nice animation?
        w = (DISPLAY_WIDTH / 3) + c * 4;
        hagl_draw_hline(DISPLAY_WIDTH / 2 - w, y2 + c * 4, w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2    , y2 + c * 4, w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2 - w, 80 + y2 + (16 - c) * 4, w, c % 8);
        hagl_draw_hline(DISPLAY_WIDTH / 2    , 80 + y2 + (16 - c) * 4, w, c % 8);
    }

    x = 0;
    for (;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, counter % 2);
        // for (uint16_t c = 0; c < 16; c++)
        // {
        //     w = counter % (DISPLAY_WIDTH / 3) + c * 4;
        //     hagl_draw_hline(DISPLAY_WIDTH / 2 - w, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(DISPLAY_WIDTH / 2, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(DISPLAY_WIDTH / 2 - w, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(DISPLAY_WIDTH / 2, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        // }
        swprintf(text, sizeof(text), L"%s [%06d]", demo, counter);
        hagl_put_text(
            text,
            (DISPLAY_WIDTH / 2) - wcslen(text) * 8 / 2,
            66 + y2,
            15 - (counter % 8),
            font8x13);
        // swprintf(text, sizeof(text), L" -<[%06d]>- ", counter);
        // for (uint8_t c = 1; c < NCLR; c++)
        // {
        //     hagl_put_text(text, 16 + 16 + x, 32 + 16 + (DISPLAY_HEIGHT / 2) + c * 10,  c, font8x13);
        // }
        // x += dx;
        // if (x > DISPLAY_WIDTH - 16 - (10 * 9)) {
        //     dx = -dx;
        // }
        sleep_ms(100);
        if (counter % 10 == 0)
        {
            printf("Counter %d\n", counter);
        }
        counter++;
    }
}

int main(void)
{
    set_sys_clock_khz(SYS_CLOCK_MHZ * 1000, true);
    stdio_init_all();
    sleep_ms(500);

    printf("*** INITIALIZATION ***\n");
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
