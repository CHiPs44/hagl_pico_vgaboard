/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw random lines, rectangles, boxes
 */
void draw_figures(rect_t *window)
{
    clip(window);
    uint16_t x = window->x + rand() % window->w;
    uint16_t y = window->y + rand() % window->h;
    uint16_t w = rand() % window->w;
    uint16_t h = rand() % window->h;
    uint8_t  c = 1 + rand() % (COLORS  - 1);
    switch (rand() % 5)
    {
        case 0:
            hagl_draw_line          (hagl_backend, x, y, x + w - 1, y + h - 1, c);            
            break;    
        case 1:
            hagl_draw_hline_xyw     (hagl_backend, x, y, w                   , c);
            break;
        case 2:
            hagl_draw_vline_xyh     (hagl_backend, x, y           , h        , c);
            break;
        case 3:
            hagl_draw_rectangle_xywh(hagl_backend, x, y, w        , h        , c);
            break;
        case 4:
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w        , h        , c);
            break;
    }
}

/* EOF */
