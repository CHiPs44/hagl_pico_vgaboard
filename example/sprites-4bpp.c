#include "hagl_hal.h"
#include "hagl.h"
#include "pico-vgaboard-palettes-sweetie16.h"

#define TOP SWEETIE16_LIGHT_GREY
#define FL1 SWEETIE16_BLUE
#define FL2 SWEETIE16_RED
#define FL3 SWEETIE16_DARK_BLUE
#define SD1 SWEETIE16_BLACK
#define SD2 SWEETIE16_GREY
#define SD3 SWEETIE16_VIOLET

color_t tile_8x8x4_1_bitmap[] = {
    TOP, TOP, TOP, TOP, TOP, TOP, TOP, TOP,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, FL1, FL1, FL1, FL1, FL1, FL1, SD1,
    TOP, SD1, SD1, SD1, SD1, SD1, SD1, SD1,
};

color_t tile_8x8x4_2_bitmap[] = {
    TOP, TOP, TOP, TOP, TOP, TOP, TOP, TOP,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, FL2, FL2, FL2, FL2, FL2, FL2, SD2,
    TOP, SD2, SD2, SD2, SD2, SD2, SD2, SD2,
};

color_t tile_8x8x4_3_bitmap[] = {
    TOP, TOP, TOP, TOP, TOP, TOP, TOP, TOP,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, FL3, FL3, FL3, FL3, FL3, FL3, SD3,
    TOP, SD3, SD3, SD3, SD3, SD3, SD3, SD3,
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
