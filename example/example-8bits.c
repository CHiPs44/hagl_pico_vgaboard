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

/* Only available with CHiPs44 fork of HAGL for now */
#define HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY

// Standard libs
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
// Pico SDK
#if !PICO_NO_HARDWARE
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pico/rand.h"
#endif
#include "pico.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
// Pico VGA Board
#include "pico-vgaboard.h"
// Colors
#include "pico-vgaboard-colors.h"
// Palettes
#include "pico-vgaboard-palettes-bubblegum16.h"
#include "pico-vgaboard-palettes-c64.h"
#include "pico-vgaboard-palettes-cga.h"
#include "pico-vgaboard-palettes-cpc.h"
#include "pico-vgaboard-palettes-dawnbringer16.h"
#include "pico-vgaboard-palettes-grey.h"
#include "pico-vgaboard-palettes-sweetie16.h"
#include "pico-vgaboard-palettes-console16.h"
#include "pico-vgaboard-palettes.h"
// Modes
#include "experimental/pico-vgaboard-modes-1024x576.h"
#include "experimental/pico-vgaboard-modes-1280x800.h"
#include "experimental/pico-vgaboard-modes-1680x1050.h"
#include "pico-vgaboard-modes-1024x768.h"
#include "pico-vgaboard-modes-1280x1024.h"
#include "pico-vgaboard-modes-1280x720.h"
#include "pico-vgaboard-modes-640x400.h"
#include "pico-vgaboard-modes-640x480.h"
#include "pico-vgaboard-modes-768x576.h"
#include "pico-vgaboard-modes-800x600.h"
// #include "experimental/pico-vgaboard-modes-1280x800.h"
// HAGL
#include "hagl.h"
#include "hagl_hal.h"

hagl_backend_t *hagl_backend = NULL;

// Convenient macros
#define WIDTH (hagl_backend->width)
#define HEIGHT (hagl_backend->height)
#define DEPTH (hagl_backend->depth)
#define COLORS (pico_vgaboard->colors)

/* "LIBS" */
/* clang-format off */
#include "font.h"
#include "font.c"
#include "rect.c"
#include "title.c"
#include "vsync.c"
#include "borders-and-axis.c"
/* clang-format on */

wchar_t *palette_name;
rect_t demo_window;

/* DEMOS */
#include "bars.c"
#include "figures.c"
#include "fonts.c"
#include "images-4bpp.c"
#include "images-8bpp.c"
#include "minimal.c"
#include "palette.c"
#include "rects.c"
#include "scroller.c"
#include "specs.c"
#include "sprites-4bpp.c"

typedef struct _demo_t
{
    wchar_t *name;
    bool (*init)();
    void (*draw)();
    void (*done)();
    int duration_s;
} demo_t;

/* clang-format off */
demo_t demos[] = {
    // { .name = L"Minimal"         , .init = minimal_init     , .draw = minimal_draw      , .done = NULL            , .duration_s = 10 },
    { .name = L"Specifications"  , .init = specs_init       , .draw = specs_draw        , .done = NULL            , .duration_s = 10 },
    // { .name = L"Palette"         , .init = palette_init     , .draw = palette_draw      , .done = NULL            , .duration_s = 10 },
    // { .name = L"Scroller"        , .init = scroller_init    , .draw = scroller_draw     , .done = NULL            , .duration_s = 45},
    // { .name = L"16 color images" , .init = images_4bpp_init , .draw = images_4bpp_draw  , .done = images_4bpp_done, .duration_s = 15 },
    // { .name = L"256 color images", .init = images_8bpp_init , .draw = images_8bpp_draw  , .done = images_8bpp_done, .duration_s = 15 },
    // { .name = L"16 color sprites", .init = sprites_init     , .draw = sprites_draw      , .done = sprites_done    , .duration_s = 1000 },
    // { .name = L"Hollow figures"  , .init = figures_init     , .draw = figures_draw      , .done = NULL            , .duration_s = 10 },
    // { .name = L"Filled figures"  , .init = figures_init     , .draw = figures_fill      , .done = NULL            , .duration_s = 10 },
    // { .name = L"Bars"            , .init = bars_init        , .draw = bars_draw         , .done = NULL            , .duration_s = 10 },
    // { .name = L"Rectangles"      , .init = rects_init       , .draw = rects_draw        , .done = NULL            , .duration_s = 10 },
    // { .name = L"Fonts"           , .init = fonts_init       , .draw = fonts_draw        , .done = NULL            , .duration_s = 10 },
};
/* clang-format on */
#define N_DEMOS (sizeof(demos) / sizeof(demo_t))
/** @brief Current demo index */
int demo;

/**
 * @brief Cycle through demos
 *        (does not return)
 */
