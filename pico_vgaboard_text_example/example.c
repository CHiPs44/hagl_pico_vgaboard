/* SPDX-License-Identifier: MIT */

// Standard C libs
#include <inttypes.h>
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

// Pico Extras
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"

// Pico VGA board
#include "colors.h"
// #include "modes/1024x768.h"
// #include "modes/1280x1024.h"
// #include "modes/1680x1050.h"
// #include "modes/640x400.h"
#include "modes/640x480.h"
#include "palettes/dawnbringer16.h"
#include "palettes/palettes.h"
#include "pico-vgaboard-console.h"
#include "pico-vgaboard-framebuffer.h"
#include "pico-vgaboard.h"

// #define VGA_MODE (&pico_vgaboard_256x192_60)
// #define VGA_WIDTH (256)
// #define VGA_HEIGHT (192)

// #define VGA_MODE (&pico_vgaboard_512x384_60)
// #define VGA_WIDTH (512)
// #define VGA_HEIGHT (384)

// #define VGA_MODE (&pico_vgaboard_320x200_70)
// #define VGA_WIDTH (320)
// #define VGA_HEIGHT (200)

// #define VGA_MODE (&pico_vgaboard_320x256_60)
// #define VGA_WIDTH (320)
// #define VGA_HEIGHT (256)

#define VGA_MODE (&pico_vgaboard_320x240_60)
#define VGA_WIDTH (320)
#define VGA_HEIGHT (240)

// #define VGA_MODE (&pico_vgaboard_336x210_60)
// #define VGA_MODE (&pico_vgaboard_512x256_60)
// #define VGA_MODE (&pico_vgaboard_640x480_60)
// #define VGA_MODE (&pico_vgaboard_1024x768_60)
// #define VGA_WIDTH (VGA_MODE->width)
// #define VGA_HEIGHT (VGA_MODE->height)

// #define VGA_MODE (&pico_vgaboard_512x384_60)
// #define VGA_WIDTH (512)
// #define VGA_HEIGHT (384)

#define FB_WIDTH VGA_WIDTH
#define FB_HEIGHT VGA_HEIGHT

// #define FB_WIDTH (320)
// #define FB_HEIGHT (240)

#define COLS (FB_WIDTH / 8)
#define ROWS (FB_HEIGHT / 8)

// Poor man's alignment to 32 bits...
uint32_t PICO_VGABOARD_DATA _fb0[(FB_WIDTH * FB_HEIGHT / 2) / 4];
uint8_t *fb0 = (uint8_t *)_fb0;
uint32_t PICO_VGABOARD_DATA _fb1[(FB_WIDTH * FB_HEIGHT / 2) / 4];
uint8_t *fb1 = (uint8_t *)_fb1;

// Always use framebuffer through pointer with "->"
pico_vgaboard_framebuffer PICO_VGABOARD_DATA _fb;
pico_vgaboard_framebuffer PICO_VGABOARD_DATA *fb = &_fb;

// Allocate static buffer for console
console_cell PICO_VGABOARD_DATA console_buffer[COLS * ROWS * sizeof(console_cell)] = {0};
// Always use console through pointer with "->"
console_state PICO_VGABOARD_DATA _console;
console_state PICO_VGABOARD_DATA *console = &_console;
char console_status[256];

