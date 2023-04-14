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

#include "srand-rosc.c"
#include "font.h"
#include "rect.c"
#include "font.c"
#include "vsync.c"
#include "title.c"
#include "borders-and-axis.c"
#include "scroller.c"

#include "minimal.c"

rect_t window;

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

demo_t demos[] = {
    { .name = L"Minimal", .init = minimal_init  , .draw = minimal_draw  , .duration_s = 10 },
    { .name = L"Specifications", .init = specs_init    , .draw = specs_draw  , .duration_s =  5 },
    { .name = L"Palette"       , .init = palette_init  , .draw = palette_draw, .duration_s = 10 },
    { .name = L"Hollow figures", .init = figures_init  , .draw = figures_draw, .duration_s = 10 },
    { .name = L"Filled figures", .init = figures_init  , .draw = figures_fill, .duration_s = 10 },
    { .name = L"Bars"          , .init = bars_init     , .draw = bars_draw   , .duration_s = 10 },
    { .name = L"Rectangles"    , .init = rects_init    , .draw = rects_draw  , .duration_s = 10 },
    // { .name = L"Fonts"         , .init = fonts_init    , .draw = fonts_draw  , .duration_s =  5 },
};
#define NDEMOS (sizeof(demos) / sizeof(demo_t))
int demo;

void example_16bpp()
{
    // uint16_t x, y, w, h;
    // uint16_t x0, y0, x1, y1, x2, y2;

    // printf("*** EXAMPLE_%dX%dX%dBPP@%dHZ ***\n", WIDTH, HEIGHT, DEPTH, vgaboard->freq_hz);

    init_windows(FONT8X13B.h, FONT8X13B.h);
    draw_borders_and_axis(&FULL_SCREEN, BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);
    title_draw(&FULL_SCREEN, L"Pico VGA HAGL", BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);
    palette_draw(&TOP_LEFT, BGAR5515_WHITE, BGAR5515_WHITE);
    specs_draw(&TOP_RIGHT, BGAR5515_WHITE, BGAR5515_WHITE, BGAR5515_WHITE);

    start_time();
    while (true)
    {
        scanvideo_wait_for_vblank();
#if PICO_VGABOARD_DEBUG
            vgaboard_frame_counter += 1;
            if (vgaboard_frame_counter % 100==0) {
                printf("VGABOARD: COUNTER=%d\n", vgaboard_frame_counter / 100);
            }
#endif
        // TODO!
        show_status(0, 0, BGAR5515_WHITE);
    }
}

/**
 * @brief Setup VGA & HAGL
 */
void setup(const vgaboard_t *vgaboard_model)
{
    stdio_init_all();
    vgaboard_init();
    vgaboard_setup(vgaboard_model);
    hagl_backend = hagl_init();
    // vgaboard_dump(vgaboard);
    // hagl_hal_dump(hagl_backend);
}

int main(void)
{
    /* 16bpp - stable, no real demo yet */
    // setup(&vgaboard_160x120x16bpp); // KO, perfs?
    // setup(&vgaboard_192x144x16bpp); // KO, perfs?
    // setup(&vgaboard_192x288x16bpp_110592); // Too much RAM

    srand_rosc();

    printf("*** CORE1 => RENDER LOOP ***\n");
    // vgaboard_enable();
    multicore_launch_core1(vgaboard_render_loop);
    sleep_ms(2000);
    printf("*** CORE0 => DEMO ***\n");
    example();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
