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

#include "pico-vga-framebuffer.h"

#include "vga_1024x768.c"

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

const scanvideo_mode_t vga_mode_640x240_60 = {
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
uint16_t *display_palette = ((uint16_t *)(&vgafb_default_palette_4));
uint16_t sys_clock_mhz = 180; //250

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
// uint16_t *display_palette =  (&vgafb_default_palette_8)
// uint16_t sys_clock_mhz =  (250)

#include "hagl_hal.h"
#include "hagl.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"

void example()
{
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
    uint8_t col;
    uint8_t row;
    uint16_t y2 = half_height + 16;
    wchar_t demo[80];

    hagl_set_clip_window(0, 0, display_width - 1, display_height - 1);

    hagl_draw_rectangle(0, 0, display_width - 1, display_height - 1, 15);
    hagl_draw_vline(half_width - 1, 0, display_height - 1, 15);
    hagl_draw_hline(0, half_height - 1, display_width - 1, 15);

    /* Title */
    x = 32;
    y = 36;
    swprintf(
        demo, sizeof(demo),
        L"HAGL RASPBERRY PI PICO %dx%dx16 COLORS VGA DEMO",
        display_width, display_height);
    printf("demo:%s\n", demo);
    hagl_draw_hline(x, y - 2, display_width - 1 - 2 * x, 15);
    hagl_put_text(demo, half_width - wcslen(demo) * 8 / 2, y, 15, font8x13);
    hagl_draw_hline(x, y + 14, display_width - 1 - 2 * x, 15);

    // // x += 24;
    // y += 24;
    // for (uint8_t c = 0; c < 16; c++)
    // {
    //     /* Nice? coloured frames around screen */
    //     // x0 = c * 2;
    //     // x1 = display_width - 1 - c * 2;
    //     // y0 = c * 2;
    //     // y1 = display_height - 1 - c * 2;
    //     // hagl_draw_rectangle(x0, y0, x1, y1, 15 - c);

    //     /* Framed tile + value for each color in the palette */
    //     col = c >= 8 ? 1 : 0;
    //     row = c % 8;
    //     x0 = x + col * (half_width + x / 2) - col * (x - 4);
    //     x1 = x0 + 16;
    //     y0 = y + row * 20;
    //     y1 = y0 + 16;
    //     hagl_fill_rectangle(x0, y0, x1, y1, c);
    //     hagl_draw_rectangle(x0, y0, x1, y1, 15);
    //     swprintf(text, sizeof(text), L"#%02d => %04x", c, vgafb_get_color(c));
    //     hagl_put_text(text, x0 + 24, y0 + 3, 15, font8x13);

    //     /* Prepare nice? animation */
    //     // w = (display_width / 3) + c * 4;
    //     // hagl_draw_hline(half_width - w, y2 + c * 4, w, c % 8);
    //     // hagl_draw_hline(half_width    , y2 + c * 4, w, c % 8);
    //     // hagl_draw_hline(half_width - w, 80 + y2 + (16 - c) * 4, w, c % 8);
    //     // hagl_draw_hline(half_width    , 80 + y2 + (16 - c) * 4, w, c % 8);
    // }

    x = 0;
    for (;;)
    {
        /* Make LED blink */
        gpio_put(PICO_DEFAULT_LED_PIN, counter % 2);
        // for (uint16_t c = 0; c < 16; c++)
        // {
        //     w = counter % (display_width / 3) + c * 4;
        //     hagl_draw_hline(half_width - w, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, y2 + c * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width - w, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        //     hagl_draw_hline(half_width, 80 + y2 + (16 - c) * 4, w, c % 8 + counter % 8);
        // }
        /*
        swprintf(text, sizeof(text), L"[%04d] %ls [%04d]", counter, demo, counter);
        hagl_put_text(
            text,
            half_width - wcslen(text) * 8 / 2,
            66 + y2,
            15 - (counter % 8),
            font8x13);
        */
        // swprintf(text, sizeof(text), L" -<[%06d]>- ", counter);
        // for (uint8_t c = 1; c < 16; c++)
        // {
        //     hagl_put_text(text, 16 + 16 + x, 32 + 16 + (half_height) + c * 10,  c, font8x13);
        // }
        // x += dx;
        // if (x > display_width - 16 - (10 * 9)) {
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
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    bool ok = true;
    // #ifdef SYS_CLOCK_MHZ
    // ok = set_sys_clock_khz(sys_clock_mhz * 1000, true);
    // #endif
    sleep_ms(500);
    stdio_init_all();
    sleep_ms(500);
    printf("*** INITIALIZATION (clock: %s) ***\n", ok ? "OK" : "KO");

    vgafb_setup(&display_mode, display_depth, display_palette);
    printf("VGAFB: VGA SETUP DONE\n");

    hagl_init();
    printf("HAGL_INIT DONE\n");

    printf("*** EXAMPLE ***\n");
    multicore_launch_core1(example);

    // uint8_t *p = &vgafb_framebuffer;
    // for (uint16_t i = 0; i < (640 * 240 / 2); i++)
    // {
    //     *p++ = i % 256;
    // }

    printf("*** RENDER LOOP ***\n");
    vgafb_render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close();
}

// EOF