void main(void)
{
    stdio_init_all();
    pico_vgaboard_init();

    /*
     * Initialize framebuffer on plane #0
     * ==================================
     */
    pico_vgaboard_framebuffer_init(
        fb, 0,
        fb0, fb1, true,
        4, (uint16_t *)palette_4bpp_db16,
        // 4, (uint16_t *)palette_4bpp_atari_ste,
        // 8, (uint16_t *)palette_8bpp_ansi,
        VGA_WIDTH, VGA_HEIGHT,
        FB_WIDTH, FB_HEIGHT,
        PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0, 0x1f, 0x0));
    // Set plane #0 as unused
    // pico_vgaboard_init_plane(0, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    /*
     * Initialize console on plane #1
     * ==============================
     */
    if (console != NULL)
    {
        console_init(console, 1,
                     VGA_WIDTH, VGA_HEIGHT,
                     (VGA_HEIGHT - FB_HEIGHT) / 2, (VGA_HEIGHT - FB_HEIGHT) / 2,
                     (VGA_WIDTH - FB_WIDTH) / 2, (VGA_WIDTH - FB_WIDTH) / 2,
                     palette_4bpp_ansi,
                     COLS, ROWS, console_buffer);
    }
    else
    {
        // Set plane #1 as unused
        pico_vgaboard_init_plane(1, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);
    }

    /*
     * Display "something" on plane #2
     * ===============================
     */
    // pico_vgaboard_init_plane(2, PICO_VGABOARD_PLANE_CUSTOM, 0, NULL, &custom_init3, &custom_render_scanline3);
    // Set plane #2 as unused
    pico_vgaboard_init_plane(2, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    printf("********************************************************************************\n");
    pico_vgaboard_dump(pico_vgaboard);
    printf("********************************************************************************\n");

    // Initialize VGA with our planes
    pico_vgaboard_start(VGA_MODE);

#if !PICO_NO_HARDWARE
    // Pico: seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // SDL2: seed RNG with UNIX time
    srand(time(NULL));
#endif

    uint8_t row, col;
    uint8_t c;
    uint16_t x, y;
    uint8_t cursor_row, cursor_col;

    printf("BEFORE framebuffer depth=%d, colors=%d, w=%d h=%d size=%d...\n",
           fb->flags.depth, fb->colors,
           fb->window_width, fb->window_height, fb->framebuffer_size);
    // memset(fb->framebuffer, DB16_GREEN << 4 | DB16_LIGHT_YELLOW, fb->framebuffer_size);
    memset(fb->framebuffer, DB16_BLACK << 4 | DB16_BLACK, fb->framebuffer_size);

    if (console != NULL)
    {
        printf("BEFORE console...\n");
        console_clear(console);
        console_dump_settings(console);
        console_move_cursor_to(console, 1, 1);
        console_put_string(console, "Hello,world!");
        printf("AFTER console...\n");
    }

    // Start VGA rendering
    multicore_launch_core1(pico_vgaboard_render_loop);

    if (console != NULL)
    {
        console_move_cursor_to(console, 0, 0);
        console_hide_cursor(console);
    }

    // height3 = VGA_HEIGHT / 8;
    // delta3 = 1;
    while (true)
    {
        // just wait for vertical sync so we can update framebuffer without tearing
        // ------------------------------------------------------------------------
        pico_vgaboard_wait_for_vsync();

        // for (x = 0; x < fb->window_width; x++)
        // {
        //     c = 1 + (x + pico_vgaboard->frame_counter) % (fb->colors - 1);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 0 / 8 - 0, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 1 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 2 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 3 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 4 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 5 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 6 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 7 / 8 - 1, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, x, fb->window_height * 8 / 8 - 1, c);
        // }
        // for (y = 0; y < fb->window_height; y++)
        // {
        //     c = 1 + (y + pico_vgaboard->frame_counter) % (fb->colors - 1);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 0 / 8 - 0, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 1 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 2 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 3 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 4 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 5 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 6 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 7 / 8 - 1, y, c);
        //     pico_vgaboard_framebuffer_put_pixel(fb, fb->window_width * 8 / 8 - 1, y, c);
        // }

        // put some pixels on plane #0
        // ---------------------------
        for (int i = 0; i < 2; i++)
        {
            x = rand() % COLS; //(fb->window_width - 8);
            y = rand() % ROWS; //(fb->window_height - 8);
            c = rand() % 16;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    pico_vgaboard_framebuffer_put_pixel(fb, 8 * x + i, 8 * y + j, c);
                }
            }
        }

        if (console != NULL)
        {
            // display some text on plane #1
            // -----------------------------
            console_set_background(console, pico_vgaboard->frame_counter % 16 ? 15 : 0);
            // console_set_foreground(console, 1 + pico_vgaboard->frame_counter % 15);
            console_set_foreground(console, pico_vgaboard->frame_counter % 16);
            console_set_transparent(console, true);
            console_set_reverse(console, pico_vgaboard->frame_counter % 2 == 0);
            console_put_char(console, pico_vgaboard->frame_counter % 256);
            console_put_char(console, ' ');
        }
        //  move plane #2 every x frames
        // -----------------------------
        // start3 += delta3;
        // if (start3 + height3 > VGA_HEIGHT)
        // {
        //     start3 = VGA_HEIGHT - height3;
        //     delta3 = -delta3;
        // }
        // else if (start3 < 0)
        // {
        //     start3 = 0;
        //     delta3 = -delta3;
        // }

        if (console != NULL)
        {
            // show stats
            // ----------
            sprintf(console_status,
                    "\xb0\xb1\xb2 \x01\x02\xdb #%08lld %04dx%04dx%01d (%06d) %04dx%04d %03dx%03d \xdb\x02\x01 \xb2\xb1\xb0",
                    pico_vgaboard->frame_counter,
                    fb->screen_width, fb->screen_height, fb->flags.depth, fb->framebuffer_size,
                    fb->window_width, fb->window_height,
                    console->cols, console->rows);
            console_get_cursor_pos(console, &cursor_row, &cursor_col);
            console_set_reverse(console, true);
            console_set_background(console, 15);
            console_set_foreground(console, 0);
            console_move_cursor_to(console, ROWS - 1, 0);
            console_put_string(console, console_status);
            console_move_cursor_to(console, cursor_row, cursor_col);
        }
        if (fb->debug_message[0] != '\0')
        {
            // printf("fb.debug: %s\n", fb->debug);
            fb->debug_message[0] = '\0';
        }
        if (console->debug_message[0] != '\0')
        {
            // printf("console.debug: %s\n", console->debug);
            console->debug_message[0] = '\0';
        }
    }

    __builtin_unreachable();
}

