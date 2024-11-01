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
#include "pico/scanvideo/scanvideo_base.h"

// Pico VGA Board
#include "pico-vgaboard.h"
#include "colors.h"
#include "palettes/palettes.h"
#include "palettes/dawnbringer16.h"
#include "modes/640x400.h"
#include "modes/640x480.h"
#include "modes/1680x1050.h"
#include "pico-vgaboard.h"
#include "pico-vgaboard-console.h"
#include "pico-vgaboard-framebuffer.h"

// #define VGA_MODE (&pico_vgaboard_320x200_70)
// #define VGA_MODE (&pico_vgaboard_640x480_60)
// #define VGA_MODE (&pico_vgaboard_320x240_60)
#define VGA_MODE (&pico_vgaboard_336x210_60)
#define VGA_WIDTH (VGA_MODE->width)
#define VGA_HEIGHT (VGA_MODE->height)
#define FB_DEPTH (4)
#define FB_WIDTH (320)
#define FB_HEIGHT (200)
#define FB_DOUBLE_BUFFER (false)
#define FB_BORDER (PICO_SCANVIDEO_PIXEL_FROM_RGB5(0xf, 0xf, 0xf))
#define COLS (FB_WIDTH / 8)
#define ROWS (FB_HEIGHT / 8)

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
t_pvga_console_cell PICO_VGABOARD_DATA console_buffer[COLS * ROWS * sizeof(t_pvga_console_cell)] = {};
// Always use console through pointer with "->"
t_pvga_console PICO_VGABOARD_DATA _console = {.allocated = false};
t_pvga_console PICO_VGABOARD_DATA *console = &_console;

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
    // TODO! initializing console after framebuffer overwrites parts of it...
    pvga_console_init(console, 1, COLS, ROWS, palette_4bpp_ansi, 0b1111, console_buffer);
    // pico_vgaboard_init_plane(1, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    // Set plane #3 as unused
    // pico_vgaboard_init_plane(2, PICO_VGABOARD_PLANE_NONE, 0, NULL, NULL, NULL);

    // Initialize VGA with our planes
    pico_vgaboard_start(VGA_MODE);

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // Seed RNG with UNIX time
    srand(time(NULL));
#endif

    // Start VGA rendering
    printf("BEFORE render loop...\n");
    multicore_launch_core1(pico_vgaboard_render_loop);
    printf("BEFORE render loop...\n");

    uint8_t row, col;
    uint8_t c;

    printf("BEFORE framebuffer w=%d h=%d size=%d...\n", fb->window_width, fb->window_height, fb->framebuffer_size);
    memset(fb->framebuffer, DB16_GREEN << 4 | DB16_LIGHT_YELLOW, fb->framebuffer_size);
    uint16_t x, y;
    printf("AFTER framebuffer...\n");

    // printf("BEFORE console...\n");
    // pvga_console_dump(console);

    pvga_console_clear(console);

    // pvga_console_set_attributes(console, PVGA_CONSOLE_TRANSPARENT);
    // pvga_console_set_background(console, DB16_BROWN);
    // pvga_console_set_foreground(console, DB16_LIGHT_PINK);
    // pvga_console_move_cursor_to(console, console->rows / 2, console->cols / 4);
    // pvga_console_put_string(console, "1: Hello, world!");
    // pvga_console_move_cursor_to(console, 1 + console->rows / 2, 1 + console->cols / 4);
    // pvga_console_put_string(console, "1: Hello, world!");

    // pvga_console_set_attributes(console, PVGA_CONSOLE_TRANSPARENT | PVGA_CONSOLE_REVERSE);
    // pvga_console_set_background(console, DB16_DARK_GREY);
    // pvga_console_set_foreground(console, DB16_LIGHT_YELLOW);
    // pvga_console_move_cursor_to(console, console->rows / 2 + 1, console->cols / 2);
    // pvga_console_put_string(console, "2: Hello, world!");
    // pvga_console_move_cursor_to(console, 1 + console->rows / 2 + 1, 1 + console->cols / 2);
    // pvga_console_put_string(console, "2: Hello, world!");

    // pvga_console_dump(console);
    // printf("AFTER console...\n");

    uint64_t counter = 0;
    while (true)
    {
        for (size_t i = 0; i < 8; i++)
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
            // pico_vgaboard_framebuffer_put_pixel(fb, x - 1, y - 1, c);
            // pico_vgaboard_framebuffer_put_pixel(fb, x - 1, y + 1, c);
            // pico_vgaboard_framebuffer_put_pixel(fb, x + 0, y + 0, c);
            // pico_vgaboard_framebuffer_put_pixel(fb, x + 1, y - 1, c);
            // pico_vgaboard_framebuffer_put_pixel(fb, x + 1, y + 1, c);
        }
        // x = rand() % fb->window_width;
        // y = rand() % fb->window_height;
        // c = rand() % 16;
        // pico_vgaboard_framebuffer_put_pixel(fb, x - 1, y - 1, c);
        // pico_vgaboard_framebuffer_put_pixel(fb, x - 1, y + 1, c);
        // pico_vgaboard_framebuffer_put_pixel(fb, x + 0, y + 0, c);
        // pico_vgaboard_framebuffer_put_pixel(fb, x + 1, y - 1, c);
        // pico_vgaboard_framebuffer_put_pixel(fb, x + 1, y + 1, c);
        // tight_loop_contents();
        // pico_vgaboard_flash_led_and_wait();
        // row = rand() % ROWS;
        // col = rand() % COLS;
        // c = 32 + rand() % 95; // ASCII printable char
        // pico_vgaboard_framebuffer_put_pixel(fb, col * 8 + rand() % 8, row * 8 + rand() % 8, c % 16);
        // pvga_console_put_char_at(console, row, col, c);
        // counter += 1;
        // if (counter % 100000 == 0)
        // {
        // printf(".");
        // printf("%" PRIu64 "\n", pvga_console_render_scanline_count);
        // for (uint8_t r = 0; r <= console->rows; r += 1)
        // {
        //     printf("%03d ", r);
        //     for (uint8_t c = 0; c <= console->cols; c += 1)
        //     {
        //         char ch = console->buffer[r * console->cols + c]->ch;
        //         printf("%c ", c >= 32 && c <= 126 ? c : '.');
        //     }
        //     printf("\r\n");
        // }
        pico_vgaboard_wait_for_vsync();
    }

    __builtin_unreachable();
}
