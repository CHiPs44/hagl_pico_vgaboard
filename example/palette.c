/*

MIT No Attribution

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

hagl_color_t palette_frame_color;
hagl_color_t palette_text_color;
wchar_t palette_text[20];
font_t *palette_font = &FONT5X8; // &FONT8X8;
wchar_t palette_separator = L'\u2192'; // \u2192 => Unicode right arrow
// wchar_t palette_separator = ':';
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
hagl_char_style_t palette_style = {
    .mode = HAGL_CHAR_MODE_TRANSPARENT,
    .scale_x_numerator = 1,
    .scale_x_denominator = 1,
    .scale_y_numerator = 1,
    .scale_y_denominator = 1,
};
#endif

void palette_draw_color(hagl_color_t color, int16_t x, int16_t y, int16_t w, int16_t h)
{
    hagl_color_t rgb;
    uint8_t r, g, b;
    hagl_color_t frame_color = color == palette_frame_color ? ~palette_frame_color : palette_frame_color;
    hagl_color_t text_color = color == palette_text_color ? ~palette_text_color : palette_text_color;
    hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, color);
    hagl_draw_rectangle_xywh(hagl_backend, x, y, w, h, frame_color);
    rgb = vgaboard_get_palette_color(color);
    r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb) << 3;
    g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb) << 3;
    b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb) << 3;
    // CC->RRGGBB
    swprintf(palette_text, sizeof(palette_text), L"%02d%lc%02X%02X%02X", color, palette_separator, r, g, b);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_put_text_styled(hagl_backend, palette_text, x + palette_font->w, y + (h - palette_font->h + 1) / 2, &palette_style);
#else
    hagl_put_text(hagl_backend, palette_text, x + palette_font->w, y + (h - palette_font->h + 1) / 2, text_color, &FONT5X8);
#endif
}

/**
 * @brief Framed tile + index + RGB values for each color in the palette
 */
bool palette_init()
{
    palette_frame_color = COLORS - 1;
    palette_text_color = COLORS - 1;
    int16_t x, y, w, h;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    palette_style.font = palette_font->fontx;
    palette_style.background_color = COLORS - 1;
    palette_style.foreground_color = 0;
#endif
    switch (DEPTH)
    {
    case 1:
        // 1 line of 2 columns
        w = window.w / 2;
        h = window.h - 8;
        uint16_t y = window.y + palette_font->h;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_put_text_styled(hagl_backend, palette_name, window.x, window.y, &palette_style);
#else
        hagl_put_text(hagl_backend, palette_name, window.x, window.y, COLORS - 1, palette_font->fontx);
#endif
        for (hagl_color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c / 2) * w;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 2:
        // 2 lines of 2 columns
        w = window.w / 2;
        h = (window.h - palette_font->h) / 2 - 1;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_put_text_styled(hagl_backend, palette_name, window.x, window.y, &palette_style);
#else
        hagl_put_text(hagl_backend, palette_name, window.x, window.y, COLORS - 1, palette_font->fontx);
#endif
        for (hagl_color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c / 2) * w;
            uint16_t y = window.y + 8 + (c % 2) * h;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 4:
        // 4 lines of 4 columns
        w = window.w / 4;
        h = (window.h - palette_font->h) / 4 - 1; // HEIGHT % 100 == 0 ? 1 + window.h / 10 : 1 + window.h / 12;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_put_text_styled(hagl_backend, palette_name, window.x, window.y, &palette_style);
#else
        hagl_put_text(hagl_backend, palette_name, window.x, window.y, COLORS - 1, palette_font->fontx);
#endif
        for (hagl_color_t c = 0; c < COLORS; c++)
        {
            uint16_t x = window.x + (c % 4) * w;
            uint16_t y = window.y + palette_font->h + (c / 4) * h;
            palette_draw_color(c, x, y, w, h);
        }
        break;
    case 8:
        // 16 lines of 16 columns, without text
        w = window.w / 16;
        h = (window.h - palette_font->h) / 16;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_put_text_styled(hagl_backend, palette_name, window.x, window.y, &palette_style);
#else
        hagl_put_text(hagl_backend, palette_name, window.x, window.y, COLORS - 1, palette_font->fontx);
#endif
        wchar_t buffer[4];
        for (uint16_t c = 0; c < COLORS; c++)
        {
            x = window.x + w * (c / 16);
            y = window.y + palette_font->h + h * (c % 16);
            hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
            swprintf(buffer, 4, L"%02x", c);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
            palette_style.foreground_color = ~c & 0xff;
            hagl_put_text_styled(hagl_backend, buffer, x + (w - 2 * palette_font->w) / 2, y + (h - palette_font->h) / 2, &palette_style);
#else
            hagl_put_text(hagl_backend, buffer, x + (w - 2 * palette_font->w) / 2, y + (h - palette_font->h) / 2, ~c & 0xff, palette_font->fontx);
#endif
        }
        break;
    case 16:
        // 32768 => 256 lines of 128 columns or
        //          128 lines of 256 columns,
        //          without text
        // TEST!
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_put_text_styled(hagl_backend, palette_name, window.x, window.y, &palette_style);
#else
        hagl_put_text(hagl_backend, palette_name, window.x, window.y, COLORS - 1, palette_font->fontx);
#endif
        for (uint8_t r = 0; r < 32; r++)
        {
            for (uint8_t g = 0; g < 32; g++)
            {
                for (uint8_t b = 0; b < 32; b++)
                {
                    uint16_t rgb = r << 10 | g << 5 | b;
                    int16_t x = window.w > window.h ? rgb / 128 : rgb / 256;
                    int16_t y = window.w > window.h ? rgb % 256 : rgb % 128;
                    hagl_color_t c = PICO_SCANVIDEO_PIXEL_FROM_RGB5(r, g, b);
                    hagl_put_pixel(hagl_backend, window.x + x, window.y + y, c);
                }
            }
        }
        break;
    default:
        break;
    }
    return true;
}

void palette_draw()
{
    // Nothing!
}

/* EOF */
