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

#include "hagl_hal.h"
#include "hagl.h"

// Use Aurora 256

#include "images/cat-8bpp-256x192.h"
#include "images/cow-8bpp-256x192.h"
#include "images/dog-8bpp-256x192.h"

hagl_bitmap_t *images_8bpp[4] = {
    &cat_8bpp_256x192_bitmap,
    &cow_8bpp_256x192_bitmap,
    &dog_8bpp_256x192_bitmap,
};
int image_8bpp_index;
int image_8bpp_speed = 250;
int image_8bpp_counter;

void image_8bpp_draw()
{
    // vgaboard_set_palette(vgaboard_palette_8bpp_aurora);
    hagl_set_clip(hagl_backend, FULL_SCREEN.x, FULL_SCREEN.y, FULL_SCREEN.w - 1, FULL_SCREEN.h - 1);
    hagl_fill_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x, 
        FULL_SCREEN.y, 
        FULL_SCREEN.w, 
        FULL_SCREEN.h, 
        8
    );
    hagl_fill_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images_8bpp[image_8bpp_index]->width ) / 2 + 1, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images_8bpp[image_8bpp_index]->height) / 2 + 1, 
        images_8bpp[image_8bpp_index]->width, 
        images_8bpp[image_8bpp_index]->height, 
        0
    );
    hagl_blit_xy(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images_8bpp[image_8bpp_index]->width ) / 2, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images_8bpp[image_8bpp_index]->height) / 2, 
        images_8bpp[image_8bpp_index]
    );
    hagl_draw_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images_8bpp[image_8bpp_index]->width ) / 2, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images_8bpp[image_8bpp_index]->height) / 2, 
        images_8bpp[image_8bpp_index]->width, 
        images_8bpp[image_8bpp_index]->height, 
        15
    );
}

bool images_8bpp_init()
{
    if (DEPTH!=8) {
        hagl_put_text(hagl_backend, L"8 bpp / 256 colors ONLY!", DEMO.x, DEMO.y, COLORS - 1, font8x13B);
        return false;
    }
    image_8bpp_index = 0;
    image_8bpp_counter = 0;
    image_8bpp_draw();
    return true;
}

void images_8bpp_draw()
{
    image_8bpp_counter += 1;
    if (image_8bpp_counter < image_8bpp_speed) return;
    image_8bpp_counter = 0;
    image_8bpp_index = (image_8bpp_index + 1) % 3;
    image_8bpp_draw();
}

/* EOF */
