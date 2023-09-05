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
#include <string.h>
// Pico
#include "pico.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
// Pico VGA Board
#include "pico-vgaboard.h"
// Colors
#include "pico-vgaboard-colors.h"
// Palettes
#include "pico-vgaboard-palettes.h"
#include "pico-vgaboard-palettes-sweetie16.h"
// Modes
// #include "pico-vgaboard-modes-640x480.h"
#include "pico-vgaboard-modes-768x576.h"
// HAGL
#include "hagl_hal.h"
#include "hagl.h"

#define RAM __not_in_flash("example")

hagl_backend_t RAM *hagl_backend = NULL;

#include "minimal.c"

int main(void)
{
    stdio_init_all();
    vgaboard_init();
    vgaboard_setup(&vgaboard_256x192x4bpp_24576_2);
    vgaboard_set_palette(vgaboard_palette_4bpp_sweetie16);
    vgaboard_dump(vgaboard);
    scanvideo_dump(vgaboard->scanvideo_mode);
    hagl_backend = hagl_init();

    printf("*** CORE1 => RENDER LOOP ***\n");
    // vgaboard_enable();
    multicore_launch_core1(vgaboard_render_loop);
    printf("*** CORE0 => MINIMAL DEMO ***\n");
    minimal_init();
    minimal_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */