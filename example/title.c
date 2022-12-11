/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw title with frame & shadow
 */
void title_draw(rect_t *window, wchar_t *title, color_t title_color, color_t frame_color, color_t shadow_color)
{
    uint16_t x, y, w, h;
    // wchar_t title[40];
    font_t *font = get_small_font(window);

    w = wcslen(title) * font->w;
    h = font->h;
    x = window->x + (window->w - w) / 2;
    y = window->y + (window->h - h) / 2;
    // Shadow
    hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 4, h + 4, 2, shadow_color);
    // Frame
    hagl_draw_rounded_rectangle_xywh(hagl_backend, x - 2, y - 2, w + 4, h + 4, 2, frame_color );
    // Title
    hagl_put_text(hagl_backend, title, x, y, title_color, font->fontx);
}

/* EOF */