void example(void)
{
    wchar_t title[40];
#if PICO_VGABOARD_DEBUG
    printf("*** EXAMPLE_%dX%dX%dBPP@%d ***\n", WIDTH, HEIGHT, DEPTH, pico_vgaboard->freq_hz);
#endif
    // init_windows(16, 0);
    init_windows(0, 0);
    // init_windows(HEIGHT <= 192 ? 0 : HEIGHT <= 240 ? 8
    //                                                : 16,
    //              8);
    // draw_borders_and_axis(&FULL_SCREEN, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    rect_copy(&DEMO, &demo_window);
    demo = 0;
    while (true)
    {
        wait_for_vblank();
#if PICO_VGABOARD_DEBUG
        wprintf(L"Launching #%d: %ls\r\n", demo, demos[demo].name);
#endif
        if (TITLE.h > 0)
        {
            clip(&TITLE);
            hagl_fill_rectangle_xywh(
                hagl_backend,
                TITLE.x, TITLE.y, TITLE.w, TITLE.h,
                DEPTH == 1 ? 0 : 1 + rand() % (COLORS - 1));
            swprintf(title, sizeof(title) / sizeof(wchar_t), L" %d/%d %ls ", demo + 1, N_DEMOS, demos[demo].name);
            title_draw(&TITLE, title);
        }
        clip(&DEMO);
        hagl_fill_rectangle_xywh(hagl_backend, DEMO.x, DEMO.y, DEMO.w, DEMO.h, rand() % COLORS);
        bool ok = demos[demo].init();
        if (ok)
        {
            clock_t demo_now = get_time_ms();
            clock_t demo_end = demo_now + demos[demo].duration_s * 1000;
            while (demo_now < demo_end)
            {
                wait_for_vblank();
                clip(&DEMO);
                demos[demo].draw();
                if (STATUS.h > 0)
                {
                    clip(&STATUS);
                    show_status();
                }
                demo_now = get_time_ms();
                // printf("now: %ld end: %ld\n", demo_now, demo_end);
            }
        }
        if (demos[demo].done != NULL)
        {
            demos[demo].done();
        }
        /**********************************************************************/
        demo = (demo + 1) % N_DEMOS;
    }
}

/**
 * @brief Setup VGA & HAGL
 */
void setup(const pico_vgaboard_t *vgaboard_model, uint16_t display_width, uint16_t display_height)
{
    stdio_init_all();
#if PICO_VGABOARD_DEBUG
    sleep_ms(250);
    printf("SETUP!\r\n");
    sleep_ms(250);
#endif
    pico_vgaboard_init();
    pico_vgaboard_setup(vgaboard_model, display_width, display_height, 0);
    hagl_backend = hagl_init();
}

