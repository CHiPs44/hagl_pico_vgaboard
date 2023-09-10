/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw borders and axis of given area
 */
void draw_borders_and_axis(rect_t *demo_window, hagl_color_t color1, hagl_color_t color2, hagl_color_t color3)
{
    clip(demo_window);
    // Borders
    hagl_draw_rectangle_xywh(hagl_backend, 
        demo_window->x, demo_window->y, demo_window->w, demo_window->h, 
        color1
    );
    // Missing pixel at bottom right corner (HAGL off by one bug?)
    hagl_put_pixel(hagl_backend, 
        demo_window->x + demo_window->w - 1, demo_window->y + demo_window->h - 1, 
        color1
    );
    // X axis (with no borders overlap)
    hagl_draw_hline_xyw(hagl_backend, 
        demo_window->x + 1, demo_window->y + demo_window->h / 2 - 1, demo_window->w - 2, 
        color2
    );
    // Y axis (with no borders overlap)
    hagl_draw_vline_xyh(hagl_backend, 
        demo_window->x + demo_window->w / 2 - 1, demo_window->y + 1, demo_window->h - 2, 
        color3
    );
}

/* EOF */
