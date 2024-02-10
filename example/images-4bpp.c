/*

MIT No Attribution

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

#include "hagl.h"
#include "hagl_hal.h"

// This one has its own palette
#include "images/cat-320x240.h"
// These use Sweetie 16
#include "images/cat-160x120.h"
#include "images/cow-160x120.h"
#include "images/dog-160x120.h"

hagl_bitmap_t *images4_bitmaps[] = {
    // &cat_4bpp_320x240_bitmap,
    &cat_4bpp_160x120_bitmap,
    &cow_4bpp_160x120_bitmap,
    &dog_4bpp_160x120_bitmap,
};
#define N_IMAGES4 (sizeof(images4_bitmaps) / sizeof(hagl_bitmap_t *))
BGAR5515 *images4_palettes[] = {
    // (BGAR5515 *)pico_vgaboard_palette_4bpp_cat_320x240,
    (BGAR5515 *)pico_vgaboard_palette_4bpp_sw16,
    (BGAR5515 *)pico_vgaboard_palette_4bpp_sw16,
    (BGAR5515 *)pico_vgaboard_palette_4bpp_sw16,
};

int images4_index;
int images4_speed = 250;
int images4_counter;
int images4_palette_save;

bool images_4bpp_init()
{
    images4_index = 0;
    images4_counter = 0;
    images4_palette_save = palette_index;
    return true;
}

void images4_draw_one(int index)
{
    pico_vgaboard_set_palette(images4_palettes[index]);
    int16_t w = images4_bitmaps[index]->width;
    int16_t h = images4_bitmaps[index]->height;
    int16_t x = DEMO.x + (DEMO.w - w) / 2;
    int16_t y = DEMO.y + (DEMO.h - h) / 2;
    hagl_blit_xywh(hagl_backend, x, y, w, h, images4_bitmaps[index]);
}

void images_4bpp_draw()
{
    images4_counter += 1;
    if (images4_counter < images4_speed)
    {
        images4_draw_one(images4_index);
        return;
    }
    images4_counter = 0;
    images4_index = (images4_index + 1) % N_IMAGES4;
    images4_draw_one(images4_index);
}

void images_4bpp_done()
{
    // Restore palette
    palette_index = images4_palette_save;
    pico_vgaboard_set_palette(palettes4[palette_index].palette);
}

/* EOF */
