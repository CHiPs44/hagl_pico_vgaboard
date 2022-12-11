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

#define USE_LED 1

// Standard libs
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
// Pico
#include "pico.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
// VGA Board
#include "pico-vgaboard.h"
// Palettes
#include "pico-vgaboard-palettes.h"
#include "pico-vgaboard-palettes-c64.h"
#include "pico-vgaboard-palettes-cga.h"
#include "pico-vgaboard-palettes-cpc.h"
#include "pico-vgaboard-palettes-grey.h"
#include "pico-vgaboard-palettes-sweetie16.h"
// Modes
#include "pico-vgaboard-modes-640x400.h"
#include "pico-vgaboard-modes-640x480.h"
#include "pico-vgaboard-modes-768x576.h"
#include "pico-vgaboard-modes-800x600.h"
#include "pico-vgaboard-modes-1024x768.h"
#include "pico-vgaboard-modes-1280x720.h"
#include "pico-vgaboard-modes-1280x1024.h"
// HAGL
#include "hagl_hal.h"
#include "hagl.h"
hagl_backend_t *hagl_backend = NULL;

// Convenient macros
#define WIDTH       (hagl_backend->width)
#define HEIGHT      (hagl_backend->height)
#define DEPTH       (hagl_backend->depth)
#define COLORS      (vgaboard->colors)
#define FREQ_HZ     (vgaboard->freq_hz)
#define FRAMEBUFFER (vgaboard->framebuffer)

#include "srand-rosc.c"
#include "font.h"
#include "rect.c"
#include "font.c"
#include "vsync.c"
#include "title.c"
#include "borders-and-axis.c"
#include "scroller.c"

rect_t window;

#define NDEMOS 6
#include "specs.c"
#include "palette.c"
#include "figures.c"
#include "fonts.c"
#include "bars.c"
#include "rects.c"

typedef struct _demo_t
{
    wchar_t *name;
    void (*init)();
    void (*draw)();
    int duration_s;
} demo_t;

demo_t demos[NDEMOS] = {
    { .name = L"Specifications", .init = specs_init  , .draw = specs_draw  , .duration_s = 10 },
    { .name = L"Palette"       , .init = palette_init, .draw = palette_draw, .duration_s = 10 },
    { .name = L"Figures"       , .init = figures_init, .draw = figures_draw, .duration_s = 10 },
    { .name = L"Fonts"         , .init = fonts_init  , .draw = fonts_draw  , .duration_s = 10 },
    { .name = L"Bars"          , .init = bars_init   , .draw = bars_draw   , .duration_s = 10 },
    { .name = L"Rectangles"    , .init = rects_init  , .draw = rects_draw  , .duration_s = 10 },
};
int demo;

#include "example-1bpp.c"
#include "example-2bpp.c"
#include "example-4bpp.c"
#include "example-8bpp.c"
#include "example-16bpp.c"
// #include "vgafont8/vgafont8_demo_4bpp.c"

void led_init()
{
#if USE_LED
    // We use the onboard LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
#endif
}

void led_flash_and_wait()
{
#if USE_LED
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(250);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(250);
#else
    sleep_ms(500);
#endif
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
    stdio_init_all();
    led_init();
    // Set system clock
    printf("SYSCLOCK: SETUP INIT\n");
    uint32_t old_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    bool ok = set_sys_clock_khz(vgaboard_model->sys_clock_khz, false);
    uint32_t new_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    led_flash_and_wait();
    stdio_init_all();
    led_flash_and_wait();
    led_flash_and_wait();
    printf("*** System clock speed %d kHz (before: %d, asked %d kHz: %s) ***\n",
           new_sys_clock_khz,
           old_sys_clock_khz,
           vgaboard_model->sys_clock_khz,
           ok ? "OK" : "KO");
    printf("SYSCLOCK: SETUP DONE\n");
    // led_flash_and_wait();

    printf("VGABOARD: SETUP INIT\n");
    vgaboard_init();
    // Set palette
    vgaboard_setup(vgaboard_model);
    // // Fill framebuffer with non zero values
    // for (int i = 0; i < vgaboard->framebuffer_size; i++)
    // {
    //     vgaboard->framebuffer[i] = i & 0xff;
    // }
    vgaboard_dump(vgaboard);
    printf("VGABOARD: SETUP DONE\n");
    // led_flash_and_wait();

    printf("HAGL: SETUP INIT\n");
    hagl_backend = hagl_init();
    hagl_hal_dump(hagl_backend);
    printf("HAGL: SETUP DONE\n");
    // led_flash_and_wait();
}

