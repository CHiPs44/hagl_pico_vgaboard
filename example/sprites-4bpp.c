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
#include "pico-vgaboard-palettes.h"
#include "pico-vgaboard-palettes-grey.h"

const uint16_t vgaboard_palette_4bpp_space[16] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ GREY16_03,
    /* 02 */ GREY16_06,
    /* 03 */ GREY16_09,
    /* 04 */ GREY16_12,
    /* 05 */ BGAR5515_RED,
    /* 06 */ BGAR5515_LIGHT_RED,
    /* 07 */ BGAR5515_DARK_GREEN,
    /* 08 */ BGAR5515_GREEN,
    /* 09 */ BGAR5515_LIGHT_GREEN,
    /* 10 */ BGAR5515_DARK_BLUE,
    /* 11 */ BGAR5515_BLUE,
    /* 12 */ BGAR5515_LIGHT_BLUE,
    /* 13 */ BGAR5515_ORANGE,
    /* 14 */ BGAR5515_LIGHT_ORANGE,
    /* 15 */ BGAR5515_WHITE
};

#define _ 8
#define M 0
#define X 13
#define Y 11
#define Z 6
#define O 14
#define W 15

color_t ship_16x16x4_1_bitmap[] = {
/*  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  */
    _, _, _, Z, M, _, _, W, M, _, _, Z, M, _, _, _, 
    _, _, _, Y, M, _, _, X, M, _, _, Y, M, _, _, _, 
    _, _, _, W, M, _, X, Y, X, M, _, W, M, _, _, _, 
    _, _, _, X, M, _, X, Y, X, M, _, X, M, _, _, _, 
    _, _, _, X, M, X, Y, Y, Y, X, M, X, M, _, _, _, 
    _, _, _, X, M, X, Y, Y, Y, X, M, X, M, _, _, _, 
    _, _, _, X, X, Y, Y, Y, Y, Y, X, X, M, _, _, _, 
    O, M, _, X, X, Y, Y, Y, Y, Y, X, X, M, _, O, M,
    X, M, _, X, Z, Z, Z, Z, Z, Z, Z, X, M, _, X, M,
    X, M, _, X, X, X, Z, Z, Z, X, X, X, M, _, X, M,
    X, M, X, X, X, X, Z, Z, Z, X, X, X, X, M, X, M,
    X, X, X, X, M, X, X, X, X, X, M, X, X, X, X, M,
    X, X, X, M, M, X, X, X, X, X, M, _, X, X, X, M,
    X, M, M, _, _, _, X, M, X, M, _, _, _, _, X, M,
    X, M, _, _, _, _, O, M, O, M, _, _, _, _, X, M,
    M, M, _, _, _, _, M, M, M, M, _, _, _, _, M, M, 
};

hagl_bitmap_t ship_16x16x4_1 = {
    .width  = 16,
    .height = 16,
    .depth  =  4, // bits
    .pitch  = 32, // 16 * 2, bytes per row
    .size   = sizeof(ship_16x16x4_1_bitmap), // 16 * 16 / 2, // in bytes
    .buffer = (uint8_t *)&ship_16x16x4_1_bitmap
};

#define B0 0
#define G1 1
#define G2 2
#define G3 3
#define G4 4
#define BF 15

color_t tile_8x8x4_0_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
};

color_t tile_8x8x4_1_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, G1, G1, G1, G1, G1, G1, BF,
    B0, G1, G1, G1, G1, G1, G1, BF,
    B0, G1, G1, G1, G1, G1, G1, BF,
    B0, G1, G1, G1, G1, G1, G1, BF,
    B0, G1, G1, G1, G1, G1, G1, BF,
    B0, G1, G1, G1, G1, G1, G1, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

color_t tile_8x8x4_2_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, G2, G2, G2, G2, G2, G2, BF,
    B0, BF, BF, BF, BF, BF, BF, BF,
};

color_t tile_8x8x4_3_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, G3, G3, G3, G3, G3, G3, BF,
    B0, G3, G3, G3, G3, G3, G3, BF,
    B0, G3, G3, G3, G3, G3, G3, BF,
    B0, G3, G3, G3, G3, G3, G3, BF,
    B0, G3, G3, G3, G3, G3, G3, BF,
    B0, G3, G3, G3, G3, G3, G3, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

color_t tile_8x8x4_4_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, G4, G4, G4, G4, G4, G4, BF,
    B0, BF, BF, BF, BF, BF, BF, BF,
};

hagl_bitmap_t tile_8x8x4_0 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_0_bitmap), // 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_0_bitmap
};

hagl_bitmap_t tile_8x8x4_1 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_1_bitmap), // 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_1_bitmap
};

hagl_bitmap_t tile_8x8x4_2 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_2_bitmap), // 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_2_bitmap
};

hagl_bitmap_t tile_8x8x4_3 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_3_bitmap), // 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_3_bitmap
};

hagl_bitmap_t tile_8x8x4_4 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_4_bitmap), // 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_4_bitmap
};

hagl_bitmap_t *tiles_8x8x4[] = {
    &tile_8x8x4_0,
    &tile_8x8x4_1,
    &tile_8x8x4_2,
    &tile_8x8x4_3,
    &tile_8x8x4_4,
};

