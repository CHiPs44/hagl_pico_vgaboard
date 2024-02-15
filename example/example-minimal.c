/* SPDX-License-Identifier: MIT-0 */

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
    pico_vgaboard_init();
    pico_vgaboard_start(&vgaboard_256x192x4bpp_24576_2);
    pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_sw16);
    pico_vgaboard_dump(pico_vgaboard);
    scanvideo_dump(pico_vgaboard->scanvideo_mode);
    hagl_backend = hagl_init();

    printf("*** CORE1 => RENDER LOOP ***\n");
    // pico_vgaboard_enable();
    multicore_launch_core1(vgaboard_render_loop);
    printf("*** CORE0 => MINIMAL DEMO ***\n");
    minimal_init();
    minimal_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
