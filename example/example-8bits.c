/*

MIT No Attribution

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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
#include <string.h>
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
// Buttons
#include "pico-vgaboard-buttons.h"
// Colors
#include "pico-vgaboard-colors.h"
// Palettes
#include "palettes/pico-vgaboard-palettes-bubblegum16.h"
#include "palettes/pico-vgaboard-palettes-c64.h"
#include "palettes/pico-vgaboard-palettes-cga.h"
#include "palettes/pico-vgaboard-palettes-console16.h"
#include "palettes/pico-vgaboard-palettes-cpc.h"
#include "palettes/pico-vgaboard-palettes-cx16.h"
#include "palettes/pico-vgaboard-palettes-dawnbringer16.h"
#include "palettes/pico-vgaboard-palettes-grey.h"
#include "palettes/pico-vgaboard-palettes-sweetie16.h"
#include "palettes/pico-vgaboard-palettes.h"
// Modes
#include "modes/experimental/pico-vgaboard-modes-1024x576.h"
#include "modes/experimental/pico-vgaboard-modes-1280x800.h"
#include "modes/experimental/pico-vgaboard-modes-1680x1050.h"
#include "modes/experimental/pico-vgaboard-modes-640x512.h"
// #include "modes/experimental/pico-vgaboard-modes-840x525.h"
#include "modes/pico-vgaboard-modes-1024x768.h"
#include "modes/pico-vgaboard-modes-1280x1024.h"
#include "modes/pico-vgaboard-modes-1280x720.h"
#include "modes/pico-vgaboard-modes-640x400.h"
#include "modes/pico-vgaboard-modes-640x480.h"
#include "modes/pico-vgaboard-modes-768x576.h"
#include "modes/pico-vgaboard-modes-800x600.h"

// HAGL
#include "hagl.h"
#include "hagl_hal.h"

hagl_backend_t *hagl_backend = NULL;

// Convenient macros
#define WIDTH (hagl_backend->width)
#define HEIGHT (hagl_backend->height)
#define DEPTH (hagl_backend->depth)
#define COLORS (pico_vgaboard->colors)

typedef struct _palette_t
{
    wchar_t *name;
    wchar_t *code;
    const BGAR5515 *palette;
} palette_t;

/* clang-format off */
palette_t palettes1[] = {
    { /* 0 */ .name = L"White on black"     , .code = L"MONO2 ", .palette = pico_vgaboard_palette_1bpp_black      },
    { /* 1 */ .name = L"Amber CRT 2"        , .code = L"AMBER ", .palette = pico_vgaboard_palette_1bpp_amber      },
    { /* 2 */ .name = L"Amstrad CPC mode 2" , .code = L"CPC2  ", .palette = pico_vgaboard_palette_1bpp_cpc2       },
    { /* 3 */ .name = L"Green CRT 2"        , .code = L"GREEN ", .palette = pico_vgaboard_palette_1bpp_green      },
    { /* 4 */ .name = L"Black on white"     , .code = L"PAPER2", .palette = pico_vgaboard_palette_1bpp_paper      },
};
palette_t palettes2[] = {
    { /* 0 */ .name = L"White on black"     , .code = L"MONO4 ", .palette = pico_vgaboard_palette_2bpp_black      },
    { /* 1 */ .name = L"Amber CRT 4"        , .code = L"AMBER4", .palette = pico_vgaboard_palette_2bpp_amber      },
    { /* 2 */ .name = L"Amstrad CPC mode 1" , .code = L"CPC1  ", .palette = pico_vgaboard_palette_2bpp_cpc1       },
    { /* 3 */ .name = L"Green CRT 4"        , .code = L"GREEN4", .palette = pico_vgaboard_palette_2bpp_green      },
    { /* 4 */ .name = L"Grey/Gray 4"        , .code = L"GREY4 ", .palette = pico_vgaboard_palette_2bpp_grey       },
};
palette_t palettes4[] = {
    { /* 0 */ .name = L"Console 16"         , .code = L"CO16  ", .palette = pico_vgaboard_palette_4bpp_co16       },
    { /* 1 */ .name = L"Amstrad CPC mode 0" , .code = L"CPC0  ", .palette = pico_vgaboard_palette_4bpp_cpc0       },
    { /* 2 */ .name = L"ANSI 16"            , .code = L"ANSI16", .palette = pico_vgaboard_palette_4bpp_ansi       },
    { /* 3 */ .name = L"Atari STE"          , .code = L"STE   ", .palette = pico_vgaboard_palette_4bpp_atari_ste  },
    { /* 4 */ .name = L"Bubblegum 16"       , .code = L"BG16  ", .palette = pico_vgaboard_palette_4bpp_bg16       },
    { /* 5 */ .name = L"CGA"                , .code = L"CGA   ", .palette = pico_vgaboard_palette_4bpp_cga        },
    { /* 6 */ .name = L"Commodore 64"       , .code = L"C64   ", .palette = pico_vgaboard_palette_4bpp_c64        },
    { /* 7 */ .name = L"Dawnbringer 16"     , .code = L"DB16  ", .palette = pico_vgaboard_palette_4bpp_db16       },
    { /* 8 */ .name = L"Grey/Gray 16"       , .code = L"GREY16", .palette = pico_vgaboard_palette_4bpp_grey       },
    { /* 9 */ .name = L"Sweetie 16"         , .code = L"SW16  ", .palette = pico_vgaboard_palette_4bpp_sw16       },
};
palette_t palettes8[] = {
    { /* 0 */ .name = L"ANSI"               , .code = L"ANSI  ", .palette = pico_vgaboard_palette_8bpp_ansi       },
    { /* 1 */ .name = L"Aurora"             , .code = L"AURORA", .palette = pico_vgaboard_palette_8bpp_aurora     },
    { /* 2 */ .name = L"Commander X16"      , .code = L"CX16  ", .palette = pico_vgaboard_palette_8bpp_cx16       },
    { /* 3 */ .name = L"RGB 332"            , .code = L"RGB332", .palette = pico_vgaboard_palette_8bpp_rgb332     },
    { /* 4 */ .name = L"RGB 685"            , .code = L"RGB685", .palette = pico_vgaboard_palette_8bpp_rgb685     },
};
/* clang-format on */
#define N_PALETTES1 (sizeof(palettes1) / sizeof(palette_t))
#define N_PALETTES2 (sizeof(palettes2) / sizeof(palette_t))
#define N_PALETTES4 (sizeof(palettes4) / sizeof(palette_t))
#define N_PALETTES8 (sizeof(palettes8) / sizeof(palette_t))
palette_t *palette_table;
int palette_count;
int palette_index = 0;

/* "LIBS" for this demo (order is important) */
/* clang-format off */
#include "font.h"
#include "font.c"
#include "rect.c"
#include "title.c"
#include "vsync.c"
#include "borders-and-axis.c"
/* clang-format on */

/* Other global variables */
wchar_t *palette_name = L"Default";

/* DEMOS */
#include "bars.c"
#include "dblbuf.c"
#include "figures.c"
#include "fonts.c"
#include "images-4bpp.c"
#include "images-8bpp.c"
#include "images.c"
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
    int16_t duration_s;
    bool cumulative; // true if each frame is drawn over previous one, false if everything is redrawn at each frame
} demo_t;

/* clang-format off */
/** @brief Demo table */
demo_t demos[] = {
    // { .name = L"Minimal"            , .init = minimal_init      , .draw = minimal_draw      , .done = NULL              , .cumulative = true , .duration_s =  10 },
    { .name = L"Specifications"     , .init = specs_init        , .draw = specs_draw        , .done = NULL              , .cumulative = false, .duration_s =  10 },
    // { .name = L"Palette"            , .init = palette_init      , .draw = palette_draw      , .done = NULL              , .cumulative = false, .duration_s =  10 },
    // { .name = L"Double buffer test" , .init = dblbuf_init       , .draw = dblbuf_draw       , .done = dblbuf_done       , .cumulative = false, .duration_s =  10 },
    // { .name = L"Hollow figures"     , .init = figures_init      , .draw = figures_draw      , .done = NULL              , .cumulative = true , .duration_s =  10 },
    // { .name = L"Filled figures"     , .init = figures_init      , .draw = figures_fill      , .done = NULL              , .cumulative = true , .duration_s =  10 },
    // { .name = L"Bars"               , .init = bars_init         , .draw = bars_draw         , .done = NULL              , .cumulative = false, .duration_s =  10 },
    // { .name = L"Rectangles"         , .init = rects_init        , .draw = rects_draw        , .done = NULL              , .cumulative = true , .duration_s =  10 },
    // { .name = L"Fonts"              , .init = fonts_init        , .draw = fonts_draw        , .done = NULL              , .cumulative = false, .duration_s =  10 },
    // { .name = L"Images"             , .init = images_init       , .draw = images_draw       , .done = images_done       , .cumulative = false, .duration_s =  40 },
    // { .name = L"16 color sprites"   , .init = sprites_init      , .draw = sprites_draw      , .done = sprites_done      , .cumulative = false, .duration_s = 360 },
    // { .name = L"Scroller"           , .init = scroller_init     , .draw = scroller_draw     , .done = NULL              , .cumulative = true , .duration_s =  60 },
};
/* clang-format on */
#define N_DEMOS (sizeof(demos) / sizeof(demo_t))