int main(void)
{
    /* 1bpp */
    // init(&vgaboard_512x768x1bpp); // OK
    // init(&vgaboard_640x480x1bpp); // OK
    // init(&vgaboard_768x576x1bpp); // OK
    // init(&vgaboard_800x600x1bpp); //OK
    // init(&vgaboard_1024x384x1bpp); // KO, perf?
    // init(&vgaboard_1024x768x1bpp_98304); // KO, perf
    // vgaboard_set_palette(vgaboard_palette_1bpp_green);
    // vgaboard_set_palette(vgaboard_palette_1bpp_amber);
    // vgaboard_set_palette(vgaboard_palette_1bpp_paperwhite);
    // vgaboard_set_palette(vgaboard_palette_1bpp_cpc_mode2);

    /* 2bpp */
    // init(&vgaboard_384x576x2bpp); // OK
    // init(&vgaboard_512x384x2bpp); // OK
    // init(&vgaboard_640x240x2bpp); // OK
    // init(&vgaboard_800x300x2bpp); // OK
    // vgaboard_set_palette(vgaboard_palette_2bpp_green);
    // vgaboard_set_palette(vgaboard_palette_2bpp_grey);
    // vgaboard_set_palette(vgaboard_palette_2bpp_cpc_mode1);

    /* 4bpp */
    // init(&vgaboard_256x192x4bpp_24576); // OK
    init(&vgaboard_320x200x4bpp); // OK
    // init(&vgaboard_320x240x4bpp); // OK
    // init(&vgaboard_320x360x4bpp); // OK
    // init(&vgaboard_320x400x4bpp_64000); // OK
    // init(&vgaboard_320x256x4bpp); // KO, as all 1280x1024 modes for now, OK on my 27" Lenovo 
    // init(&vgaboard_256x384x4bpp); // OK
    // init(&vgaboard_384x288x4bpp); // ??
    // init(&vgaboard_400x300x4bpp); // OK
    // init(&vgaboard_512x192x4bpp); // OK
    // init(&vgaboard_512x384x4bpp_98304); // KO, perf???
    // init(&vgaboard_640x200x4bpp_64000); // OK
    // vgaboard_set_palette(vgaboard_palette_4bpp_c64);
    // vgaboard_set_palette(vgaboard_palette_4bpp_cga);
    // vgaboard_set_palette(vgaboard_palette_4bpp_cpc_mode0);
    // vgaboard_set_palette(vgaboard_palette_4bpp_sweetie16);

    /* 8bpp */
    // init(&vgaboard_160x200x8bpp); // OK
    // init(&vgaboard_160x240x8bpp); // OK
    // init(&vgaboard_192x288x8bpp); // KO
    // init(&vgaboard_320x200x8bpp_64000); // OK
    // init(&vgaboard_320x240x8bpp_76800); // OK
    // init(&vgaboard_320x180x8bpp); // OK, sort of (flashing lines at top of screen & complete drops)
    // init(&vgaboard_384x144x8bpp); // KO after a few seconds
    // vgaboard_set_palette(vgaboard_palette_8bpp_grey);

    /* 16bpp - stable, no real demo yet */
    // init(&vgaboard_160x120x16bpp); // OK, sort of, weird colors
    // init(&vgaboard_192x144x16bpp); // OK, sort of, weird colors
    // init(&vgaboard_192x288x16bpp_110592); // OK, sort of, weird colors

    // /* HELP! vgaboard_render_loop should work on core1 */
    // //  NB: from pico-extras/src/common/pico_scanvideo/README.adoc (line 220)
    // //      You should call `scanvideo_setup` and `scanvideo_timing_enable`
    // //      from the core you wish to use for IRQs (it doesn't matter which
    // //      of, or if, both cores are being used for scanline generation).
    // printf("*** CORE1 => RENDER LOOP ***\n");
    // multicore_launch_core1(vgaboard_render_loop);
    // printf("*** CORE0 => EXAMPLE ***\n");
    // switch (vgaboard->depth)
    // {
    // case 1:
    //     example_1bpp();
    //     break;
    // case 2:
    //     example_2bpp();
    //     break;
    // case 4:
    //     example_4bpp();
    //     break;
    // case 8:
    //     example_8bpp();
    //     break;
    // // case 16:
    // //     example_16bpp();
    // //     break;
    // default:
    //     panic("No example for %d depth!", vgaboard->depth);
    //     break;
    // }

    printf("*** CORE1 => EXAMPLE %dbpp ***\n", vgaboard->depth);
    srand_rosc();
    switch (vgaboard->depth)
    {
    case 1:
        multicore_launch_core1(example_1bpp);
        break;
    case 2:
        multicore_launch_core1(example_2bpp);
        break;
    case 4:
        multicore_launch_core1(example_4bpp);
        break;
    case 8:
        multicore_launch_core1(example_8bpp);
        break;
    case 16:
        multicore_launch_core1(example_16bpp);
        break;
    default:
        panic("No example for %d depth!", vgaboard->depth);
        break;
    }
    // multicore_launch_core1(vgafont8_demo_4bpp);
    printf("*** CORE0 => RENDER LOOP ***\n");
    vgaboard_render_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
}

// EOF
