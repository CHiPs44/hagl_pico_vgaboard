/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw framed filled rects in a matrix 
 */
void rects_draw(rect_t *window)
{
    uint8_t  columns = 8;
    uint8_t  lines   = window->h % 10 == 0 ? 10 : 8;
    uint16_t w       = window->w / columns;
    uint16_t h       = window->h / lines;
    uint16_t x       = w * (rand() % columns);
    uint16_t y       = h * (rand() % lines  );
    uint8_t  c       = 1 + rand() % (COLORS  - 1);
    hagl_fill_rectangle_xywh(hagl_backend, window->x + x, window->y + y, w, h,  c);
    hagl_draw_rectangle_xywh(hagl_backend, window->x + x, window->y + y, w, h, ~c);
}

/* EOF */
