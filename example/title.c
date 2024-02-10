/* SPDX-License-Identifier: MIT-0 */

wchar_t title_text[40];
hagl_color_t title_text_color;
hagl_color_t title_frame_color;
hagl_color_t title_shadow_color;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
hagl_char_style_t title_text_style;
#endif

void title_init()
{
    // background color is 0
    switch (DEPTH)
    {
    case 1:
        title_text_color = 1;
        title_frame_color = 1;
        title_shadow_color = 1;
        break;
    case 2:
        title_text_color = 3;
        title_frame_color = 2;
        title_shadow_color = 1;
        break;
    case 4:
    case 8:
        title_text_color = 1 + rand() % (COLORS - 1);
        do
        {
            title_frame_color = 1 + rand() % (COLORS - 1);
            title_shadow_color = 1 + rand() % (COLORS - 1);
        } while (title_text_color == title_frame_color || title_frame_color == title_shadow_color);
        break;
    case 16:
        title_text_color = (rand() % 0xffff) & ~PICO_SCANVIDEO_ALPHA_MASK;
        title_frame_color = (rand() % 0xffff) & ~PICO_SCANVIDEO_ALPHA_MASK;
        title_shadow_color = (rand() % 0xffff) & ~PICO_SCANVIDEO_ALPHA_MASK;
        break;
    default:
        // defaults to monochrome
        break;
    }
}

/**
 * @brief Draw title (with frame & shadow if window is tall enough)
 */
void title_draw()
{
    int16_t x, y, w, h;
    font_t *font = TITLE.h >= 16 ? &FONT8X13B : &FONT8X8;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    title_text_style.font = font->fontx;
    title_text_style.background_color = DEPTH == 1 ? 0 : title_shadow_color;
    title_text_style.foreground_color = title_text_color;
    title_text_style.mode = HAGL_CHAR_MODE_OPAQUE;
    title_text_style.scale_x_numerator = 1;
    title_text_style.scale_x_denominator = 1;
    title_text_style.scale_y_numerator = 1;
    title_text_style.scale_y_denominator = 1;
    w = wcslen(title_text) * font->w * title_text_style.scale_x_numerator / title_text_style.scale_x_denominator;
    h = font->h * title_text_style.scale_y_numerator / title_text_style.scale_y_denominator;
#else
    w = wcslen(title_text) * font->w;
    h = font->h;
#endif
    x = TITLE.x + (TITLE.w - w) / 2;
    if (TITLE.h > h + 4)
    {
        y = TITLE.y + (TITLE.h - h) / 2;
        hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 4, h + 4, 1, title_shadow_color);
        hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 2, y - 2, w + 4, h + 4, 1, title_frame_color);
    }
    else
    {
        y = TITLE.y;
    }
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    title_text_style.foreground_color = title_text_color;
    hagl_put_text_styled(hagl_backend, title_text, x, y, &title_text_style);
#else
    hagl_put_text(hagl_backend, title_text, x, y, title_text_color, font->fontx);
#endif
}

/* EOF */
