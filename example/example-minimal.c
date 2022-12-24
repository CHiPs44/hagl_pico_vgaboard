/*

MIT No Attribution

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

void minimal_init()
{
    // Upper half: color stripes writing directly to framebuffer
    // assume 4bpp => 2 pixels per byte
    size_t real_fb_size = vgaboard->width * vgaboard->height / 2;
    for(int c = 0; c < 16; c += 1) {
        size_t offset = c * real_fb_size / 16 / 2;
        size_t bytes = real_fb_size / 16 / 2;
        uint8_t byte = c * 16 + c;
        uint8_t *address = vgaboard->framebuffer + offset;
        printf(
            "framebuffer=%p address=%p offset=%04x bytes=%04x byte=%02x\r\n", 
            vgaboard->framebuffer, address, offset, bytes, byte
        );
        memset(address, byte,  bytes);
    }
    // Lower half: color stripes using HAGL
    // hagl_clear(hagl_backend);
    for (int i = 0; i < 16; i += 1) {
        // scanvideo_wait_for_vblank();
        hagl_fill_rectangle_xywh(
            hagl_backend, 
            0, vgaboard->height / 2 + (vgaboard->height / 2 / 16) * i, 
            vgaboard->width, vgaboard->height / 16 / 2, 
            i
        );
    }
}

void minimal_loop()
{
    int x0, y0, x1, y1;
    color_t c0, c1;
    int counter = 0;
    printf("counter=%d\r\n", counter / 100);
    while (true) {
        // tight_loop_contents();
        scanvideo_wait_for_vblank();
        x0 = rand() % vgaboard->width;
        y0 = rand() % vgaboard->height;
        c0 = hagl_get_pixel(hagl_backend, x0, y0);
        x1 = rand() % vgaboard->width;
        y1 = rand() % vgaboard->height;
        c1 = hagl_get_pixel(hagl_backend, x1, y1);
        hagl_put_pixel(hagl_backend, x0, y0, c1);
        hagl_put_pixel(hagl_backend, x1, y1, c0);
        counter += 1;
        if (counter % 10000 == 0) {
            printf("counter=%d\r\n", counter / 10000);
        }
    }
}

int main(void)
{
    stdio_init_all();
    vgaboard_init();
    hagl_backend = hagl_init();
    vgaboard_setup(&vgaboard_256x192x4bpp_24576_2);
    vgaboard_set_palette(vgaboard_palette_4bpp_sweetie16);
    vgaboard_dump(vgaboard);
    scanvideo_dump(vgaboard->scanvideo_mode);
    minimal_init();

    printf("*** CORE0 => MINIMAL DEMO ***\n");
    printf("*** CORE1 => RENDER LOOP ***\n");
    vgaboard_enable();
    multicore_launch_core1(vgaboard_render_loop);
    minimal_loop();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
