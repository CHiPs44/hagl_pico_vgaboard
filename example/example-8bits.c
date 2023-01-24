/*

MIT No Attribution

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

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

// Standard libs
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
// Pico
#include "pico.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
// Pico VGA Board
#include "pico-vgaboard.h"
// Colors
#include "pico-vgaboard-colors.h"
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

/* "LIBS" */
#include "srand-rosc.c"
#include "font.h"
#include "rect.c"
#include "font.c"
#include "vsync.c"
#include "title.c"
#include "borders-and-axis.c"
#include "scroller.c"

wchar_t *palette_name;
rect_t window;

/* DEMOS */
#include "minimal.c"
#include "specs.c"
#include "palette.c"
#include "figures.c"
#include "fonts.c"
#include "bars.c"
#include "rects.c"
#include "sprites-4bpp.c"

typedef struct _demo_t
{
    wchar_t *name;
    void (*init)();
    void (*draw)();
    int duration_s;
} demo_t;

demo_t demos[] = {
    // { .name = L"Minimal", .init = minimal_init    , .draw = minimal_draw  , .duration_s = 10 },
    { .name = L"Specifications", .init = specs_init    , .draw = specs_draw  , .duration_s = 10 },
    // { .name = L"Palette"       , .init = palette_init  , .draw = palette_draw, .duration_s = 10 },
    // { .name = L"Sprites"       , .init = sprites_init  , .draw = sprites_draw, .duration_s = 10 },
    // { .name = L"Hollow figures", .init = figures_init  , .draw = figures_draw, .duration_s = 10 },
    // { .name = L"Filled figures", .init = figures_init  , .draw = figures_fill, .duration_s = 10 },
    // { .name = L"Bars"          , .init = bars_init     , .draw = bars_draw   , .duration_s = 10 },
    // { .name = L"Rectangles"    , .init = rects_init    , .draw = rects_draw  , .duration_s = 10 },
    // // { .name = L"Fonts"         , .init = fonts_init    , .draw = fonts_draw  , .duration_s =  5 },
};
#define NDEMOS (sizeof(demos) / sizeof(demo_t))
int demo;

void example()
{
    wchar_t title[40];

    printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, vgaboard->freq_hz);

    init_windows(FONT8X8.h * 2, FONT8X8.h * 3 / 2);
    // draw_borders_and_axis(&FULL_SCREEN, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    // scroller_init(scroller);
    // scroller->font = &FONT8X8;

    rect_copy(&DEMO, &window);
    demo = 0;
    while (true)
    {
        wprintf(L"Lauching #%d: %ls\r\n", demo, demos[demo].name);
        // scroller->color = scroller_get_color();
        /**********************************************************************/
        clip(&TITLE);
        hagl_fill_rectangle_xywh(hagl_backend, TITLE.x, TITLE.y, TITLE.w, TITLE.h, 1 + rand() % (COLORS - 1));
        swprintf(title, sizeof(title), L" %d/%d %ls ", demo + 1, NDEMOS, demos[demo].name);
        title_draw(&TITLE, title);
        /**********************************************************************/
        clip(&DEMO);
        hagl_fill_rectangle_xywh(hagl_backend, DEMO.x, DEMO.y, DEMO.w, DEMO.h, 1 + rand() % (COLORS - 1));
        // rect_dump("WINDOW  ", &window);
        demos[demo].init();
        clock_t demo_end = get_time_ms() + demos[demo].duration_s * 1000;
        /**********************************************************************/
        while (get_time_ms() < demo_end)
        {
            wait_for_vblank();
            clip(&DEMO);
            demos[demo].draw();
            // scroller_draw(scroller);
            // clip(&FULL_SCREEN);
            // //    1234567890123456789012345678901234567890
            // hagl_put_text(
            //     hagl_backendhagl_backendhagl_backend12, 
            //     //12345678901234567890123456789012
            //     L"There should be a scroller here!", 
            //     SCROLLER.x, SCROLLER.y, 
            //     scroller->color, scroller->font->fontx
            // );
            cycle_time(0, SCROLLER.y - FONT5X7.h - 2, COLORS - 1);
        }
        /**********************************************************************/
        demo = (demo + 1) % NDEMOS;
    }
}

/**
 * @brief Setup VGA & HAGL
 */
void setup(const vgaboard_t *vgaboard_model)
{
    stdio_init_all();
    // vgaboard_init_led();
    printf("SETUP!\r\n");
    // vgaboard_flash_led_and_wait();
    vgaboard_init();
    // vgaboard_flash_led_and_wait();
    vgaboard_setup(vgaboard_model);
    // vgaboard_flash_led_and_wait();
    hagl_backend = hagl_init();
    // vgaboard_flash_led_and_wait();
    // vgaboard_dump(vgaboard);
    // vgaboard_flash_led_and_wait();
    // hagl_hal_dump(hagl_backend);
    // vgaboard_flash_led_and_wait();
}

