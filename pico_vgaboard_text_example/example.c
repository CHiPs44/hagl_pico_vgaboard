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
#include "modes/640x480.h"
#include "modes/1680x1050.h"

#include "pico-vgaboard-console.h"

#define VGA_MODE (&pico_vgaboard_336x210x4bpp_1)
// #define VGA_WIDTH (VGA_MODE->scanvideo_mode->width)
// #define VGA_HEIGHT (VGA_MODE->scanvideo_mode->height)
#define VGA_WIDTH (320)
#define VGA_HEIGHT (200)
#define VGA_BORDER (PICO_SCANVIDEO_PIXEL_FROM_RGB5(0xf, 0xf, 0xf))
#define COLS (VGA_WIDTH / 8)
#define ROWS (VGA_HEIGHT / 8)

t_pvga_console *console;

void main(void)
{
    stdio_init_all();
    pico_vgaboard_init(false);
    console = pvga_console_init(COLS, ROWS);
    pico_vgaboard->render_scanline_plane2 = pvga_console_render_scanline;
    pico_vgaboard->plane2_state = console;
    pico_vgaboard_start(VGA_MODE, VGA_WIDTH, VGA_HEIGHT, VGA_BORDER);
    pico_vgaboard_set_palette(palette_4bpp_ansi);

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // Seed RNG with UNIX time
    srand(time(NULL));
#endif

    multicore_launch_core1(pico_vgaboard_render_loop);

    uint64_t counter = 0;
    uint8_t row, col;
    uint8_t c;

    while (true)
    {
        // sleep_ms(100);
        row = rand() % ROWS;
        col = rand() % COLS;
        c = 32 + rand() % 95;
        // printf("row=%d, col=%d, c=%d\n", row, col, c);
        pico_vgaboard_put_pixel(col * 8 + rand() % 8, row * 8 + rand() % 8, c % 16);
        pvga_console_put_char_at(console, row, col, c);
        counter += 1;
        if (counter % 1000000 == 0)
        {
            printf("%" PRIu64 "\n", pvga_console_render_scanline_count);
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
        }
    }

    __builtin_unreachable();
}
