#include <pico/time.h>

#define __ 0
#define XX 12
#define II 15
#define OO 3

// clang-format off

hagl_color_t alien_16x16x4_0_bitmap[] = {
//  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, // 00
    __, __, __, __, __, __, XX, XX, XX, XX, __, __, __, __, __, __, // 01
    __, __, __, __, __, XX, XX, XX, XX, XX, XX, __, __, __, __, __, // 02
    __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, // 03
    __, __, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, __, __, // 04
    __, XX, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, XX, __, // 05
    __, XX, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, XX, __, // 06
    __, __, XX, XX, XX, II, OO, XX, XX, II, OO, XX, XX, XX, __, __, // 07
    __, __, XX, XX, XX, OO, OO, XX, XX, OO, OO, XX, XX, XX, __, __, // 08
    __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, // 09
    __, __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, __, // 10
    __, __, __, __, XX, XX, XX, XX, XX, XX, XX, __, __, __, __, __, // 11
    __, __, __, __, __, XX, __, __, __, __, XX, __, __, __, __, __, // 12
    __, __, __, __, XX, __, __, __, __, __, __, XX, __, __, __, __, // 13
    __, __, XX, XX, __, __, __, __, __, __, __, __, XX, XX, __, __, // 14
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, // 15
};

hagl_color_t alien_16x16x4_1_bitmap[] = {
//  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, // 00
    __, __, __, __, __, __, XX, XX, XX, XX, __, __, __, __, __, __, // 01
    __, __, __, __, __, XX, XX, XX, XX, XX, XX, __, __, __, __, __, // 02
    __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, // 03
    __, __, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, __, __, // 04
    __, XX, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, XX, __, // 05
    __, XX, XX, XX, XX, II, II, XX, XX, II, II, XX, XX, XX, XX, __, // 06
    __, __, XX, XX, XX, OO, II, XX, XX, OO, II, XX, XX, XX, __, __, // 07
    __, __, XX, XX, XX, OO, OO, XX, XX, OO, OO, XX, XX, XX, __, __, // 08
    __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, // 09
    __, __, __, __, XX, XX, XX, XX, XX, XX, XX, XX, __, __, __, __, // 10
    __, __, __, __, XX, XX, XX, XX, XX, XX, XX, __, __, __, __, __, // 11
    __, __, __, __, __, XX, __, __, __, __, XX, __, __, __, __, __, // 12
    __, __, __, __, XX, __, __, __, __, __, __, XX, __, __, __, __, // 13
    __, __, __, __, __, XX, XX, __, __, XX, XX, __, __, __, __, __, // 14
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, // 15
};

// clang-format on

hagl_bitmap_t alien_16x16x4_0 = {
    .width = 16,
    .height = 16,
    .depth = 4,                             // bits
    .pitch = 32,                            // 16 * 2, bytes per row
    .size = sizeof(alien_16x16x4_0_bitmap), // 16 * 16  = 256 bytes
    .buffer = (uint8_t *)&alien_16x16x4_0_bitmap};

hagl_bitmap_t alien_16x16x4_1 = {
    .width = 16,
    .height = 16,
    .depth = 4,                             // bits
    .pitch = 32,                            // 16 * 2, bytes per row
    .size = sizeof(alien_16x16x4_1_bitmap), // 16 * 16  = 256 bytes
    .buffer = (uint8_t *)&alien_16x16x4_1_bitmap};

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t dx;
    int16_t frame;
} alien_t;

#define ALIENS 64
#define ALIEN_COLS 16
#define ALIEN_ROWS 4
#define ALIEN_SPEED_MS 512
alien_t aliens[ALIENS];
int alien_speed_ms = ALIEN_SPEED_MS;
absolute_time_t alien_timer;
uint alien_counter;

void alien_draw()
{
    for (size_t i = 0; i < ALIENS; i++)
    {
        hagl_bitmap_t *bitmap = aliens[i].frame ? &alien_16x16x4_1 : &alien_16x16x4_0;
        hagl_blit_xywh_transparent(
            hagl_backend,
            DEMO.x + aliens[i].x, DEMO.y + aliens[i].y,
            bitmap->width, bitmap->height,
            bitmap,
            __);
    }
}

void alien_init()
{
    alien_timer = make_timeout_time_ms(alien_speed_ms);
    alien_counter = 0;
    for (size_t col = 0; col < ALIEN_COLS; col++)
    {
        for (size_t row = 0; row < ALIEN_ROWS; row++)
        {
            size_t i = row * ALIEN_COLS + col;
            aliens[i].x = 24 + col * 24;
            aliens[i].y = 24 + row * 24;
            aliens[i].dx = 1;
        }
    }
    alien_draw();
}

void alien_anim()
{
    if (get_absolute_time() > alien_timer)
    {
        alien_timer = make_timeout_time_ms(alien_speed_ms);
        alien_counter++;
        if (alien_counter % 100 == 0 && alien_speed_ms >= (ALIEN_SPEED_MS / 10))
        {
            alien_speed_ms -= ALIEN_SPEED_MS / 10;
        }
        for (size_t i = 0; i < ALIENS; i++)
        {
            aliens[i].frame = 1 - aliens[i].frame;
            if (aliens[i].x + aliens[i].dx > DEMO.w - 24 - 16 || aliens[i].x + aliens[i].dx < 24)
            {
                aliens[i].dx = -aliens[i].dx;
            }
            else
            {
                aliens[i].x += aliens[i].dx;
            }
        }
    }
}
