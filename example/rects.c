/* SPDX-License-Identifier: MIT-0 */

uint8_t rects_columns;
uint8_t rects_lines;
uint16_t rects_width;
uint16_t rects_height;

bool rects_init()
{
    rects_columns = DEMO.w % 10 == 0 ? 10 : 8;
    rects_lines = DEMO.h % 10 == 0 ? 10 : 8;
    rects_width = DEMO.w / rects_columns;
    rects_height = DEMO.h / rects_lines;
    return true;
}

/**
 * @brief Draw framed filled rects in a matrix
 */
void rects_draw()
{
    uint16_t x = rects_width * (rand() % rects_columns);
    uint16_t y = rects_height * (rand() % rects_lines);
    uint8_t c = 1 + (rand() % (COLORS - 1));
    hagl_fill_rectangle_xywh(hagl_backend, DEMO.x + x, DEMO.y + y, rects_width, rects_height, c);
    hagl_draw_rectangle_xywh(hagl_backend, DEMO.x + x, DEMO.y + y, rects_width, rects_height, ~c);
}

/* EOF */
