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
// #include "hardware/clocks.h"
// #include "hardware/vreg.h"
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
#include "pico-vgaboard-modes-1024x768.h"
// HAGL
#include "hagl_hal.h"
#include "hagl.h"

#define RAM __not_in_flash("example")

hagl_backend_t RAM *hagl_backend = NULL;

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "pico-vgaboard.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

void __not_in_flash_func(vgaboard_render_loop_4bpp)(void)
{
    int counter = 0;
#if PICO_VGABOARD_DEBUG
    printf("VGABOARD: Starting render %dx%dx%d/%d@%dHz (%dMHz)\n",
           vgaboard->width, vgaboard->height,
           vgaboard->depth, vgaboard->colors,
           vgaboard->freq_hz, clock_get_hz(clk_sys) / 1000000);
#endif
    vgaboard_enable();
    // Let's go for the show!
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        uint16_t scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *scanline_colors = buffer->data;
        uint8_t *framebuffer_line_start;
        uint8_t bits, bits76, bits54, bits32, bits10, bits7654, bits3210;
        framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width / 2) * scanline_number]);
#if USE_INTERP == 1
        ++scanline_colors;
        convert_from_pal16(scanline_colors, framebuffer_line_start, vgaboard->width / 2);
        scanline_colors += vgaboard->width / 2;
#else
        for (uint16_t x = 0; x < vgaboard->width / 2; ++x)
        {
            ++scanline_colors;
            *scanline_colors = vgaboard_double_palette_4bpp[*framebuffer_line_start];
            ++framebuffer_line_start;
        }
        ++scanline_colors;
#endif
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (vgaboard->width - 2);
        buffer->data_used = (vgaboard->width + 4) / 2; // 2 16 bits pixels in each 32 bits word
        scanvideo_end_scanline_generation(buffer);
        counter += 1;
        if (counter>1000) {
            counter = 0;
            vgaboard_toggle_led();
        }
    }
}

void minimal_init()
{
    // hagl_clear(hagl_backend);
    // for (int i = 0; i < 16; i += 1) {
    //     // scanvideo_wait_for_vblank();
    //     hagl_draw_rectangle_xywh(
    //         hagl_backend, 
    //         4 * i, 4 * i, 
    //         vgaboard->width - 8 * i, vgaboard->height - 8 * i, 
    //         i
    //     );
    // }
    // assume 4bpp => 2 pixels per byte
    size_t real_fb_size = vgaboard->width * vgaboard->height / 2;
    for(int c = 0; c < 16; c += 1) {
        size_t offset = c * real_fb_size / 16;
        size_t bytes = real_fb_size / 16;
        uint8_t byte = c * 16 + c;
        uint8_t *address = vgaboard->framebuffer + offset;
        printf(
            "framebuffer=%p address=%p offset=%04x bytes=%04x byte=%02x\r\n", 
            vgaboard->framebuffer, address, offset, bytes, byte
        );
        memset(address, byte,  bytes);
    }
}

void minimal_loop()
{
    // int counter = 0;
    // printf("counter=%d\r\n", counter / 100);
    while (true) {
        // tight_loop_contents();
        scanvideo_wait_for_vblank();
        // sleep_ms(100);
        // counter += 1;
        // if (counter % 10000 == 0) {
        //     printf("counter=%d\r\n", counter / 10000);
        // }
    }
}

int main(void)
{
    // stdio_init_all();
    vgaboard_init();
    // hagl_backend = hagl_init();
    // vgaboard_setup(&vgaboard_320x240x4bpp);
    vgaboard_setup(&vgaboard_256x192x4bpp_24576_1);
    vgaboard_set_palette(vgaboard_palette_4bpp_sweetie16);
    vgaboard_dump(vgaboard);
    scanvideo_dump(vgaboard->scanvideo_mode);
    minimal_init();

    printf("*** CORE0 => MINIMAL DEMO ***\n");
    printf("*** CORE1 => RENDER LOOP ***\n");
    // sleep_ms(1000);
    // vgaboard_enable();
    // sleep_ms(1000);
    multicore_launch_core1(vgaboard_render_loop_4bpp);
    // sleep_ms(1000);
    minimal_loop();

    // printf("*** CORE0 => RENDER LOOP ***\n");
    // printf("*** CORE1 => MINIMAL DEMO ***\n");
    // // sleep_ms(1000);
    // vgaboard_enable();
    // // sleep_ms(1000);
    // multicore_launch_core1(minimal_loop);
    // // sleep_ms(1000);
    // vgaboard_render_loop_4bpp();

    printf("*** UNREACHABLE ***\n");
    hagl_close(hagl_backend);
    return 0;
}

/* EOF */
