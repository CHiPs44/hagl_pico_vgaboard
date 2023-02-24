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

#include "cat-160x120.h"
#include "cow-160x120.h"
#include "dog-160x120.h"

hagl_bitmap_t *images[3] = {
    &cat_160x120_bitmap,
    &cow_160x120_bitmap,
    &dog_160x120_bitmap,
};
int image_index;
int image_speed = 250;
int image_counter;

void image_draw()
{
    hagl_fill_rectangle_xywh(
        hagl_backend, 
        DEMO.x + (DEMO.w - 160) / 2 + 1, 
        DEMO.y + (DEMO.h - 120) / 2 + 1, 
        160, 120, 
        SWEETIE16_BLACK
    );
    hagl_blit_xywh(
        hagl_backend, 
        DEMO.x + (DEMO.w - 160) / 2, 
        DEMO.y + (DEMO.h - 120) / 2, 
        160, 120, 
        images[image_index]
    );
    hagl_draw_rectangle_xywh(
        hagl_backend, 
        DEMO.x + (DEMO.w - 160) / 2, 
        DEMO.y + (DEMO.h - 120) / 2, 
        160, 120, 
        SWEETIE16_WHITE
    );
}

void images_init()
{
    if (DEPTH!=4) {
        return;
    }
    image_index = 0;
    image_counter = 0;
    image_draw();
}

void images_draw()
{
    if (DEPTH!=4) {
        return;
    }
    image_counter += 1;
    if (image_counter < image_speed) return;
    image_counter = 0;
    image_index = (image_index + 1) % 3;
    image_draw();
}

/* EOF */
