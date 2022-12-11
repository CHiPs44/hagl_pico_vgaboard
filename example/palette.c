/* SPDX-License-Identifier: MIT-0 */

/**
 * @brief Framed tile + index + RGB values for each color in the palette
 */
void palette_draw(rect_t *window, color_t frame_color, color_t text_color)
{
    font_t *font = &FONT5X8;
    wchar_t text[20];
    int16_t x, y, w, h;

    switch (COLORS)
    {
    case 2:
        // 1 line of 2 columns
        // TODO!
        break;
    case 4:
        // 2 lines of 2 columns
        // TODO!
        break;
    case 16:
        // 8 lines of 2 columns
        w = window->w / 2;
        h = window->h / 8 - 1; //HEIGHT % 100 == 0 ? 1 + window->h / 10 : 1 + window->h / 12;
        for (color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window->x + (c / 8) * w;
            uint16_t y = window->y + (c % 8) * h;
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, c==frame_color ? ~frame_color : frame_color);
            color_t rgb = vgaboard_get_palette_color(c);
            uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb) << 3;
            uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb) << 3;
            uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb) << 3;
            swprintf(text, sizeof(text), L"%02d\u2192%02X%02X%02X", c, r, g, b);
            // \u2192 (Unicode right arrow)
            hagl_put_text(hagl_backend, text, x + font->w, y + (h - font->h + 1) / 2, text_color, font->fontx);
        }
        break;
    case 256:
        // 16 lines of 16 columns, without text
        w = window->w / 16;
        h = w * HEIGHT / WIDTH;
        for (uint16_t c = 0; c < COLORS; c++)
        {
            x = window->x + (c / 16) * (w);
            y = window->y + (c % 16) * (h);
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
        }
        break;
    case 65536:
        // 32768 => 256 lines of 128 columns or 
        //          128 lines of 256 columns, 
        //          without text
        // TEST!
        for (uint8_t r = 0; r < 32; r++)
        {
            for (uint8_t g = 0; g < 32; g++)
            {
                for (uint8_t b = 0; b < 32; b++)
                {
                    uint16_t toto = r << 10 | g << 5 | b;
                    int16_t x = window->w > window->h ? toto / 128 : toto / 256;
                    int16_t y = window->w > window->h ? toto % 256 : toto % 128;
                    color_t c = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r << 3, g << 3, b << 3);
                    hagl_put_pixel(hagl_backend, window->x + x, window->y + y, c);
                }
            }
        }
        break;
    default:
        break;
    }
}

/* EOF */
