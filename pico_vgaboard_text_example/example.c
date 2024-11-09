/* SPDX-License-Identifier: MIT */

// Standard libs
#include <stdint.h>
#include <inttypes.h>
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

// Pico VGA Board
#include "pico-vgaboard.h"
#include "colors.h"
#include "palettes/palettes.h"
#include "palettes/dawnbringer16.h"
// #include "modes/640x400.h"
// #include "modes/640x480.h"
#include "modes/1024x768.h"
// #include "modes/1680x1050.h"
#include "pico-vgaboard.h"
#include "pico-vgaboard-console.h"
#include "pico-vgaboard-framebuffer.h"

// #define VGA_MODE (&pico_vgaboard_320x200_70)
// #define VGA_MODE (&pico_vgaboard_640x480_60)
#define VGA_MODE (&pico_vgaboard_512x384_60)
// #define VGA_MODE (&pico_vgaboard_320x240_60)
// #define VGA_MODE (&pico_vgaboard_336x210_60)
// #define VGA_MODE (&pico_vgaboard_256x192_60)
#define VGA_WIDTH (VGA_MODE->width)
#define VGA_HEIGHT (VGA_MODE->height)
#define FB_DEPTH (4)
#define FB_WIDTH (320)
#define FB_HEIGHT (240)
#define FB_DOUBLE_BUFFER (false)
#define FB_BORDER (PICO_SCANVIDEO_PIXEL_FROM_RGB5(0x0, 0x0, 0x0))
// #define COLS (FB_WIDTH / 8)
// #define ROWS (FB_HEIGHT / 8)
#define COLS (512 / 8)
#define ROWS (384 / 8)

// Poor man's alignment to 32 bits...
uint32_t PICO_VGABOARD_DATA _fb0[(FB_WIDTH * FB_HEIGHT / 2) / 4];
uint8_t *fb0 = (uint8_t *)_fb0;
// uint32_t PICO_VGABOARD_DATA _fb1[(FB_WIDTH * FB_HEIGHT / 2) / 4];
// uint8_t *fb1 = (uint8_t *)_fb1;
uint8_t *fb1 = NULL;

// Always use framebuffer through pointer with "->"
pico_vgaboard_framebuffer_t PICO_VGABOARD_DATA _framebuffer;
pico_vgaboard_framebuffer_t PICO_VGABOARD_DATA *fb = &_framebuffer;

// Allocate static buffer
t_pvga_console_cell PICO_VGABOARD_DATA console_buffer[COLS * ROWS * sizeof(t_pvga_console_cell)] = {0};
// Always use console through pointer with "->"
t_pvga_console PICO_VGABOARD_DATA _console = {.allocated = false};
t_pvga_console PICO_VGABOARD_DATA *console = &_console;

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
    for (int i = 0; i < VGA_WIDTH / 2; i += 1) // there are 2 16 bits pixels in each 32 bits word
    {
        ++scanline_colors;
        // *scanline_colors = 0x12345678;
        if (i % 16 < 8)
            *scanline_colors = ((palette_4bpp_ansi[i & 15] | PICO_SCANVIDEO_ALPHA_MASK) << 16) |
                               ((palette_4bpp_ansi[15 - (i & 15)] | PICO_SCANVIDEO_ALPHA_MASK));
        else
            *scanline_colors = ((0) << 16) | (0);
        // *scanline_colors = (rand() & 0xffff0000) | (PICO_SCANVIDEO_ALPHA_MASK);
        // *scanline_colors = rand();
    }
    ++scanline_colors;
    // scanline end
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (VGA_WIDTH - 2);
    data_used = (VGA_WIDTH + 4) / 2; // 2 16 bits pixels in each 32 bits word
    return data_used;
}

// volatile uint32_t counter2 = 0;

// void custom_init2(void *plane_state)
// {
//     printf("*** custom_init2! ***\n");
// }

// uint16_t __not_in_flash("pico_vgaboard_code")(custom_render_scanline2)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max)
// {
//     counter2 += 1;
//     return custom_render_scanline(plane_state, scanline_number, data, data_max, 16, 64);
// }

volatile uint32_t counter3 = 0;

void custom_init3(void *plane_state)
{
    printf("*** custom_init3! ***\n");
}

uint16_t __not_in_flash("pico_vgaboard_code")(custom_render_scanline3)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max)
{
    counter3 += 1;
    return custom_render_scanline(plane_state, scanline_number, data, data_max, 128, 64);
}

