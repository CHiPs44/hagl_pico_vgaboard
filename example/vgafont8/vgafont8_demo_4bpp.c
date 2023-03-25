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

#include "vgafont8.c"
#include "pc_ibm_bios_f08.h"
#include "unscii_f08.h"
#include "unscii_fantasy_f08.h"
#include "unscii_mcr_f08.h"
#include "unscii_thin_f08.h"

const uint8_t *font_bitmaps[6] = {
    pc_ibm_bios_f08,
    unscii_f08,
    unscii_fantasy_f08,
    unscii_mcr_f08,
    unscii_thin_f08,
    NULL,
};

const uint8_t *font_names[6] = {
    "PC IBM BIOS",
    "Unscii",
    "Unscii Fantasy",
    "Unscii MCR",
    "Unscii Thin",
    NULL,
};

void vgafont8_demo_4bpp()
{
    hagl_set_clip(hagl_backend, 0, 0, hagl_backend->width - 1, hagl_backend->height - 1);
    hagl_fill_rectangle_xywh(hagl_backend, 0, 0, hagl_backend->width, hagl_backend->height, 0x04);
    hagl_draw_hline(hagl_backend, 8, 8, hagl_backend->width - 16 - 1, 0x0b);
    hagl_draw_hline(hagl_backend, 8, hagl_backend->height - 8, hagl_backend->width - 16 - 1, 0x0b);
    vgafont8_set_hagl_backend(hagl_backend);
    int font_index = -1;
    int16_t x, y;
    uint16_t w, h;
    uint8_t c;
    uint32_t counter = 0;
    int led = 0;
    char buffer[16];
    while (true)
    {
        if (font_index == -1 || counter % 500 == 0)
        {
            // Cycle through fonts
            font_index += 1;
            if (font_bitmaps[font_index] == NULL)
            {
                font_index = 0;
                printf("Reset font index\n");
            }
            // Display font name
            printf("Font name: %s\n", font_names[font_index]);
            vgafont8_set_font_bitmap(pc_ibm_bios_f08);
            vgafont8_set_background_color(0x0b);
            vgafont8_set_foreground_color(0x04);
            x = 8 * 2;
            y = 8 * 2;
            w = hagl_backend->width - 8 * 4;
            h = 8;
            hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 4, y - 4, w + 8, h + 8, 2, 0x0b);
            vgafont8_put_text(x, y, font_names[font_index]);
            // Display all 256 chars: 8 lines x 32 columns
            vgafont8_set_font_bitmap(font_bitmaps[font_index]);
            vgafont8_set_background_color(0x04);
            vgafont8_set_foreground_color(0x0b);
            for (uint8_t line = 0; line < 8; line++)
            {
                vgafont8_set_background_color((line + font_index) % 8);
                vgafont8_set_foreground_color(15 - (line + font_index) % 8);
                y = 8 * (4 + line);
                for (uint8_t column = 0; column < 32; column++)
                {
                    x = 8 * (4 + column);
                    c = line * 32 + column;
                    // vgafont8_set_background_color(column % 8);
                    // vgafont8_set_foreground_color(8 + column % 8);
                    vgafont8_put_char(x, y, c);
                }
            }
        }
        // vgafont8_set_background_color(get_rand_32() % 16);
        // vgafont8_set_foreground_color(get_rand_32() % 16);
        // vgafont8_put_char(
        //     get_rand_32() % (hagl_backend->width),
        //     get_rand_32() % (hagl_backend->height),
        //     get_rand_32() % 256);
        snprintf(buffer, sizeof(buffer), "%d", counter);
        vgafont8_set_font_bitmap(pc_ibm_bios_f08);
        vgafont8_set_background_color(0x0b);
        vgafont8_set_foreground_color(0x04);
        vgafont8_put_text(0, 0, buffer);
        counter += 1;
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        sleep_ms(10);
        led = 1 - led;
    }
}

// EOF