/** @brief Current demo index */
int demo;
/** @brief Current demo title */

#include "buttons.c"

void screen_update()
{
#if PICO_VGABOARD_DOUBLE_BUFFER == 1
    if (demos[demo].cumulative)
    {
        // Copy previous image to working screen
        memcpy(
            pico_vgaboard->framebuffers[pico_vgaboard->framebuffer_index],
            pico_vgaboard->framebuffers[1 - pico_vgaboard->framebuffer_index],
            pico_vgaboard->framebuffer_size);
    }
    else
    {
        // Clear screen
        memset((uint8_t *)pico_vgaboard->framebuffer, 0, pico_vgaboard->framebuffer_size);
    }
#endif
    // Draw title?
    if (TITLE.h > 0)
    {
        clip(&TITLE);
        swprintf(
            title_text, sizeof(title_text) / sizeof(wchar_t),
            L" %d/%d %ls ",
            demo + 1, N_DEMOS, demos[demo].name);
        title_draw();
    }
    // Draw demo
    clip(&DEMO);
    demos[demo].draw();
    // Draw status?
    if (STATUS.h > 0)
    {
        clip(&STATUS);
        show_status();
    }
}

/**
 * @brief Cycle through demos
 *        (does not return)
 */
void example_demo_loop(void)
{
#if PICO_VGABOARD_DEBUG
    printf("*** EXAMPLE_%dX%dX%dBPP@%d ***\n", WIDTH, HEIGHT, DEPTH, pico_vgaboard->freq_hz);
#endif
    // init_windows(0, 0);
    // init_windows(0, 8);
    init_windows(8, 8);
    // init_windows( HEIGHT <= 192 ? 0 : HEIGHT <= 240 ? 8 : 16, 8);
    // draw_borders_and_axis(&FULL_SCREEN, 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1), 1 + rand() % (COLORS - 1));
    demo = 0;
    buttons_demo_first = false;
    buttons_demo_next = false;
    title_init();
    while (true)
    {
#if PICO_VGABOARD_DEBUG
        wprintf(L"Launching #%d of %d: %ls\r\n", demo, N_DEMOS, demos[demo].name);
#endif
        // Clear framebuffer between each demo
        memset((uint8_t *)pico_vgaboard->framebuffer, 0, pico_vgaboard->framebuffer_size);
        if (demos[demo].init())
        {
#if PICO_VGABOARD_DOUBLE_BUFFER == 1
            // Display in first framebuffer
            pico_vgaboard_framebuffer_flip();
            screen_update();
            // Display in second framebuffer
            pico_vgaboard_framebuffer_flip();
            screen_update();
#else
            wait_for_vblank();
            screen_update();
#endif
            clock_t demo_now_ms = get_time_ms();
            volatile clock_t demo_end_ms = demo_now_ms + demos[demo].duration_s * 1000;
            while ((demo_now_ms < demo_end_ms) && !buttons_demo_next && !buttons_demo_first)
            {
#if PICO_VGABOARD_DOUBLE_BUFFER == 1
                pico_vgaboard_framebuffer_flip();
#else
                wait_for_vblank();
#endif
                buttons_handle();
                screen_update();
                demo_now_ms = get_time_ms();
            }
        }
#if PICO_VGABOARD_DEBUG
        else
        {
            wprintf(L"FAILED launching of demo #%d of %d: %ls\r\n", demo, N_DEMOS, demos[demo].name);
        }
#endif
        if (demos[demo].done != NULL)
        {
            demos[demo].done();
        }
        if (buttons_demo_first)
        {
            // First demo
            demo = 0;
            buttons_demo_first = false;
        }
        else
        {
            // Next demo, be it by timer or by button press
            demo = (demo + 1) % N_DEMOS;
        }
        buttons_demo_next = false;
    }
}

