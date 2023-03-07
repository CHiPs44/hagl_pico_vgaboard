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

// Use Sweetie 16 for now
// #include "pico-vgaboard-palettes.h"
// #include "pico-vgaboard-palettes-grey.h"
// const uint16_t vgaboard_palette_4bpp_space[16] = {
//     /* 00 */ BGAR5515_BLACK,
//     /* 01 */ GREY16_03,
//     /* 02 */ GREY16_06,
//     /* 03 */ GREY16_09,
//     /* 04 */ GREY16_12,
//     /* 05 */ BGAR5515_RED,
//     /* 06 */ BGAR5515_LIGHT_RED,
//     /* 07 */ BGAR5515_DARK_GREEN,
//     /* 08 */ BGAR5515_GREEN,
//     /* 09 */ BGAR5515_LIGHT_GREEN,
//     /* 10 */ BGAR5515_DARK_BLUE,
//     /* 11 */ BGAR5515_BLUE,
//     /* 12 */ BGAR5515_LIGHT_BLUE,
//     /* 13 */ BGAR5515_ORANGE,
//     /* 14 */ BGAR5515_LIGHT_ORANGE,
//     /* 15 */ BGAR5515_WHITE
// };
// #define _ 8
// #define M 0
// #define X 13
// #define Y 11
// #define Z 6
// #define O 14
// #define W 15

#define _ 1
#define M 0
#define X 13
#define Y 14
#define Z 2
#define O 3
#define W 12

hagl_color_t ship_16x16x4_1_bitmap[] = {
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

/*
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0xc, 0xe,
    0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0xe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xd, 0xc, 0xd, 0x0,
    0x0, 0x0, 0x0, 0xe, 0xe, 0xd, 0x0, 0x0,
    0x0, 0x0, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0,
*/
#define B0 0
#define G0 1
#define G1 2
#define G2 3
#define G3 4
#define G4 6
#define BF 14

hagl_color_t tile_8x8x4_0_bitmap[] = {
    G0, G0, G0, G0, G0, G0, G0, B0,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    G0, G0, G0, G0, G0, G0, G0, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_1_bitmap[] = {
    G1, G1, G1, G1, G1, G1, G1, B0,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    G1, G1, G1, G1, G1, G1, G1, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_2_bitmap[] = {
    G2, G2, G2, G2, G2, G2, G2, B0,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    G2, G2, G2, G2, G2, G2, G2, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_3_bitmap[] = {
    G3, G3, G3, G3, G3, G3, G3, B0,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    G3, G3, G3, G3, G3, G3, G3, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_color_t tile_8x8x4_4_bitmap[] = {
    G4, G4, G4, G4, G4, G4, G4, B0,
    G4, G4, G4, G4, G4, G4, G4, BF,
    G4, G4, G4, G4, G4, G4, G4, BF,
    G4, G4, G4, G4, G4, G4, G4, BF,
    G4, G4, G4, G4, G4, G4, G4, BF,
    G4, G4, G4, G4, G4, G4, G4, BF,
    G4, G4, G4, G4, G4, G4, G4, BF,
    BF, BF, BF, BF, BF, BF, BF, BF,
};

hagl_bitmap_t tile_8x8x4_0 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_0_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_0_bitmap
};

hagl_bitmap_t tile_8x8x4_1 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_1_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_1_bitmap
};

hagl_bitmap_t tile_8x8x4_2 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_2_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_2_bitmap
};

hagl_bitmap_t tile_8x8x4_3 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_3_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_3_bitmap
};

hagl_bitmap_t tile_8x8x4_4 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile_8x8x4_4_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile_8x8x4_4_bitmap
};

hagl_bitmap_t *tiles_8x8x4[] = {
    &tile_8x8x4_0,
    &tile_8x8x4_1,
    &tile_8x8x4_2,
    &tile_8x8x4_3,
    &tile_8x8x4_4,
};

const int tile_zoom = 1;
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

bool sprites_init()
{
    if (DEPTH!=4) {
        return false;
    }

    // use whatever current palette for now...
    // vgaboard_set_palette((const uint16_t *)(&vgaboard_palette_4bpp_space));
    // palette_name = L"SPACE!";
    tile_width   = 8 * tile_zoom;
    tile_height  = 8 * tile_zoom;
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
    ship_x = rand() % DEMO.w;//DEMO.w / 2 - ship_16x16x4_1.width / 2;
    ship_y = rand() % DEMO.h;//DEMO.h / 2 - ship_16x16x4_1.height / 2;
    ship_dx = rand() % 2 == 0 ? 1 : -1;
    ship_dy = rand() % 2 == 0 ? 1 : -1;
    return true;
}

void sprites_draw()
{
    if (DEPTH!=4) {
        return;
    }
    ship_counter += 1;
    if (ship_counter % 25 == 0) {
        if (ship_dx == 0 && rand() % 5 == 0) {
            ship_dx = rand() % 2 == 0 ? 1 : -1;
        } else if (rand() % 5 == 0) {
            ship_dx = 0;
        } else if (rand() % 5 == 0) {
            ship_dx = -ship_dx;
        }
        if (ship_dy == 0 && rand() % 5 == 0) {
            ship_dy = rand() % 2 == 0 ? 1 : -1;
        } else if (rand() % 5 == 0) {
            ship_dy = 0;
        } else if (rand() % 5 == 0) {
            ship_dy = -ship_dy;
        }
    }

    // Clean old position of ship
    int col1 = ship_x / tile_width - 1;
    if (col1 < 0) {
        col1 = 0;
    }
    int col2 = col1 + ship_16x16x4_1.width / tile_width + 2;
    if (col2 >= tile_columns) {
        col2 = tile_columns;
    }
    int line1 = ship_y / tile_height - 1;
    if (line1 < 0) {
        line1 = 0;
    }
    int line2 = line1 + ship_16x16x4_1.height / tile_height + 2;
    if (line2 > tile_lines) {
        line2 = tile_lines;
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
    //     SWEETIE16_ORANGE         
    // );

    // Move and draw ship
    if (ship_x + ship_dx < 0) {
        ship_x = 0;
        ship_dx = -ship_dx;
    } else if (ship_x + ship_16x16x4_1.width + ship_dx > DEMO.w) {
        ship_x = DEMO.w - ship_16x16x4_1.width;
        ship_dx = -ship_dx;
    } else {
        ship_x += ship_dx;
    }
    if (ship_y + ship_dy < 0) {
        ship_y = 0;
        ship_dy = -ship_dy;
    } else if (ship_y + ship_16x16x4_1.height + ship_dy > DEMO.h) {
        ship_y = DEMO.h - ship_16x16x4_1.height;
        ship_dy = -ship_dy;
    } else {
        ship_y += ship_dy;
    }
    hagl_blit_xywh_transparent(
        hagl_backend, 
        DEMO.x + ship_x, DEMO.y + ship_y, 
        ship_16x16x4_1.width, ship_16x16x4_1.height, 
        &ship_16x16x4_1,
        _
    );
}

/* EOF */
