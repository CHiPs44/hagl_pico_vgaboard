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

/**
 * @brief Draw name, digits & some accented chars from each registered font
 */
void fonts_init()
{
    wchar_t text[80];

    int16_t y = 0;
    for (uint8_t i = 0; i < NFONTS; i++)
    {
        // 1234567890123456789012345678901234567890
        // 1234567890123456 1234567890 123456789012
        swprintf(text, sizeof(text), L"%-16ls 0123456789 ÄäÂâÉéÊêÈèÇç", FONTS[i]->name);
        hagl_put_text(hagl_backend, text, window.x, window.y + y, COLORS - 1, FONTS[i]->fontx);
        y += FONTS[i]->h + 2;
    }
}

void fonts_draw()
{
    // Nothing!
}

/* EOF */
