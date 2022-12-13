/* SPDX-License-Identifier: MIT-0 */

void fonts_init()
{
    // Nothing!
}

/**
 * @brief Draw name, digits & accented chars from each registered font
 */
void fonts_draw()
{
    wchar_t text[80];

    int16_t y = 0;
    for (uint8_t i = 0; i < NFONTS; i++)
    {
        swprintf(
            text, sizeof(text), 
            L"%-16ls 0123456789 ÄäÂâÉéÊêÈèÇç", 
            FONTS[i]->name
        );
        hagl_put_text(
            hagl_backend, text, 
            window.x, window.y + y, 
            COLORS - 1, FONTS[i]->fontx
        );
        y += FONTS[i]->h + 2;
    }
}

/* EOF */
