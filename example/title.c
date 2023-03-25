/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Draw title (with frame & shadow if window is tall enough) 
 */
void title_draw(rect_t *window, wchar_t *title)
{
    hagl_color_t title_color = 1, frame_color = 1, shadow_color = 1;
    if (DEPTH>1) {
        do {
            title_color  = 1 + get_rand_32() % (COLORS - 1); 
            frame_color  = 1 + get_rand_32() % (COLORS - 1); 
            shadow_color = 1 + get_rand_32() % (COLORS - 1); 
        } while (title_color == frame_color || frame_color == shadow_color);
    }
    uint16_t x, y, w, h;
    font_t *font = get_title_font(&FULL_SCREEN);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_char_style_t style = {
        .font = font->fontx,
        .background_color = DEPTH == 1 ? 0 : shadow_color,
        .foreground_color = title_color,
        .mode = HAGL_CHAR_MODE_OPAQUE,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
    w = wcslen(title) * font->w * style.scale_x_numerator / style.scale_x_denominator;
    h = font->h * style.scale_y_numerator / style.scale_y_denominator;
#else
    w = wcslen(title) * font->w;
    h = font->h;
#endif
    x = window->x + (window->w - w) / 2;
    y = window->y + (window->h - h) / 2;
    if (window->h > h) {
        hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 4, h + 4, 1, shadow_color);
        hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 2, y - 2, w + 4, h + 4, 1, frame_color );
    }
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_put_text_styled(hagl_backend, title, x, y, &style);
#else
    hagl_put_text(hagl_backend, title, x, y, title_color, font->fontx);
#endif
}

/* EOF */