void
hagl_blit_xywh_alpha(void const *_surface, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, hagl_bitmap_t *source, color_t alpha)
{
    const hagl_surface_t *surface = _surface;

    if (surface->scale_blit) {
        surface->scale_blit(&surface, x0, y0, w, h, source);
    } else {
        color_t color;
        color_t *ptr = (color_t *) source->buffer;
        uint32_t x_ratio = (uint32_t)((source->width << 16) / w);
        uint32_t y_ratio = (uint32_t)((source->height << 16) / h);

        for (uint16_t y = 0; y < h; y++) {
            for (uint16_t x = 0; x < w; x++) {
                uint16_t px = ((x * x_ratio) >> 16);
                uint16_t py = ((y * y_ratio) >> 16);
                color = ptr[(uint8_t)((py * source->width) + px)];
                if (alpha!=color) {
                    hagl_put_pixel(surface, x0 + x, y0 + y, color);
                }
            }
        }
    }
};

const int zoom = 1;
int tile_width;
int tile_height;
int tile_columns;
int tile_lines;
uint8_t *tile_map;
int ship_x;
int ship_y;
int ship_dx;
int ship_dy;
int ship_counter = 0;

void sprites_init()
{
    if (DEPTH!=4) {
        return;
    }
    // vgaboard_set_palette((const uint16_t *)(&vgaboard_palette_4bpp_space));
    palette_name = L"SPACE!";
    tile_width   = 8 * zoom;
    tile_height  = 8 * zoom;
    tile_columns = DEMO.w / tile_width;
    tile_lines   = DEMO.h / tile_height;
    tile_map = malloc(sizeof(uint8_t) * tile_columns * tile_lines);
    // printf(
    //     "width: %d, height: %d, columns: %d, lines: %d, map=%p\r\n", 
    //     tile_width, tile_height, tile_columns, tile_lines, tile_map
    // );
    // sleep_ms(1000);
    for(int line = 0; line < tile_lines; line++) {
        for(int column = 0; column < tile_columns; column++) {
            tile_map[line * tile_columns + column] = rand() % 5;
            hagl_blit_xywh(
                hagl_backend, 
                DEMO.x + tile_width  * column, 
                DEMO.y + tile_height * line, 
                tile_width, tile_height, 
                tiles_8x8x4[tile_map[line * tile_columns + column]]
            );
        }
    }
    ship_x = DEMO.w / 2 - ship_16x16x4_1.width / 2;//rand() % DEMO.w;
    ship_y = DEMO.h / 2 - ship_16x16x4_1.height / 2;//rand() % DEMO.h;
    ship_dx = 1;//rand() % 2 == 0 ? 1 : -1;
    ship_dy = 0;//rand() % 2 == 0 ? 1 : -1;
}

void sprites_draw()
{
    if (DEPTH!=4) {
        return;
    }
    // ship_counter += 1;
    // if (ship_counter < 5)
    // {
    //     return;
    // }
    // ship_counter = 0;

    // Clean old position of ship
    int col1 = ship_x / tile_width;
    if (col1 < 0) {
        col1 = 0;
    }
    int col2 = col1 + 16 / tile_width + 2;
    if (col2 >= tile_columns) {
        col2 = tile_columns - 1;
    }
    int line1 = ship_y / tile_height;
    if (line1 < 0) {
        line1 = 0;
    }
    int line2 = line1 + 8 / tile_height + 2;
    if (line2 > tile_lines) {
        line2 = tile_lines - 1;
    }
    for (int line = line1; line < line2; line++) {
        for (int column = col1; column < col2; column++) {
            hagl_blit_xywh(
                hagl_backend, 
                DEMO.x + tile_width  * column, 
                DEMO.y + tile_height * line, 
                tile_width, tile_height, 
                tiles_8x8x4[tile_map[line * tile_columns + column]]
            );
        }
    }
    // hagl_draw_rectangle_xyxy(
    //     hagl_backend, 
    //     DEMO.x + tile_width  * col1, 
    //     DEMO.y + tile_height * line1, 
    //     DEMO.x + tile_width  * col2, 
    //     DEMO.y + tile_height * line2,
    //     13 /* orange */         
    // );

    // Move and draw ship
    if (ship_x + ship_dx < 0) {
        ship_x = 0;
        ship_dx = -ship_dx;
    } else if (ship_x + 16 + ship_dx > DEMO.w) {
        ship_x = DEMO.w - 16;
        ship_dx = -ship_dx;
    } else {
        ship_x += ship_dx;
    }
    if (ship_y + ship_dy < 0) {
        ship_y = 0;
        ship_dy = -ship_dy;
    } else if (ship_y + 8 + ship_dy > DEMO.h) {
        ship_y = DEMO.h - 8;
        ship_dy = -ship_dy;
    } else {
        ship_y += ship_dy;
    }
    hagl_blit_xywh_alpha(
        hagl_backend, 
        DEMO.x + ship_x, DEMO.y + ship_y, 
        16, 16, 
        &ship_16x16x4_1,
        _
    );
}

/* EOF */
