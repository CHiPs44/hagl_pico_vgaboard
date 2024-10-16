/* SPDX-License-Identifier: MIT */

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

// Pico Extras
#include "pico/scanvideo/scanvideo_base.h"

// Pico VGA Board
#include "pico-vgaboard.h"
#include "colors.h"
#include "palettes/palettes.h"
#include "modes/640x480.h"

#include "pico-vgaboard-console.h"

#define VGA_MODE    (&pico_vgaboard_320x240x4bpp)
#define VGA_WIDTH   320
#define VGA_HEIGHT  240
#define VGA_BORDER  PICO_SCANVIDEO_PIXEL_FROM_RGB5(0xf, 0xf, 0xf)
#define COLS        (VGA_WIDTH / 8)
#define ROWS        (VGA_HEIGHT / 8)

t_pvga_console *console;

void main(void)
{
    stdio_init_all();
    pico_vgaboard_init(false);
    console = pvga_console_init(COLS, ROWS);
    pico_vgaboard->render_scanline_plane2 = pvga_console_render_scanline;
    pico_vgaboard->plane2_params = console;
    pico_vgaboard_start(VGA_MODE, VGA_WIDTH, VGA_HEIGHT, VGA_BORDER);

#if !PICO_NO_HARDWARE
    // Seed C library standard RNG with SDK's random number generator
    srand(get_rand_32());
#else
    // Seed RNG with UNIX time
    srand(time(NULL));
#endif

    uint16_t row, col;
    uint8_t c;
    while (true)
    {
        row = rand() % ROWS;
        col = rand() % COLS;
        c = rand() % 256;
        pvga_console_put_char_at(console, row, col, c);
        // tight_loop_contents();
    }

    __builtin_unreachable();
}
