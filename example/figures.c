/* SPDX-License-Identifier: MIT-0 */

int figure;

bool figures_init()
{
    figure = rand() % 9;
    // figure = 7 + rand() % 2;
    return true;
}

/**
 * @brief Draw random lines, squares, rectangles, circles & ellipses
 */
void figures_draw()
{
    uint16_t x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
    uint16_t w, h;
    x0 = demo_window.x + rand() % (demo_window.w / 1);
    y0 = demo_window.y + rand() % (demo_window.h / 1);
    w  = 8 + rand() % (demo_window.w / 8);
    h  = 8 + rand() % (demo_window.h / 8);
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
            x0 = demo_window.x + rand() % (demo_window.w);
            y0 = demo_window.y + rand() % (demo_window.h);
            x1 = demo_window.x + rand() % (demo_window.w);
            y1 = demo_window.y + rand() % (demo_window.h);
            x2 = demo_window.x + rand() % (demo_window.w);
            y2 = demo_window.y + rand() % (demo_window.h);
            hagl_draw_triangle      (hagl_backend, x0, y0, x1, y1, x2, y2        , c);
            break;
        case 8: // Polygon
            x0 = demo_window.x + rand() % (demo_window.w);
            y0 = demo_window.y + rand() % (demo_window.h);
            x1 = demo_window.x + rand() % (demo_window.w);
            y1 = demo_window.y + rand() % (demo_window.h);
            x2 = demo_window.x + rand() % (demo_window.w);
            y2 = demo_window.y + rand() % (demo_window.h);
            x3 = demo_window.x + rand() % (demo_window.w);
            y3 = demo_window.y + rand() % (demo_window.h);
            x4 = demo_window.x + rand() % (demo_window.w);
            y4 = demo_window.y + rand() % (demo_window.h);
            uint16_t v[5 * 2] = { x0, y0, x1, y1, x2, y2, x3, y3, x4, y4 };
            hagl_draw_polygon       (hagl_backend, 5, v                          , c);
            break;
    }
}

/**
 * @brief Fill random squares, rectangles, circles & ellipses
 */
void figures_fill()
{
    uint16_t x = demo_window.x + rand() % (demo_window.w / 1);
    uint16_t y = demo_window.y + rand() % (demo_window.h / 1);
    uint16_t w = 8 + rand() % (demo_window.w / 8);
    uint16_t h = 8 + rand() % (demo_window.h / 8);
    uint8_t  c = 1 + rand() % (COLORS  - 1);
    switch (rand() % 4)
    {
        case 0:
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, w, c);
            break;
        case 1:
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            break;
        case 2:
            hagl_fill_circle        (hagl_backend, x, y, w   , c);
            break;
        case 3:
            hagl_fill_ellipse       (hagl_backend, x, y, w, h, c);
            break;
    }
}

/* EOF */