void main(void)
{
    stdio_init_all();

    pico_vgaboard_init();

    // Initialize framebuffer at plane #1
    pico_vgaboard_framebuffer_init(
        fb, 0,
        fb0, fb1, FB_DOUBLE_BUFFER,
        FB_DEPTH,
        (uint16_t *)palette_4bpp_db16,
        VGA_WIDTH, VGA_HEIGHT,
        FB_WIDTH, FB_HEIGHT,
        FB_BORDER);
    printf("INIT framebuffer w=%d h=%d size=%d...\n", fb->window_width, fb->window_height, fb->framebuffer_size);

    // Initialize console at plane #2
    pvga_console_init(console, 1, COLS, ROWS, palette_4bpp_ansi, 0b00001111, console_buffer);
    // pico_vgaboard_init_plane(1, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    // Set plane #3 as unused
    // pico_vgaboard_init_plane(2, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    // pico_vgaboard_init_plane(1, PICO_VGABOARD_PLANE_CUSTOM, 0, NULL, &custom_init2, &custom_render_scanline2);
    // pico_vgaboard_init_plane(2, PICO_VGABOARD_PLANE_CUSTOM, 0, NULL, &custom_init3, &custom_render_scanline3);

    // Initialize VGA with our planes
    pico_vgaboard_start(VGA_MODE);

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // Seed RNG with UNIX time
    srand(time(NULL));
#endif

    uint8_t row, col;
    uint8_t c;

    printf("BEFORE framebuffer w=%d h=%d size=%d...\n", fb->window_width, fb->window_height, fb->framebuffer_size);
    memset(fb->framebuffer, DB16_GREEN << 4 | DB16_LIGHT_YELLOW, fb->framebuffer_size);
    uint16_t x, y;
    printf("AFTER framebuffer...\n");

    printf("BEFORE console...\n");
    pvga_console_dump_settings(console);

    pvga_console_clear(console);

    pvga_console_set_attributes(console, PVGA_CONSOLE_TRANSPARENT);
    pvga_console_set_background(console, 0);
    pvga_console_set_foreground(console, 15);
    pvga_console_move_cursor_to(console, 0, 0); // console->rows * 1 / 4, console->cols / 4);
    pvga_console_put_string(console, "1: Hello, world!");

    pvga_console_set_background(console, 0);
    pvga_console_set_foreground(console, 7);
    pvga_console_move_cursor_to(console, 1, 0); // console->rows * 2 / 4, console->cols / 4);
    pvga_console_set_attributes(console, PVGA_CONSOLE_REVERSE);
    pvga_console_put_string(console, "2: Hello, ");
    pvga_console_set_attributes(console, PVGA_CONSOLE_NONE);
    pvga_console_put_string(console, "world!");

    pvga_console_dump_settings(console);
    printf("AFTER console...\n");

    // Start VGA rendering
    printf("BEFORE render loop...\n");
    multicore_launch_core1(pico_vgaboard_render_loop);
    printf("BEFORE render loop...\n");

    uint64_t counter = 0;
    pvga_console_set_attributes(console, PVGA_CONSOLE_TRANSPARENT);
    pvga_console_move_cursor_to(console, 0, 0);
    pvga_console_set_background(console, 0);
    while (true)
    {
        pico_vgaboard_wait_for_vsync();
        for (size_t i = 0; i < 2; i++)
        {
            x = 8 + rand() % (fb->window_width - 16);
            y = 8 + rand() % (fb->window_height - 16);
            c = rand() % 16;
            for (int i = -4; i < 4; i++)
            {
                for (int j = -4; j < 4; j++)
                {
                    pico_vgaboard_framebuffer_put_pixel(fb, x + i, y + j, c);
                }
            }
        }
        pvga_console_set_foreground(console, 1 + counter % 15);
        pvga_console_put_char(console, counter % 256);
        counter += 1;
        if (counter % 1000 == 0)
        {
            printf("pvga_console_render_scanline: min=%d, max=%d, core=%d\n",
                   pvga_console_render_scanline_min,
                   pvga_console_render_scanline_max,
                   pvga_console_render_scanline_core);
        }
        // if (counter2 > 0xffff)
        // {
        //     counter2 = 0;
        //     printf("2");
        // }
        // if (counter3 > 0xffff)
        // {
        //     counter3 = 0;
        //     printf("3");
        // }
    }

    __builtin_unreachable();
}
