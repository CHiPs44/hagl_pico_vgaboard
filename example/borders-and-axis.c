/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw borders and axis of given area
 */
void draw_borders_and_axis(rect_t *window, hagl_color_t color1, hagl_color_t color2, hagl_color_t color3)
{
    clip(window);
    // Borders
    hagl_draw_rectangle_xywh(hagl_backend, 
        window->x, window->y, window->w, window->h, 
        color1
    );
    // Missing pixel at bottom right corner (HAGL off by one bug?)
    hagl_put_pixel(hagl_backend, 
        window->x + window->w - 1, window->y + window->h - 1, 
        color1
    );
    // X axis (with no borders overlap)
    hagl_draw_hline_xyw(hagl_backend, 
        window->x + 1, window->y + window->h / 2 - 1, window->w - 2, 
        color2
    );
    // Y axis (with no borders overlap)
    hagl_draw_vline_xyh(hagl_backend, 
        window->x + window->w / 2 - 1, window->y + 1, window->h - 2, 
        color3
    );
}

/* EOF */
