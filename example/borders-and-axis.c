/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw borders and axis of given area
 */
void draw_borders_and_axis(rect_t *DEMO, hagl_color_t color1, hagl_color_t color2, hagl_color_t color3)
{
    clip(DEMO);
    // Borders
    hagl_draw_rectangle_xywh(hagl_backend, 
        DEMO->x, DEMO->y, DEMO->w, DEMO->h, 
        color1
    );
    // Missing pixel at bottom right corner (HAGL off by one bug?)
    hagl_put_pixel(hagl_backend, 
        DEMO->x + DEMO->w - 1, DEMO->y + DEMO->h - 1, 
        color1
    );
    // X axis (with no borders overlap)
    hagl_draw_hline_xyw(hagl_backend, 
        DEMO->x + 1, DEMO->y + DEMO->h / 2 - 1, DEMO->w - 2, 
        color2
    );
    // Y axis (with no borders overlap)
    hagl_draw_vline_xyh(hagl_backend, 
        DEMO->x + DEMO->w / 2 - 1, DEMO->y + 1, DEMO->h - 2, 
        color3
    );
}

/* EOF */