int main(void)
{
    palette_name = L"Default";

    /* clang-format off */

    /**************************************************************************/
    /* 1BPP - MONOCHROME                                                      */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_512x384x1bpp_24576  ,   0,   0); // OK
    // setup(&pico_vgaboard_512x768x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x480x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_768x576x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_800x600x1bpp        ,   0,   0); // OK

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_640x200x1bpp_16000  ,   0,   0); // OK
    // setup(&pico_vgaboard_640x400x1bpp        ,   0,   0); // OK

    /******************************** 5:4 RATIO *******************************/
    // setup(&pico_vgaboard_640x512x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_1024x384x1bpp       , 0, 0); // KO, perf
    // setup(&pico_vgaboard_1024x768x1bpp_98304 , 0, 0); // KO, perf

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_640x360x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_1280x720x1bpp_115200, 0, 0); // KO, perf
    /********************************* PALETTES *******************************/
    // pico_vgaboard_set_palette(pico_vgaboard_palette_1bpp_default   ); palette_name = L"Monochrome";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_1bpp_amber     ); palette_name = L"Amber";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_1bpp_cpc_mode2 ); palette_name = L"CPC mode 2";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_1bpp_green     ); palette_name = L"Green";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_1bpp_paperwhite); palette_name = L"Paperwhite";

    /**************************************************************************/
    /* 2BPP - 4 COLORS                                                        */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_384x576x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_512x384x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_512x384x2bpp        , 480, 272); // OK (16:10 letterbox as 2x scale of TIC-80)
    // setup(&pico_vgaboard_640x240x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_800x300x2bpp        , 0  ,   0); // OK
    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_320x200x2bpp_16000  , 0  ,   0); // OK
    // setup(&pico_vgaboard_640x200x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_640x400x2bpp_64000  , 0  ,   0); // OK
    /********************************* PALETTES *******************************/
    // pico_vgaboard_set_palette(pico_vgaboard_palette_2bpp_amber    ); palette_name = L"Amber";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_2bpp_cpc_mode1); palette_name = L"CPC mode 1";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_2bpp_green    ); palette_name = L"Green";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_2bpp_grey     ); palette_name = L"Grey";

    /**************************************************************************/
    /* 4BPP - 16 COLORS                                                       */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_256x192x4bpp_24576_1,   0,   0); // OK (1024x768 based)
    // setup(&pico_vgaboard_256x192x4bpp_24576_2,   0,   0); // OK (768x576 based)
    // setup(&pico_vgaboard_256x192x4bpp_24576_2, 240, 136); // OK (768x576 based)
    // setup(&pico_vgaboard_320x240x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x240x4bpp        , 320, 200); // OK (so we have 320x200@60 in a standard mode)
    // setup(&pico_vgaboard_320x240x4bpp        , 256, 192); // OK
    // setup(&pico_vgaboard_384x288x4bpp        ,   0,   0); // OK (768x576 based)
    // setup(&pico_vgaboard_384x288x4bpp        , 320, 200); // OK
    // setup(&pico_vgaboard_384x288x4bpp        , 320, 240); // OK
    // setup(&pico_vgaboard_400x300x4bpp        , 320, 240); // OK
    // setup(&pico_vgaboard_384x288x4bpp        , 320, 200); // OK
    // setup(&pico_vgaboard_400x300x4bpp        , 224, 288); // OK (Pac-man rulez ;-))
    // setup(&pico_vgaboard_512x384x4bpp_98304  , 480, 272); // OK (2x scale of TIC-80)

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_160x200x4bpp_16000  ,   0,   0); // OK
    // setup(&pico_vgaboard_256x144x4bpp_18432_1,   0,   0); // OK
    // setup(&pico_vgaboard_256x144x4bpp_18432_1, 240, 136); // OK
    // setup(&pico_vgaboard_320x100x4bpp_16000  ,   0,   0); // OK (not very interesting...)
    // setup(&pico_vgaboard_320x180x4bpp        , 240, 136); // OK
    // setup(&pico_vgaboard_320x200x4bpp        , 240, 136); // OK
    // setup(&pico_vgaboard_320x256x4bpp        , 224, 256); // OK (Space Invaders rulez ;-))
    // setup(&pico_vgaboard_320x360x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x400x4bpp_64000  ,   0,   0); // OK
    // setup(&pico_vgaboard_256x384x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_512x144x4bpp        ,   0,   0); // OK (sort of: 144 lines is not enough...)
    // setup(&pico_vgaboard_256x288x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_512x192x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x180x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x200x4bpp        ,   0,   0); // OK
    setup(&pico_vgaboard_840x525x4bpp_1      ,   480,   272); // OK
    // setup(&pico_vgaboard_840x525x4bpp_2      ,   320,   200); // OK

    /********************************* PALETTES *******************************/
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_c64      ); palette_name = L"C64";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_cga      ); palette_name = L"CGA";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_cpc_mode0); palette_name = L"CPC mode 0";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_sweetie16); palette_name = L"Sweetie 16";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_db16     ); palette_name = L"Dawnbringer 16";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_bg16     ); palette_name = L"Bubblegum 16";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_grey     ); palette_name = L"Grey/Gray 16";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_co16     ); palette_name = L"Console 16";

    /**************************************************************************/
    /* 8BPP - 256 COLORS                                                      */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_160x240x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_192x288x8bpp        ,   0,   0); // KO
    // setup(&pico_vgaboard_256x192x8bpp_1      ,   0,   0); // OK (1024x768 based)
    // setup(&pico_vgaboard_256x192x8bpp_49152_2,   0,   0); // OK (768x576 based)
    // setup(&pico_vgaboard_256x192x8bpp_49152_2, 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)
    // setup(&pico_vgaboard_320x240x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x240x8bpp        , 320, 200); // OK (so we have 320x200@60 in a standard mode)
    // setup(&pico_vgaboard_320x240x8bpp        , 256, 192); // OK
    // setup(&pico_vgaboard_384x144x8bpp        ,   0,   0); // KO after a few seconds

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_160x200x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x200x8bpp_64000  ,   0,   0); // OK

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_256x144x8bpp        , 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)
    // setup(&pico_vgaboard_320x180x8bpp        , 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)

    /********************************* PALETTES *******************************/
    // pico_vgaboard_set_palette(pico_vgaboard_palette_8bpp_default); palette_name = L"IRGB";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_8bpp_rgb685 ); palette_name = L"RGB685";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_8bpp_aurora ); palette_name = L"Aurora";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_8bpp_rgb332 ); palette_name = L"RGB332";
    // pico_vgaboard_set_palette(pico_vgaboard_palette_8bpp_ansi   ); palette_name = L"ANSI 256";

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    srand(time(NULL));
#endif

    /* Random borders instead of default black ones */
    // pico_vgaboard->border_color_top    = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
    // pico_vgaboard->border_color_left   = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
    // pico_vgaboard->border_color_bottom = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
    // pico_vgaboard->border_color_right  = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;

    /* clang-format on */

#if PICO_VGABOARD_DEBUG
    printf("*** CORE1 => RENDER LOOP ***\n");
#endif
    multicore_launch_core1(pico_vgaboard_render_loop);
#if PICO_VGABOARD_DEBUG
    printf("*** CORE0 => DEMO ***\n");
#endif
    example();

    __builtin_unreachable();
#if PICO_VGABOARD_DEBUG
    printf("*** UNREACHABLE ***\n");
#endif
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
