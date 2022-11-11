/*

MIT No Attribution

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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
#include "pico/stdlib.h"

// #define VGABOARD_FRAMEBUFFER_SIZE (96 * 1024)
#include "pico-vgaboard-framebuffer.h"
#include "pico-vgaboard-palettes.h"
#include "pico-vgaboard-palettes-cga.h"
#include "pico-vgaboard-modes-640x480.h"
#include "pico-vgaboard-modes-640x400.h"
#include "pico-vgaboard-modes-1024x768.h"

#define HAGL_HAL_DEBUG 1
#include "hagl_hal.h"
#include "hagl.h"
#include "./external/embedded-fonts/X11/include/font5x7.h"
#include "./external/embedded-fonts/X11/include/font8x13.h"
#include "./external/embedded-fonts/X11/include/font8x13B.h"
// #include "./external/embedded-fonts/X11/include/font6x10.h"
// #include "unscii-8.h"

hagl_backend_t *hagl_backend = NULL;

#include "example-4bpp.c"
#include "example-8bpp.c"

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
    // We use the onboard LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    // Set system clock
    printf("SYSCLOCK: SETUP INIT\n");
    uint32_t old_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    bool ok = set_sys_clock_khz(vgaboard_model->sys_clock_khz, true);
    uint32_t new_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    flash_led();
    stdio_init_all();
    flash_led();
    printf("*** System clock speed %d kHz (before: %d, asked %d kHz: %s) ***\n",
           new_sys_clock_khz,
           old_sys_clock_khz,
           vgaboard_model->sys_clock_khz,
           ok ? "OK" : "KO");
    printf("SYSCLOCK: SETUP DONE\n");
    flash_led();

    printf("VGABOARD: SETUP INIT\n");
    vgaboard_init();
    vgaboard_setup(vgaboard_model);
    // vgaboard_set_palette(vgaboard_cga_palette_4bpp);
    vgaboard_set_palette(vgaboard_default_palette_4bpp);
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

#include "vgafont8/vgafont8_demo_4bpp.c"

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

// const scanvideo_mode_t vga_mode_640x120_60_chips44 = {
//     .default_timing = &vga_timing_640x480_60_default,
//     .pio_program = &video_24mhz_composable,
//     .width = 640,
//     .height = 480,
//     .xscale = 1,
//     .yscale = 4,
// };

// /** @brief 640x120@60Hz, 4bpp, 16 colors */
// const vgaboard_t vgaboard_640x120x4bpp = {
//     .scanvideo_mode = &vga_mode_640x120_60_chips44,
//     .freq_hz = 60,
//     .depth = 4,
//     .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
//     .sys_clock_khz = 250000L,
// };

// /** @brief 640x120@60Hz, 4bpp, 16 colors */
// const vgaboard_t vgaboard_640x120x2bpp = {
//     .scanvideo_mode = &vga_mode_640x120_60_chips44,
//     .freq_hz = 60,
//     .depth = 2,
//     .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
//     .sys_clock_khz = 250000L,
// };

// base from src/common/pico_scanvideo/vga_modes.c
/*
from https://www.intel.com/content/dam/support/us/en/programmable/support-resources/fpga-wiki/asset03/basic-vga-controller-design-example.pdf
Pixel Clock Frequency: 33.33MHz (33MHz will suffice)
Table 1: LCD timing parameters for the NEEK board
Horizontal Scanline Part Pixels Vertical Frame part Lines
Visible area                800 Visible area          480
Front Porch                 210 Front Porch            22
Sync Pulse                   30 Sync Pulse             13
Back Porch                   16 Back Porch             10
Whole line                 1056 Whole frame           525
*/
const scanvideo_timing_t vga_timing_wide_480_intel =
{
    // .clock_freq = 33000000,
    // 34.483 kHz
    .clock_freq = 34000000,

    .h_active = 800,
    .v_active = 480,

// cf. https://misterfpga.org/viewtopic.php?t=2552
    // .h_front_porch = 210,
    .h_front_porch = 40,
    // .h_pulse = 30,
    .h_pulse = 48,
    // .h_total = 1056,
    .h_total = 928,
    .h_sync_polarity = 0,

    // .v_front_porch = 22,
    .v_front_porch = 13,
    // .v_pulse = 13,
    .v_pulse = 3,
    .v_total = 525,
    .v_sync_polarity = 0,

    .enable_clock = 1,
    .clock_polarity = 0,

    .enable_den = 1
};

const scanvideo_mode_t vga_mode_tft_400x240_intel =
{
    .default_timing = &vga_timing_wide_480_intel,
    .pio_program = &video_24mhz_composable,
    // .width = 400,
    // .height = 240,
    .width = 800,
    .height = 480,
    .xscale = 2,
    .yscale = 2,
};

/** @brief 400x240@50Hz, 4bpp, 16 colors */
const vgaboard_t vgaboard_400x240x4bpp = {
    .scanvideo_mode = &vga_mode_tft_400x240_intel,
    .freq_hz = 255,
    .depth = 4,
    .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
    // .sys_clock_khz = 33000L * 8L,
    .sys_clock_khz = 34000L * 8L,
};

int main(void)
{
    // init(&vgaboard_640x480x1bpp); // KO, timing issues, optimization required
    // init(&vgaboard_320x240x1bpp); // KO, idem above

    // init(&vgaboard_512x384x4bpp); // ???
    // init(&vgaboard_640x240x2bpp); // KO, idem above
    // init(&vgaboard_640x120x2bpp); // ??

    // init(&vgaboard_256x192x4bpp); // OK
    // init(&vgaboard_320x240x4bpp); // OK
    init(&vgaboard_320x200x4bpp); // OK
    // init(&vgaboard_400x240x4bpp); // WIDE
    // init(&vgaboard_512x192x4bpp); // OK
    // init(&vgaboard_640x120x4bpp); // OK
    // init(&vgaboard_640x200x4bpp); // OK

    // init(&vgaboard_320x120x8bpp); // quite OK, some quirks with text & lines, "blocks" quite OK
    // init(&vgaboard_320x200x8bpp); // Same as other 8bpp mode

    // init(&vgaboard_160x120x16bpp); // ??? => stable, no demo yet

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

    printf("*** CORE1 => EXAMPLE ***\n");
    multicore_launch_core1(example_4bpp);
    // multicore_launch_core1(example_8bpp);
    // multicore_launch_core1(vgafont8_demo_4bpp);
    printf("*** CORE0 => RENDER LOOP ***\n");
    vgaboard_render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
}

// EOF
