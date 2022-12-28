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

#define B0 0
#define G1 1
#define G2 2
#define G3 3
#define BO 15
// #define SD1 
// #define SD2 
// #define SD3 

color_t tile_8x8x4_1_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, G1, G1, G1, G1, G1, G1, B0,
    B0, G1, BO, G1, G1, G1, G1, B0,
    B0, G1, G1, G1, G1, G1, G1, B0,
    B0, G1, G1, G1, G1, G1, G1, B0,
    B0, G1, G1, G1, G1, BO, G1, B0,
    B0, G1, G1, G1, G1, G1, G1, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
};

color_t tile_8x8x4_2_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, G2, G2, G2, G2, G2, G2, B0,
    B0, G2, G2, G2, G2, BO, G2, B0,
    B0, G2, G2, G2, G2, G2, G2, B0,
    B0, G2, G2, G2, G2, G2, G2, B0,
    B0, G2, BO, G2, G2, G2, G2, B0,
    B0, G2, G2, G2, G2, G2, G2, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
};

color_t tile_8x8x4_3_bitmap[] = {
    B0, B0, B0, B0, B0, B0, B0, B0,
    B0, G3, G3, G3, G3, G3, G3, B0,
    B0, G3, BO, G3, G3, BO, G3, B0,
    B0, G3, G3, G3, G3, G3, G3, B0,
    B0, G3, G3, G3, G3, G3, G3, B0,
    B0, G3, BO, G3, G3, BO, G3, B0,
    B0, G3, G3, G3, G3, G3, G3, B0,
    B0, B0, B0, B0, B0, B0, B0, B0,
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

hagl_bitmap_t *tiles_8x8x4[] = {
    &tile_8x8x4_1,
    &tile_8x8x4_2,
    &tile_8x8x4_3,
};

void sprites_init()
{
    vgaboard_set_palette(&vgaboard_palette_4bpp_space);
    // hagl_blit_xywh(hagl_backend, DEMO.x + 8, DEMO.y + 8, 8 * 4, 8 * 4, &tile_8x8x4_1);
    int zoom = 1;
    for(int i = 0; i < DEMO.w / 8 / zoom; i++) {
        for(int j = 0; j < DEMO.h / 8 / zoom; j++) {
            // hagl_blit(hagl_backend, DEMO.x + 8 + 8 * i, DEMO.y + 8 + 8 * j, &tile_8x8x4_1);
            hagl_blit_xywh(
                hagl_backend, 
                DEMO.x + 8 * i * zoom, 
                DEMO.y + 8 * j * zoom, 
                8 * zoom, 8 * zoom, 
                // rand() % 2 ? &tile_8x8x4_1 : &tile_8x8x4_2
                tiles_8x8x4[rand() % 3]
            );
        }
    }
}

void sprites_draw()
{

}

/* EOF */
