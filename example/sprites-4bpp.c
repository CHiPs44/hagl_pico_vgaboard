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

// Use current palette for now
// #include "pico-vgaboard-palettes.h"
// #include "pico-vgaboard-palettes-grey.h"
// const uint16_t pico_vgaboard_palette_4bpp_space[16] = {
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
    /*  00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 */
    _, _, _, Z, M, _, _, W, M, _, _, Z, M, _, _, _, /* 00 */
    _, _, _, Y, M, _, _, X, M, _, _, Y, M, _, _, _, /* 01 */
    _, _, _, W, M, _, X, Y, X, M, _, W, M, _, _, _, /* 02 */
    _, _, _, X, M, _, X, Y, X, M, _, X, M, _, _, _, /* 03 */
    _, _, _, X, M, X, Y, Y, Y, X, M, X, M, _, _, _, /* 04 */
    _, _, _, X, M, X, Y, Y, Y, X, M, X, M, _, _, _, /* 05 */
    _, _, _, X, X, Y, Y, Y, Y, Y, X, X, M, _, _, _, /* 06 */
    O, M, _, X, X, Y, Y, Y, Y, Y, X, X, M, _, O, M, /* 07 */
    X, M, _, X, Z, Z, Z, Z, Z, Z, Z, X, M, _, X, M, /* 08 */
    X, M, _, X, X, X, Z, Z, Z, X, X, X, M, _, X, M, /* 09 */
    X, M, X, X, X, X, Z, Z, Z, X, X, X, X, M, X, M, /* 10 */
    X, X, X, X, M, X, X, X, X, X, M, X, X, X, X, M, /* 11 */
    X, X, X, M, M, X, X, X, X, X, M, _, X, X, X, M, /* 12 */
    X, M, M, _, _, _, X, M, X, M, _, _, _, _, X, M, /* 13 */
    X, M, _, _, _, _, O, M, O, M, _, _, _, _, X, M, /* 14 */
    M, M, _, _, _, _, M, M, M, M, _, _, _, _, M, M, /* 15 */
};

hagl_bitmap_t ship_16x16x4_1 = {
    .width = 16,
    .height = 16,
    .depth = 4,                            // bits
    .pitch = 32,                           // 16 * 2, bytes per row
    .size = sizeof(ship_16x16x4_1_bitmap), // 16 * 16 / 2, // in bytes
    .buffer = (uint8_t *)&ship_16x16x4_1_bitmap};

int ship_x;
int ship_y;
int ship_dx;
int ship_dy;
int ship_counter = 0;

#include "tiles-4bpp.c"
#include "aliens-4bpp.c"

bool sprites_init()
{
    if (DEPTH != 4)
    {
        return false;
    }
    // use whatever current palette for now...
    // pico_vgaboard_set_palette((const uint16_t *)(&pico_vgaboard_palette_4bpp_space));
    // palette_name = L"SPACE!";

    if (!tile_init())
    {
        return false;
    }
    tile_draw();

    alien_init();
    alien_draw();

    ship_x = rand() % DEMO.w;
    ship_y = rand() % DEMO.h;
    ship_y = DEMO.h / 2 + rand() % (DEMO.h / 2);
    ship_dx = rand() % 2 == 0 ? 1 : -1;
    ship_dy = rand() % 2 == 0 ? 1 : -1;
    // ship_x = DEMO.w / 2 - ship_16x16x4_1.width / 2;
    // ship_y = DEMO.h / 2 - ship_16x16x4_1.height / 2;
    // ship_dx = 0;
    // ship_dy = 0;

    return true;
}

void sprites_done()
{
    free(tile_map);
}

void sprites_draw()
{
    if (DEPTH != 4)
    {
        return;
    }
    ship_counter += 1;
    if (ship_counter % 100 == 0)
    {
        if (ship_dx == 0 && rand() % 5 == 0)
        {
            ship_dx = rand() % 2 == 0 ? 1 : -1;
        }
        else if (rand() % 5 == 0)
        {
            ship_dx = 0;
        }
        else if (rand() % 5 == 0)
        {
            ship_dx = -ship_dx;
        }
        if (ship_dy == 0 && rand() % 5 == 0)
        {
            ship_dy = rand() % 2 == 0 ? 1 : -1;
        }
        else if (rand() % 5 == 0)
        {
            ship_dy = 0;
        }
        else if (rand() % 5 == 0)
        {
            ship_dy = -ship_dy;
        }
    }

    tile_anim();
    tile_draw();

    alien_anim();
    alien_draw();

    // Move and draw ship
    if (ship_x + ship_dx < 0)
    {
        ship_x = 0;
        ship_dx = -ship_dx;
    }
    else if (ship_x + ship_16x16x4_1.width + ship_dx > DEMO.w)
    {
        ship_x = DEMO.w - ship_16x16x4_1.width;
        ship_dx = -ship_dx;
    }
    else
    {
        ship_x += ship_dx;
    }
    if (ship_y + ship_dy < 0)
    {
        ship_y = 0;
        ship_dy = -ship_dy;
    }
    else if (ship_y + ship_16x16x4_1.height + ship_dy > DEMO.h)
    {
        ship_y = DEMO.h - ship_16x16x4_1.height;
        ship_dy = -ship_dy;
    }
    else
    {
        ship_y += ship_dy;
    }
    hagl_blit_xywh_transparent(
        hagl_backend,
        DEMO.x + ship_x, DEMO.y + ship_y,
        ship_16x16x4_1.width, ship_16x16x4_1.height,
        &ship_16x16x4_1,
        _);
    // sleep_ms(500);
}

/* EOF */
