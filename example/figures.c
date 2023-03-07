/* SPDX-License-Identifier: MIT-0 */

int figure;

void figures_init()
{
    figure = rand() % 9;
    // figure = 7 + rand() % 2;
}

/**
 * @brief Draw random lines, squares, rectangles, circles & ellipses
 */
bool figures_draw()
{
    uint16_t x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
    uint16_t w, h;
    x0 = window.x + rand() % (window.w  / 1);
    y0 = window.y + rand() % (window.h / 2);
    w  = rand() % (window.w  / 1);
    h  = rand() % (window.h / 2);
    uint8_t  c = 1 + rand() % (COLORS  - 1);
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
            x0 = window.x + rand() % (window.w);
            y0 = window.y + rand() % (window.h);
            x1 = window.x + rand() % (window.w);
            y1 = window.y + rand() % (window.h);
            x2 = window.x + rand() % (window.w);
            y2 = window.y + rand() % (window.h);
            hagl_draw_triangle      (hagl_backend, x0, y0, x1, y1, x2, y2        , c);
            break;
        case 8: // Polygon
            x0 = window.x + rand() % (window.w);
            y0 = window.y + rand() % (window.h);
            x1 = window.x + rand() % (window.w);
            y1 = window.y + rand() % (window.h);
            x2 = window.x + rand() % (window.w);
            y2 = window.y + rand() % (window.h);
            x3 = window.x + rand() % (window.w);
            y3 = window.y + rand() % (window.h);
            x4 = window.x + rand() % (window.w);
            y4 = window.y + rand() % (window.h);
            uint16_t v[5 * 2] = { x0, y0, x1, y1, x2, y2, x3, y3, x4, y4 };
            hagl_draw_polygon       (hagl_backend, 5, v                        , c);
            break;
    }
    return true;
}

/**
 * @brief Fill random squares, rectangles, circles & ellipses
 */
void figures_fill()
{
    uint16_t x = window.x + rand() % (window.w  / 1);
    uint16_t y = window.y + rand() % (window.h / 1);
    uint16_t w = rand() % (window.w  / 1);
    uint16_t h = rand() % (window.h / 1);
    uint8_t  c = 1 + rand() % (COLORS  - 1);
    switch (rand() % 4)
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
