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

// Pico VGA Board
#include "pico-vgaboard.h"
#include "colors.h"
#include "palettes/palettes.h"
#include "modes/640x480.h"

#include "../include/pico-vgaboard-text-screen.h"

int main(void)
{
    stdio_init_all();
    pico_vgaboard_init(false);
    pico_vgaboard_start(pico_vgaboard_320x240x4bpp, 0, 0, PICO_SCANVIDEO_PIXEL_FROM_RGB5(0xf, 0xf, 0xf));

    while (true)
    {

    }

    __builtin_unreachable();
}
