/* SPDX-License-Identifier: MIT-0 */

int figure;

bool figures_init()
{
    figure = get_rand_32() % 9;
    // figure = 7 + get_rand_32() % 2;
    return true;
}

/**
 * @brief Draw random lines, squares, rectangles, circles & ellipses
 */
void figures_draw()
{
    uint16_t x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
    uint16_t w, h;
    x0 = window.x + get_rand_32() % (window.w  / 1);
    y0 = window.y + get_rand_32() % (window.h / 2);
    w  = get_rand_32() % (window.w  / 1);
    h  = get_rand_32() % (window.h / 2);
    uint8_t  c = 1 + get_rand_32() % (COLORS  - 1);
    switch (figure)
    {
        case 0: // Line
            hagl_draw_line          (hagl_backend, x0, y0, x0 + w - 1, y0 + h - 1, c);            
            break;    
        case 1: // Horizontal line
            hagl_draw_hline_xyw     (hagl_backend, x0, y0, w                     , c);
            break;
        case 2: // Vertical line
            hagl_draw_vline_xyh     (hagl_backend, x0, y0            , h         , c);
            break;
        case 3: // Square
            hagl_draw_rectangle_xywh(hagl_backend, x0, y0, w         , w         , c);
            break;
        case 4: // Rectangle
            hagl_draw_rectangle_xywh(hagl_backend, x0, y0, w         , h         , c);
            break;
        case 5: // Circle
            hagl_draw_circle        (hagl_backend, x0, y0, w                     , c);
            break;
        case 6: // Ellipse
            hagl_draw_ellipse       (hagl_backend, x0, y0, w         , h         , c);
            break;
        case 7: // Triangle
            x0 = window.x + get_rand_32() % (window.w);
            y0 = window.y + get_rand_32() % (window.h);
            x1 = window.x + get_rand_32() % (window.w);
            y1 = window.y + get_rand_32() % (window.h);
            x2 = window.x + get_rand_32() % (window.w);
            y2 = window.y + get_rand_32() % (window.h);
            hagl_draw_triangle      (hagl_backend, x0, y0, x1, y1, x2, y2        , c);
            break;
        case 8: // Polygon
            x0 = window.x + get_rand_32() % (window.w);
            y0 = window.y + get_rand_32() % (window.h);
            x1 = window.x + get_rand_32() % (window.w);
            y1 = window.y + get_rand_32() % (window.h);
            x2 = window.x + get_rand_32() % (window.w);
            y2 = window.y + get_rand_32() % (window.h);
            x3 = window.x + get_rand_32() % (window.w);
            y3 = window.y + get_rand_32() % (window.h);
            x4 = window.x + get_rand_32() % (window.w);
            y4 = window.y + get_rand_32() % (window.h);
            uint16_t v[5 * 2] = { x0, y0, x1, y1, x2, y2, x3, y3, x4, y4 };
            hagl_draw_polygon       (hagl_backend, 5, v                        , c);
            break;
    }
}

/**
 * @brief Fill random squares, rectangles, circles & ellipses
 */
void figures_fill()
{
    uint16_t x = window.x + get_rand_32() % (window.w  / 1);
    uint16_t y = window.y + get_rand_32() % (window.h / 1);
    uint16_t w = get_rand_32() % (window.w  / 1);
    uint16_t h = get_rand_32() % (window.h / 1);
    uint8_t  c = 1 + get_rand_32() % (COLORS  - 1);
    switch (get_rand_32() % 4)
    {
        case 0: // Filled square
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w        , w        , c);
            break;
        case 1: // Filled rectangle
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w        , h        , c);
            break;
        case 2: // Filled circle
            hagl_fill_circle        (hagl_backend, x, y, w                   , c);
            break;
        case 3: // Filled ellipse
            hagl_fill_ellipse       (hagl_backend, x, y, w        , h        , c);
            break;
    }
}

/* EOF */
