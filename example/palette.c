/* SPDX-License-Identifier: MIT-0 */

color_t palette_frame_color;
color_t palette_text_color;
wchar_t palette_text[20];

void palette_draw_color(color_t color, int16_t x, int16_t y, int16_t w, int16_t h)
{
    font_t *font = &FONT5X8;
    color_t rgb;
    uint8_t r, g, b;

    hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, color);
    hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, color==palette_frame_color ? ~palette_frame_color : palette_frame_color);
    rgb = vgaboard_get_palette_color(color);
    r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb) << 3;
    g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb) << 3;
    b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb) << 3;
    // CC->RRGGBB
    swprintf(palette_text, sizeof(palette_text), L"%02d\u2192%02X%02X%02X", color, r, g, b);
    // \u2192 (Unicode right arrow)
    hagl_put_text(hagl_backend, palette_text, x + font->w, y + (h - font->h + 1) / 2, palette_text_color, font->fontx);
}

/**
 * @brief Framed tile + index + RGB values for each color in the palette
 */
void palette_init()
{
    palette_frame_color = COLORS - 1;
    palette_text_color  = COLORS - 1;
    int16_t x, y, w, h;

    switch (COLORS)
    {
    case 2:
        // 1 line of 2 columns
        w = window.w / 2;
        h = window.h;
        uint16_t y = window.y;
        for (color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c / 2) * w;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 4:
        // 2 lines of 2 columns
        w = window.w / 2;
        h = window.h / 2 - 1;
        for (color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c / 2) * w;
            uint16_t y = window.y + (c % 2) * h;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 16:
        // 8 lines of 2 columns
        w = window.w / 2;
        h = window.h / 8 - 1; //HEIGHT % 100 == 0 ? 1 + window.h / 10 : 1 + window.h / 12;
        for (color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c / 8) * w;
            uint16_t y = window.y + (c % 8) * h;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 256:
        // 16 lines of 16 columns, without text
        w = window.w / 16;
        h = w * HEIGHT / WIDTH;
        for (uint16_t c = 0; c < COLORS; c++)
        {
            x = window.x + (c / 16) * (w);
            y = window.y + (c % 16) * (h);
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
                    int16_t x = window.w > window.h ? toto / 128 : toto / 256;
                    int16_t y = window.w > window.h ? toto % 256 : toto % 128;
                    color_t c = PICO_SCANVIDEO_PIXEL_FROM_RGB5(r, g, b);
                    hagl_put_pixel(hagl_backend, window.x + x, window.y + y, c);
                }
            }
        }
        break;
    default:
        break;
    }
}

void palette_draw()
{
    // Nothing!
}

/* EOF */
