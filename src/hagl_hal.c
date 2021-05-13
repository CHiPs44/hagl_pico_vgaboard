/*

MIT License

Copyright (c) 2021 Christophe Petit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:
https://github.com/CHiPs44/hagl_pico_vgaboard

https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#pragma message "Compiling " __FILE__ "..."

#include <stdio.h>
#include <stdint.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include <bitmap.h>

#include "hagl_hal.h"
#include "pico-vga-framebuffer.h"

scanvideo_mode_t *hgal_hal_vga_mode = NULL;

void hagl_hal_set_vga_mode(const scanvideo_mode_t *vga_mode)
{
    hgal_hal_vga_mode = (scanvideo_mode_t *) vga_mode;
}

void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    plot_point(x0, y0, color);
}

color_t hagl_hal_get_pixel(int16_t x0, int16_t y0)
{
    uint8_t clr = get_point(x0, y0);
    color_t color;
    
}

bitmap_t *hagl_hal_init(void)
{
    setup_video(hgal_hal_vga_mode);
    /* This HAL does not use double buffering so we return NULL. */
    return NULL;
}

// size_t hagl_hal_flush()
// {
//     /* TODO? */
//     return DISPLAY_WIDTH * DISPLAY_HEIGHT * DISPLAY_DEPTH / 8;
// }

// void hagl_hal_close()
// {
//     /* TODO? */
// }

// EOF
