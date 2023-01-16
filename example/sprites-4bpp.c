#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vgaboard-palettes.h"
#include "pico-vgaboard-palettes-grey.h"

const uint16_t vgaboard_palette_4bpp_space[16] = {
    /* 00 */ BGAR5515_BLACK,
    /* 01 */ GREY16_04,
    /* 02 */ GREY16_08,
    /* 03 */ GREY16_12,
    /* 04 */ BGAR5515_DARK_RED,
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

/*
    0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
  0               XXXX
  1       XX    XXYYYYXX    XX
  2       XX  XXYYYYYYYYXX  XX
  3       XXXXYYYYYYYYYYYYXXXX
  4 XX    XXZZZZZZZZZZZZZZZZXX    XX
  5 XX  XXXXXXXXZZZZZZZZXXXXXXXX  XX
  6 XXXXXX    XXXXXXXXXXXX    XXXXXX
  7 XX          XX    XX          XX
*/

#define __ 8
#define XX 1
#define YY 2
#define ZZ 3

color_t ship_8x8x4_1_bitmap[] = {
/*  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  */
    __, __, __, __, __, __, __, XX, XX, __, __, __, __, __, __, __, 
    __, __, __, XX, __, __, XX, YY, YY, XX, __, __, XX, __, __, __, 
    __, __, __, XX, __, XX, YY, YY, YY, YY, XX, __, XX, __, __, __, 
    __, __, __, XX, XX, YY, YY, YY, YY, YY, YY, XX, XX, __, __, __, 
    XX, __, __, XX, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, ZZ, XX, __, __, XX, 
    XX, __, __, XX, XX, XX, ZZ, ZZ, ZZ, ZZ, XX, XX, XX, __, __, XX, 
    XX, XX, XX, __, __, XX, XX, XX, XX, XX, XX, __, __, XX, XX, XX, 
    XX, __, __, __, __, __, XX, __, __, XX, __, __, __, __, __, XX, 
};

hagl_bitmap_t ship_8x8x4_1 = {
    .width  = 16,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 32, // 16 * 2, bytes per row
    .size   = 16 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&ship_8x8x4_1_bitmap
};

#define B0 0
#define G1 1
#define G2 2
#define G3 3
#define BO 15
color_t tile_8x8x4_1_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BO,
    B0, G1, G1, G1, G1, G1, G1, BO,
    B0, G1, BO, G1, G1, G1, G1, BO,
    B0, G1, G1, G1, G1, G1, G1, BO,
    B0, G1, G1, G1, G1, G1, G1, BO,
    B0, G1, G1, G1, G1, BO, G1, BO,
    B0, G1, G1, G1, G1, G1, G1, BO,
    B0, BO, BO, BO, BO, BO, BO, BO,
};

color_t tile_8x8x4_2_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BO,
    B0, G2, G2, G2, G2, G2, G2, BO,
    B0, G2, G2, G2, G2, BO, G2, BO,
    B0, G2, G2, G2, G2, G2, G2, BO,
    B0, G2, G2, G2, G2, G2, G2, BO,
    B0, G2, BO, G2, G2, G2, G2, BO,
    B0, G2, G2, G2, G2, G2, G2, BO,
    B0, BO, BO, BO, BO, BO, BO, BO,
};

color_t tile_8x8x4_3_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, BO, G3, G3, BO, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, BO, G3, G3, BO, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, BO, BO, BO, BO, BO, BO, BO,
};

color_t tile_8x8x4_4_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, BO, G3, G3, G3, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, G3, G3, G3, G3, BO, G3, BO,
    B0, G3, G3, G3, G3, G3, G3, BO,
    B0, BO, BO, BO, BO, BO, BO, BO,
};

hagl_bitmap_t tile_8x8x4_1 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_1_bitmap
};

hagl_bitmap_t tile_8x8x4_2 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_2_bitmap
};

hagl_bitmap_t tile_8x8x4_3 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_3_bitmap
};

hagl_bitmap_t tile_8x8x4_4 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = 8 * 8 / 2, // in bytes
    .buffer = (uint8_t *)&tile_8x8x4_4_bitmap
};

hagl_bitmap_t *tiles_8x8x4[] = {
    &tile_8x8x4_1,
    &tile_8x8x4_2,
    &tile_8x8x4_3,
    &tile_8x8x4_4,
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
    vgaboard_set_palette((const uint16_t *)(&vgaboard_palette_4bpp_space));
    tile_width   = 8 * zoom;
    tile_height  = 8 * zoom;
    tile_columns = DEMO.w / tile_width;
    tile_lines   = DEMO.h / tile_height;
    tile_map = malloc(sizeof(uint8_t) * tile_columns * tile_lines);
    printf(
        "width: %d, height: %d, columns: %d, lines: %d, map=%p\r\n", 
        tile_width, tile_height, tile_columns, tile_lines, tile_map
    );
    sleep_ms(1000);
    for(int line = 0; line < tile_lines; line++) {
        for(int column = 0; column < tile_columns; column++) {
            tile_map[line * tile_columns + column] = rand() % 4;
            hagl_blit_xywh(
                hagl_backend, 
                DEMO.x + tile_width  * column, 
                DEMO.y + tile_height * line, 
                tile_width, tile_height, 
                tiles_8x8x4[tile_map[line * tile_columns + column]]
            );
        }
    }
    ship_x = rand() % DEMO.w;
    ship_y = rand() % DEMO.h;
    ship_dx = rand() % 2 == 0 ? 2 : -2;
    ship_dy = rand() % 2 == 0 ? 2 : -2;
}

void sprites_draw()
{
    // ship_counter += 1;
    // if (ship_counter < 10)
    // {
    //     return;
    // }
    // ship_counter = 0;
    // Clean old position of ship
    int col1 = ship_x / tile_width;
    int col2 = col1 + 1 + 16 / tile_width;
    if (col2 > tile_columns) {
        col2 = tile_columns - 1;
    }
    int line1 = ship_y / tile_height;
    int line2 = line1 + 8 / tile_height;
    if (line2 > tile_lines) {
        line2 = tile_lines - 1;
    }
    for(int line = line1; line < line2; line++) {
        for(int column = col1; column < col2; column++) {
            hagl_blit_xywh(
                hagl_backend, 
                DEMO.x + tile_width  * column, 
                DEMO.y + tile_height * line, 
                tile_width, tile_height, 
                tiles_8x8x4[tile_map[line * tile_columns + column]]
            );
        }
    }
    // Move and draw ship
    if (ship_x + ship_dx < 0) {
        ship_x = 0;
        ship_dx = 1;
    } else if (ship_x + 16 + ship_dx > DEMO.w) {
        ship_x = DEMO.w - 16;
        ship_dx = -1;
    } else {
        ship_x += ship_dx;
    }
    if (ship_y + ship_dy < 0) {
        ship_y = 0;
        ship_dy = 1;
    } else if (ship_y + 8 + ship_dy > DEMO.h) {
        ship_y = DEMO.h - 8;
        ship_dy = -1;
    } else {
        ship_y += ship_dy;
    }
    hagl_blit_xywh(
        hagl_backend, 
        DEMO.x + ship_x, DEMO.y + ship_y, 
        16, 8, 
        &ship_8x8x4_1
    );
}

/* EOF */
