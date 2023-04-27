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

/* Stack space is precious */
wchar_t line1[80], line2[80];

/**
 * @brief Draw name, digits & some accented chars for each registered font
 */
bool fonts_init()
{
    uint16_t x = 2;
    uint16_t y = 2;
    for (uint8_t i = 0; i < NFONTS; i++)
    {
        hagl_color_t color = 1 + get_rand_32() % (COLORS - 1);
        swprintf(line1, sizeof(line1), L"%ls", FONTS[i]->name);
        swprintf(line2, sizeof(line2), L"0123456789 AaEeIiMmWw ÄäÂâÉéÊêÈèÏÎïîÖÔöôÜüÿŸÇç");
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        hagl_char_style_t style = {
            .font = FONTS[i]->fontx,
            .background_color = 0,
            .foreground_color = color,
            .mode = HAGL_CHAR_MODE_REVERSE,
            .scale_x_numerator = 1, .scale_x_denominator = 1,
            .scale_y_numerator = 1, .scale_y_denominator = 1,
        };
        hagl_put_text_styled(hagl_backend, line1, window.x + x, window.y + y, &style);
        y += FONTS[i]->h * style.scale_y_numerator / style.scale_y_denominator + 2;
        style.mode = HAGL_CHAR_MODE_OPAQUE;
        hagl_put_text_styled(hagl_backend, line2, window.x + x, window.y + y, &style);
        y += FONTS[i]->h * style.scale_y_numerator / style.scale_y_denominator + 2;
#else
        hagl_put_text(hagl_backend, line1, window.x + x, window.y + y, color, FONTS[i]->fontx);
        y += FONTS[i]->h + 2;
        hagl_put_text(hagl_backend, line2, window.x + x, window.y + y, color, FONTS[i]->fontx);
        y += FONTS[i]->h + 2;
#endif
    }
    return true;
}

void fonts_draw()
{
    // Nothing!
}

/* EOF */