/**
 * @brief Setup VGA mode, letterbox and HAGL
 */
void setup(const pico_vgaboard_t *vgaboard_model, uint16_t display_width, uint16_t display_height)
{
    stdio_init_all();
#if PICO_VGABOARD_DEBUG
    sleep_ms(100);
    printf("SETUP!\r\n");
#endif
    pico_vgaboard_init();
#if !PICO_NO_HARDWARE
    pico_vgaboard_buttons_init();
#endif
    pico_vgaboard_start(vgaboard_model, display_width, display_height, 0);
    hagl_backend = hagl_init();
}

int main(void)
{
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
    // setup(&pico_vgaboard_1024x384x1bpp       ,   0,   0); // KO, perf
    // setup(&pico_vgaboard_1024x768x1bpp_98304 ,   0,   0); // KO, perf

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_640x200x1bpp_16000  ,   0,   0); // OK
    // setup(&pico_vgaboard_640x400x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_840x525x1bpp_1        ,   0,   0); // OK
    // setup(&pico_vgaboard_840x525x1bpp_2        ,   0,   0); // OUT OF RANGE!

    /******************************** 5:4 RATIO *******************************/
    // setup(&pico_vgaboard_640x512x1bpp        ,   0,   0); // OK ()
    // setup(&pico_vgaboard_640x512x1bpp_2        ,   0,   0); // OK

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_640x360x1bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_1024x576x1bpp_73728 ,   0,   0); // ?
    // setup(&pico_vgaboard_1024x576x1bpp_73728 ,   0,   0); // ?
    // setup(&pico_vgaboard_1280x720x1bpp_115200,   0,   0); // KO, perf

    /**************************************************************************/
    /* 2BPP - 4 COLORS                                                        */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_384x576x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_512x384x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_512x384x2bpp        , 480, 272); // OK (16:9 letterbox as 2x scale of TIC-80)
    // setup(&pico_vgaboard_640x240x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_320x480x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_800x300x2bpp        , 0  ,   0); // OK

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_320x200x2bpp_16000  , 0  ,   0); // OK
    // setup(&pico_vgaboard_640x200x2bpp        , 0  ,   0); // OK
    // setup(&pico_vgaboard_640x400x2bpp_64000  , 0  ,   0); // OK

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_512x288x2bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_1024x576x2bpp_147456, 960, 540); // OK

    /**************************************************************************/
    /* 4BPP - 16 COLORS                                                       */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_256x192x4bpp_24576_1,   0,   0); // OK (1024x768 based)
    // setup(&pico_vgaboard_256x192x4bpp_24576_1, 240, 136); // OK (1024x768 based)
    // setup(&pico_vgaboard_256x192x4bpp_24576_2,   0,   0); // OK (768x576 based)
    // setup(&pico_vgaboard_256x192x4bpp_24576_2, 240, 136); // OK (768x576 based)
    // setup(&pico_vgaboard_320x240x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x240x4bpp        , 320, 200); // OK (so we have 320x200@60 in a standard mode)
    // setup(&pico_vgaboard_320x240x4bpp        , 256, 192); // OK
    // setup(&pico_vgaboard_384x288x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_384x288x4bpp        , 224, 256); // OK (Space Invaders rulez ;-))
    // setup(&pico_vgaboard_384x288x4bpp        , 224, 288); // OK (Pac-man rulez ;-))
    // setup(&pico_vgaboard_384x288x4bpp        , 320, 200); // OK (768x576 based)
    // setup(&pico_vgaboard_384x288x4bpp        , 320, 240); // OK
    // setup(&pico_vgaboard_400x300x4bpp        , 320, 240); // OK
    // setup(&pico_vgaboard_512x384x4bpp_98304  ,   0,   0); // OK
    // setup(&pico_vgaboard_512x384x4bpp_98304  , 480, 272); // OK (2x scale of TIC-80 => 65280 bytes framebuffer)
    // setup(&pico_vgaboard_640x480x4bpp_153600 , 480, 272); // OK (2x scale of TIC-80 => 65280 bytes framebuffer)
    // setup(&pico_vgaboard_800x600x4bpp        , 640, 480); // OK 

    /******************************** 5:4 RATIO *******************************/
    // setup(&pico_vgaboard_320x256x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x256x4bpp        , 224, 256); // OK (Space Invaders rulez ;-), again)
    // setup(&pico_vgaboard_320x256x4bpp_2      ,   0,   0); // OK
    // setup(&pico_vgaboard_320x256x4bpp_2      , 288, 224); // OK, sort of, no bottom border on my LG monitor...

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_640x360x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x360x4bpp        , 576, 324); // OK
    // setup(&pico_vgaboard_640x360x4bpp        , 608, 328); // OK
    // setup(&pico_vgaboard_1024x576x4bpp_294912, 640, 400); // OK, too much margins

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_160x200x4bpp_16000  ,   0,   0); // OK
    // setup(&pico_vgaboard_256x144x4bpp_18432_1,   0,   0); // OK
    // setup(&pico_vgaboard_256x144x4bpp_18432_1, 240, 136); // OK
    // setup(&pico_vgaboard_320x100x4bpp_16000  ,   0,   0); // OK (not very interesting...)
    // setup(&pico_vgaboard_320x180x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x180x4bpp        , 240, 136); // OK
    // setup(&pico_vgaboard_320x200x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x200x4bpp        , 240, 136); // OK
    // setup(&pico_vgaboard_320x360x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x400x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_256x384x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_512x144x4bpp        ,   0,   0); // OK (sort of: 144 lines is not enough...)
    // setup(&pico_vgaboard_256x288x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_512x192x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_512x384x4bpp_98304  , 512, 288); // OK (1024x768 based, 16:9)
    setup(&pico_vgaboard_560x350x4bpp_1      , 480, 272); // OK? '(1680x150 based, 16:9ish)
    // setup(&pico_vgaboard_640x180x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x200x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x256x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x400x4bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_640x400x4bpp        , 480, 272); // OK
    // Experimentation around 1680x1050...
    // setup(&pico_vgaboard_840x525x4bpp_1      , 480, 272); // OK (not centered on my LG monitor)
    // setup(&pico_vgaboard_840x525x4bpp_2      , 480, 272); // OK?

    /**************************************************************************/
    /* 8BPP - 256 COLORS                                                      */
    /**************************************************************************/

    /******************************** 4:3 RATIO *******************************/
    // setup(&pico_vgaboard_160x240x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_192x288x8bpp        ,   0,   0); // KO
    // setup(&pico_vgaboard_256x192x8bpp_1      ,   0,   0); // OK (1024x768 based)
    // setup(&pico_vgaboard_256x192x8bpp_1      , 240, 136); // OK (1024x768 based, TIC-80, 16:9ish)
    // setup(&pico_vgaboard_256x192x8bpp_1      , 256, 144); // OK (1024x768 based, 16:9)
    // setup(&pico_vgaboard_256x192x8bpp_49152_2,   0,   0); // OK (768x576 based)
    // setup(&pico_vgaboard_256x192x8bpp_49152_2, 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)
    // setup(&pico_vgaboard_320x240x8bpp_76800  ,   0,   0); // OK (/!\ 76,800 bytes framebuffer /!\)
    // setup(&pico_vgaboard_320x240x8bpp_76800  , 256, 224); // OK (NES letterbox)
    // setup(&pico_vgaboard_320x240x8bpp        , 320, 200); // OK (so we have 320x200@60 in a standard mode)
    // setup(&pico_vgaboard_320x240x8bpp        , 256, 192); // OK
    // setup(&pico_vgaboard_384x144x8bpp        ,   0,   0); // KO after a few seconds
    // setup(&pico_vgaboard_384x288x8bpp_110592 ,   0,   0); // ?
    // setup(&pico_vgaboard_640x480x8bpp_307200 , 480, 272); // OK (2x scale of TIC-80 => 130560 bytes framebuffer)

    /******************************** 5:4 RATIO *******************************/
    // setup(&pico_vgaboard_320x256x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x256x8bpp_2      ,   0,   0); // OK
    // setup(&pico_vgaboard_320x256x8bpp_2      , 288, 224); // OK, sort of, no bottom border on my LG monitor...

    /******************************* 16:10 RATIO ******************************/
    // setup(&pico_vgaboard_160x200x8bpp        ,   0,   0); // OK
    // setup(&pico_vgaboard_320x200x8bpp        ,   0,   0); // OK

    /******************************* 16:9 RATIO *******************************/
    // setup(&pico_vgaboard_256x144x8bpp        ,   0,   0); // OK (16:9 letterbox as 1x scale of TIC-80)
    // setup(&pico_vgaboard_256x144x8bpp        , 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)
    // setup(&pico_vgaboard_320x180x8bpp        , 240, 136); // OK (16:9 letterbox as 1x scale of TIC-80)

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // Seed RNG with UNIX time
    srand(time(NULL));
