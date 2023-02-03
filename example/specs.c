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

#include "hagl/char.h"

/**
 * @brief Get free RAM using malloc() and dichotomy
 * 
 * @return size_t 
 */
size_t get_free_ram()
{
    size_t left = 0, right = 512 * 1024, middle;
    void *ptr;
    while (left < right) {
        middle = (left + right) / 2;
        // printf("Trying to allocate %d bytes\r\n", middle);
        // sleep_ms(250);
        ptr = malloc(middle);
        if (ptr) {
            left = middle + 1;
            free(ptr);
        } else {
            right = middle;
        }
    }

    return left;
}

/* Make them global since they seem to don't fit into stack anymore */
#define NLINES 4
wchar_t *lines[NLINES];
#define NLABELS 10
wchar_t *labels[NLABELS];
wchar_t  values[NLABELS][20];

/**
 * @brief Draw specs of current VGA mode
 */
void specs_init()
{
    window.x = DEMO.x;
    window.y = DEMO.y;
    window.w = DEMO.w;
    window.h = DEMO.h;
    const font_t *font = &FONT8X8;
    color_t color1 = DEPTH==1 ? 1 : 1 + rand() % (COLORS - 1);
    color_t color2 = DEPTH==1 ? 1 : 1 + rand() % (COLORS - 1);
    color_t color3 = DEPTH==1 ? 1 : 1 + rand() % (COLORS - 1);
    color_t color4 = DEPTH==1 ? 1 : 1 + rand() % (COLORS - 1);
    uint16_t x0, y0, x1, y1;
    /* TITLE LINES */
    //                            1234567890123456789      1234567890
    lines[0] = window.w > 160 ? L"Raspberry Pi Pico"   : L"RPi Pico"  ;
    lines[1] = window.w > 160 ? L"VGA Demo Board"      : L"VGA Board" ;
    lines[2] = window.w > 160 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL"  ;
    lines[3] = window.w > 160 ? L"github.com/CHiPs44"  : L"by CHiPs44";
    color_t colors[4] = { color1, color2, color3, color4 };
    y0 = window.y;
    bool compact = window.h / font->h <= NLINES + NLABELS;
#ifdef HAGL_HAS_STYLED_TEXT
    hagl_char_style_t style1 = {
        .font = font->fontx,
        .background_color = 0,
        .foreground_color = 0,
        .mode = HAGL_CHAR_MODE_OPAQUE,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
#endif
    if (!compact) { 
        y0 += font->h;
    }
    for (uint8_t i = 0; i < NLINES; i += 1)
    {
        size_t len = wcslen(lines[i]);
#ifdef HAGL_HAS_STYLED_TEXT
        style1.background_color = DEPTH==1 ? 0 : colors[0];
        style1.foreground_color = colors[1 + (i % 2)];
        x0 = window.x + (window.w - font->w * len * style1.scale_x_numerator / style1.scale_x_denominator) / 2;
        hagl_put_text_styled(hagl_backend, lines[i], x0, y0, &style1);
        y0 += font->h * style1.scale_y_numerator / style1.scale_y_denominator;
#else
        x0 = window.x + (window.w - font->w * len) / 2;
        hagl_put_text       (hagl_backend, lines[i], x0, y0, colors[i % 3], font->fontx);
        y0 += font->h;
#endif
        // wprintf(L"Line %d: %ls\r\n", i, lines[i]);
    }
    if (!compact) { 
        y0 += font->h;
    }
    /* LABELS */
    //                             1234567890123456      12345678
    labels[0] = window.w > 160 ? L"BASE VGA MODE   " : L"VGA MODE";
    labels[1] = window.w > 160 ? L"HORIZONTAL CLOCK" : L"HORZ CLK";
    labels[2] = window.w > 160 ? L"VERTICAL REFRESH" : L"VERT REF";
    labels[3] = window.w > 160 ? L"DISPLAY MODE    " : L"DISPLAY ";
    labels[4] = window.w > 160 ? L"BPP / COLORS    " : L"BPP/COLS";
    labels[5] = window.w > 160 ? L"FRAMEBUFFER SIZE" : L"FRAMEBUF";
    labels[6] = window.w > 160 ? L"SYSTEM CLOCK    " : L"SYST CLK";
    labels[7] = window.w > 160 ? L"VREG VOLTAGE    " : L"VOLT REG";
    labels[8] = window.w > 160 ? L"FREE MEMORY     " : L"FREE MEM";
    labels[9] = window.w > 160 ? L"PALETTE NAME    " : L"PALETTE ";
    /* VALUES */
    wchar_t *vreg_voltage;
    int vreg = vgaboard->vreg_voltage;
    if (vreg==0) {
        vreg = VREG_VOLTAGE_DEFAULT;
    }
    switch (vreg)
    {
        case VREG_VOLTAGE_0_85: vreg_voltage = L"0.85"; break;
        case VREG_VOLTAGE_0_90: vreg_voltage = L"0.90"; break;
        case VREG_VOLTAGE_0_95: vreg_voltage = L"0.95"; break;
        case VREG_VOLTAGE_1_00: vreg_voltage = L"1.00"; break;
        case VREG_VOLTAGE_1_05: vreg_voltage = L"1.05"; break;
        case VREG_VOLTAGE_1_10: vreg_voltage = L"1.10"; break;
        case VREG_VOLTAGE_1_15: vreg_voltage = L"1.15"; break;
        case VREG_VOLTAGE_1_20: vreg_voltage = L"1.20"; break;
        case VREG_VOLTAGE_1_25: vreg_voltage = L"1.25"; break;
        case VREG_VOLTAGE_1_30: vreg_voltage = L"1.30"; break;
        default:                vreg_voltage = L"?.??";
    }
    swprintf(values[0], sizeof(values[0]), L"%dx%d"   , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[1], sizeof(values[1]), L"%d MHz"  , vgaboard->scanvideo_mode->default_timing->clock_freq / 1000 / 1000);
    swprintf(values[2], sizeof(values[2]), L"%d Hz"   , vgaboard->freq_hz);
    swprintf(values[3], sizeof(values[3]), L"%dx%d"   , WIDTH, HEIGHT);
    swprintf(values[4], sizeof(values[4]), L"%d/%d"   , DEPTH, COLORS);
    swprintf(values[5], sizeof(values[5]), L"%d/%d"   , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[6], sizeof(values[6]), L"%d MHz"  , clock_get_hz(clk_sys) / 1000 / 1000);
    swprintf(values[7], sizeof(values[7]), L"%ls V"   , vreg_voltage);
    swprintf(values[8], sizeof(values[8]), L"%d/256kb", get_free_ram());
    swprintf(values[9], sizeof(values[9]), L"%ls"     , DEPTH==16 ? L"N/A" : palette_name);
    /* DISPLAY LABELS & VALUES */
    x0 = window.x;// + font->w;
#ifdef HAGL_HAS_STYLED_TEXT
    hagl_char_style_t style2 = {
        .font = font->fontx,
        .background_color = DEPTH==1 ? 0 : color1,
        .foreground_color = color2,
        .mode = HAGL_CHAR_MODE_OPAQUE,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
#endif
    for(uint8_t i = 0; i < NLABELS; i += 1)
    {
#ifdef HAGL_HAS_STYLED_TEXT
        x1 = x0 + (wcslen(labels[i]) + 1) * font->w * style2.scale_x_denominator / style2.scale_y_denominator;
        y1 = y0 + i * font->h * style2.scale_y_numerator / style2.scale_y_denominator;
        style2.mode = HAGL_CHAR_MODE_OPAQUE;
        hagl_put_text_styled(hagl_backend, labels[i], x0, y1, &style2);
        style2.mode = HAGL_CHAR_MODE_REVERSE;
        hagl_put_text_styled(hagl_backend, values[i], x1, y1, &style2);
#else
        x1 = x0 + (wcslen(labels[i]) + 1) * font->w;
        y1 = y0 + i * font->h;
        hagl_put_text(hagl_backend, labels[i], x0, y1, color1, font->fontx);
        hagl_put_text(hagl_backend, values[i], x1, y1, color2, font->fontx);
#endif
        // wprintf(
        //     L"spec #%d: label=%ls value=%ls\r\n", 
        //     i, labels[i], values[i]
        // );
    }
}

void specs_draw()
{
    // Nothing!
}

/* EOF */