int main(void)
{
    palette_name = L"Default";

    /* 1bpp */
    // setup(&vgaboard_640x200x1bpp_16000); // TODO!
    // setup(&vgaboard_512x768x1bpp); // OK
    // setup(&vgaboard_640x400x1bpp); // OK
    // setup(&vgaboard_640x480x1bpp); // OK
    // setup(&vgaboard_768x576x1bpp); // OK
    // setup(&vgaboard_800x600x1bpp); // OK
    // setup(&vgaboard_1024x384x1bpp); // KO, perf?
    // setup(&vgaboard_1024x768x1bpp_98304); // KO, perf
    // vgaboard_set_palette(vgaboard_palette_1bpp_amber);
    // vgaboard_set_palette(vgaboard_palette_1bpp_cpc_mode2);
    // vgaboard_set_palette(vgaboard_palette_1bpp_green);
    // vgaboard_set_palette(vgaboard_palette_1bpp_paperwhite);

    /* 2bpp */
    // setup(&vgaboard_320x200x2bpp_16000); // OK
    // setup(&vgaboard_384x576x2bpp); // OK?
    // setup(&vgaboard_512x384x2bpp); // OK?
    // setup(&vgaboard_640x200x2bpp); // OK?
    // setup(&vgaboard_640x240x2bpp); // OK?
    // setup(&vgaboard_640x400x2bpp_64000); // OK?
    // setup(&vgaboard_800x300x2bpp); // OK?
    // vgaboard_set_palette(vgaboard_palette_2bpp_amber); palette_name = L"Amber";
    // vgaboard_set_palette(vgaboard_palette_2bpp_cpc_mode1); palette_name = L"CPC";
    // vgaboard_set_palette(vgaboard_palette_2bpp_green); palette_name = L"Green";
    // vgaboard_set_palette(vgaboard_palette_2bpp_grey); palette_name = L"Grey";

    /* 4bpp */
    setup(&vgaboard_160x200x4bpp_16000); // OK
    // setup(&vgaboard_256x192x4bpp_24576_1); // OK (1024x768 based)
    // setup(&vgaboard_256x192x4bpp_24576_2); // OK (768x756 based)
    // setup(&vgaboard_320x200x4bpp); // OK
    // setup(&vgaboard_320x240x4bpp); // OK
    // setup(&vgaboard_320x360x4bpp); // KO, as all 1280x720 modes for now
    // setup(&vgaboard_320x400x4bpp_64000); // OK
    // setup(&vgaboard_320x256x4bpp); // KO, as all 1280x1024 modes for now, OK on my 27" Lenovo 
    // setup(&vgaboard_256x384x4bpp); // OK
    // setup(&vgaboard_384x288x4bpp); // OK
    // setup(&vgaboard_400x300x4bpp); // OK
    // setup(&vgaboard_512x192x4bpp); // OK
    // setup(&vgaboard_512x384x4bpp_98304); // KO, perf???
    // setup(&vgaboard_640x200x4bpp_64000); // OK
    // vgaboard_set_palette(vgaboard_palette_4bpp_c64      ); palette_name = L"C64";
    // vgaboard_set_palette(vgaboard_palette_4bpp_cga      ); palette_name = L"CGA";
    vgaboard_set_palette(vgaboard_palette_4bpp_cpc_mode0); palette_name = L"CPC";
    // vgaboard_set_palette(vgaboard_palette_4bpp_sweetie16); palette_name = L"Sweetie 16";

    /* 8bpp */
    // setup(&vgaboard_160x200x8bpp); // OK
    // setup(&vgaboard_160x240x8bpp); // OK
    // setup(&vgaboard_192x288x8bpp); // KO
    // setup(&vgaboard_256x192x8bpp_1); // OK (1024x768 based)
    // setup(&vgaboard_256x192x8bpp_2); // OK (768x576 based)
    // setup(&vgaboard_320x200x8bpp_64000); // OK
    // setup(&vgaboard_320x240x8bpp_76800); // OK
    // setup(&vgaboard_320x180x8bpp); // OK, sort of (flashing lines at top of screen & complete drops)
    // setup(&vgaboard_384x144x8bpp); // KO after a few seconds
    // vgaboard_set_palette(vgaboard_palette_8bpp_grey); palette_name = L"Grey";

    srand_rosc();

    printf("*** CORE1 => RENDER LOOP ***\n");
    vgaboard_enable();
    multicore_launch_core1(vgaboard_render_loop);
    sleep_ms(2000);
    printf("*** CORE0 => MINIMAL DEMO ***\n");
    example();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