#endif

    if (pico_vgaboard->has_margins){
        /* Random border colors in letterbox mode instead of default black ones */
        pico_vgaboard->border_color_top    = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
        pico_vgaboard->border_color_left   = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
        pico_vgaboard->border_color_bottom = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
        pico_vgaboard->border_color_right  = (rand() % 65536) & ~PICO_SCANVIDEO_ALPHA_MASK;
    }

    /* clang-format on */

    switch (DEPTH)
    {
    case 1:
        palette_table = palettes1;
        palette_count = N_PALETTES1;
        break;
    case 2:
        palette_table = palettes2;
        palette_count = N_PALETTES2;
        break;
    case 4:
        palette_table = palettes4;
        palette_count = N_PALETTES4;
        break;
    case 8:
        palette_table = palettes8;
        palette_count = N_PALETTES8;
        break;
    default:
        panic("NO PALETTES for DEPTH %d!!!", DEPTH);
        break;
    }
    pico_vgaboard_set_palette(palette_table[palette_index].palette);
    palette_name = palette_table[palette_index].name;

#if PICO_VGABOARD_DEBUG
    printf("*** CORE1 => RENDER LOOP ***\n");
#endif
    multicore_launch_core1(pico_vgaboard_render_loop);
#if PICO_VGABOARD_DEBUG
    printf("*** CORE0 => DEMO ***\n");
#endif
    example_demo_loop();

    __builtin_unreachable();
#if PICO_VGABOARD_DEBUG
    printf("*** UNREACHABLE ***\n");
#endif
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