uint16_t __not_in_flash("pico_vgaboard_code")(custom_render_scanline)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max, uint16_t start, uint16_t height)
{
    if (scanline_number < start || scanline_number >= start + height)
    {
        data[0] = COMPOSABLE_RAW_1P | (0 << 16);
        data[1] = COMPOSABLE_EOL_SKIP_ALIGN;
        return 2;
    }
    uint16_t data_used;
    uint32_t *scanline_colors = data;
    int color_index = 0;
    for (int i = 0; i < VGA_WIDTH / 2; i += 1) // there are 2 16 bits pixels in each 32 bits word
    {
        if (i % 2 == 0)
        {
            *++scanline_colors = ((palette_4bpp_ansi[color_index] | PICO_SCANVIDEO_ALPHA_MASK) << 16) |
                                 ((palette_4bpp_ansi[color_index] | PICO_SCANVIDEO_ALPHA_MASK));
        }
        else
        {
            *++scanline_colors = ((0) << 16) | (0);
        }
        if (i > 0 && i % (VGA_WIDTH / 32) == 0)
        {
            color_index += 1;
            if (color_index > 15)
                color_index = 0;
        }
    }
    // scanline end
    *++scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (VGA_WIDTH - 2);
    data_used = (VGA_WIDTH + 4) / 2; // 2 16 bits pixels in each 32 bits word
    return data_used;
}

// uint64_t counter3 = 0;
int16_t start3;
uint16_t height3;
int16_t offset3;
int16_t delta3;

void custom_init3(void *plane_state)
{
    printf("*** custom_init3! ***\n");
}

uint16_t __not_in_flash("pico_vgaboard_code")(custom_render_scanline3)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max)
{
    return custom_render_scanline(plane_state, scanline_number, data, data_max, start3, height3);
}
