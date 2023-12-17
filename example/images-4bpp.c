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

// Use Sweetie 16

#include "images/cat-320x240.h"
#include "images/cat-160x120.h"
#include "images/cow-160x120.h"
#include "images/dog-160x120.h"

hagl_bitmap_t *images[4] = {
    &cat_4bpp_320x240_bitmap,
    &cat_160x120_bitmap,
    &cow_4bpp_160x120_bitmap,
    &dog_4bpp_160x120_bitmap,
};
int image_index;
int image_speed = 250;
int image_counter;

void image_draw()
{
    if (image_index==0) {
        pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_cat_320x240);
    } else {
        pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_sw16);
    }
    hagl_set_clip(hagl_backend, FULL_SCREEN.x, FULL_SCREEN.y, FULL_SCREEN.w - 1, FULL_SCREEN.h - 1);
    hagl_fill_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x, 
        FULL_SCREEN.y, 
        FULL_SCREEN.w, 
        FULL_SCREEN.h, 
        SWEETIE16_GREY
    );
    hagl_fill_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images[image_index]->width ) / 2 + 1, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images[image_index]->height) / 2 + 1, 
        images[image_index]->width, 
        images[image_index]->height, 
        0
    );
    hagl_blit_xywh(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images[image_index]->width ) / 2, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images[image_index]->height) / 2, 
        images[image_index]->width, 
        images[image_index]->height, 
        images[image_index]
    );
    hagl_draw_rectangle_xywh(
        hagl_backend, 
        FULL_SCREEN.x + (FULL_SCREEN.w - images[image_index]->width ) / 2, 
        FULL_SCREEN.y + (FULL_SCREEN.h - images[image_index]->height) / 2, 
        images[image_index]->width, 
        images[image_index]->height, 
        15
    );
}

bool images_4bpp_init()
{
    if (DEPTH!=4) {
        hagl_put_text(hagl_backend, L"4 bpp / 16 colors ONLY!", DEMO.x, DEMO.y, COLORS - 1, font8x13B);
        return false;
    }
    image_index = 0;
    image_counter = 0;
    image_draw();
    return true;
}

void images_4bpp_draw()
{
    if (DEPTH!=4) {
        return;
    }
    image_counter += 1;
    if (image_counter < image_speed) return;
    image_counter = 0;
    image_index = (image_index + 1) % 4;
    image_draw();
}

void images_4bpp_done()
{
    pico_vgaboard_set_palette(pico_vgaboard_palette_4bpp_sw16);
}

/* EOF */
