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

#include <pico/version.h>

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
#define NLABELS 11
wchar_t *labels[NLABELS];
wchar_t  values[NLABELS][20];
wchar_t  _specs_scroller[NLABELS * (20 + 20)];
wchar_t  *specs_scroller = _specs_scroller;

hagl_color_t tile1_bitmap[] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0xc, 0xe,
    0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0xe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};
hagl_bitmap_t tile1 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile1_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile1_bitmap
};

hagl_color_t tile2_bitmap[] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xd, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xd, 0xc, 0xd, 0x0,
    0x0, 0x0, 0x0, 0xe, 0xe, 0xd, 0x0, 0x0,
    0x0, 0x0, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0,
};
hagl_bitmap_t tile2 = {
    .width  =  8,
    .height =  8,
    .depth  =  4, // bits
    .pitch  = 16, // 8 * 2, bytes per row
    .size   = sizeof(tile2_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile2_bitmap
};

#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY

void specs_text(uint16_t x0, uint16_t y0, wchar_t *text, hagl_char_style_t *style)
{
    hagl_color_t foreground_color = style->foreground_color;
    style->background_color = 0;
    /* Shadow text */
    style->foreground_color = 0;
    hagl_put_text_styled(hagl_backend, text, x0 - 1, y0 - 1, style);
    hagl_put_text_styled(hagl_backend, text, x0 - 1, y0 + 0, style);
    hagl_put_text_styled(hagl_backend, text, x0 + 1, y0 - 0, style);
    hagl_put_text_styled(hagl_backend, text, x0 - 0, y0 + 1, style);
    hagl_put_text_styled(hagl_backend, text, x0 + 0, y0 - 1, style);
    hagl_put_text_styled(hagl_backend, text, x0 + 1, y0 + 1, style);
    /* Real text */
    style->foreground_color = foreground_color;
    hagl_put_text_styled(hagl_backend, text, x0, y0, style);
}

#endif

void specs_calc(bool for_scroller)
{
    /* LABELS */
    //                            12345678901234567                        12345678901      1234
    labels[ 0] = for_scroller ? L"VGA mode"          : (window.w > 160 ? L"VGA MODE   " : L"MODE");
    labels[ 1] = for_scroller ? L"Horizontal clock"  : (window.w > 160 ? L"HORIZ. CLK " : L"HORZ");
    labels[ 2] = for_scroller ? L"Vertical refresh " : (window.w > 160 ? L"V. REFRESH " : L"VERT");
    labels[ 3] = for_scroller ? L"Display mode"      : (window.w > 160 ? L"DISP. MODE " : L"DISP");
    labels[ 4] = for_scroller ? L"BPP / colors "     : (window.w > 160 ? L"BPP/COLORS " : L"BPP ");
    labels[ 5] = for_scroller ? L"Framebuffer"       : (window.w > 160 ? L"FRAMEBUFFER" : L"FBUF");
    labels[ 6] = for_scroller ? L"System clock"      : (window.w > 160 ? L"SYST. CLK  " : L"SCLK");
    labels[ 7] = for_scroller ? L"Voltage regulator" : (window.w > 160 ? L"VOLTAGE REG" : L"VREG");
    labels[ 8] = for_scroller ? L"Free memory"       : (window.w > 160 ? L"FREE RAM   " : L"FREE");
    labels[ 9] = for_scroller ? L"Palette"           : (window.w > 160 ? L"PALETTE    " : L"PAL ");
    labels[10] = for_scroller ? L"Pico SDK"          : (window.w > 160 ? L"PICO SDK   " : L"SDK ");
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
    swprintf(values[ 0], sizeof(values[ 0]), L"%dx%d"   , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[ 1], sizeof(values[ 1]), L"%d MHz"  , vgaboard->scanvideo_mode->default_timing->clock_freq / 1000 / 1000);
    swprintf(values[ 2], sizeof(values[ 2]), L"%d Hz"   , vgaboard->freq_hz);
    swprintf(values[ 3], sizeof(values[ 3]), L"%dx%d"   , WIDTH, HEIGHT);
    swprintf(values[ 4], sizeof(values[ 4]), L"%d/%d"   , DEPTH, COLORS);
    swprintf(values[ 5], sizeof(values[ 5]), L"%d/%d"   , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[ 6], sizeof(values[ 6]), L"%d MHz"  , clock_get_hz(clk_sys) / 1000 / 1000);
    swprintf(values[ 7], sizeof(values[ 7]), L"%ls V"   , vreg_voltage);
    swprintf(values[ 8], sizeof(values[ 8]), L"%d/256"  , get_free_ram());
    swprintf(values[ 9], sizeof(values[ 9]), L"%ls"     , DEPTH==16 ? L"N/A" : palette_name);
    swprintf(values[10], sizeof(values[10]), L"v%s"     , PICO_SDK_VERSION_STRING);
    if (for_scroller) {
        wchar_t buffer[40];
        for (int i = 0; i < NLABELS; i++)
        {
            swprintf(buffer, 40, L"%ls: %ls ", labels[i], values[i]);
            wcsncat(specs_scroller, buffer, 40);
        }
    }
}

/**
 * @brief Draw specs of current VGA mode
 */
bool specs_init()
{
    window.x = DEMO.x;
    window.y = DEMO.y;
    window.w = DEMO.w;
    window.h = DEMO.h;
    const font_t *font = &FONT8X8;
    hagl_color_t color1, color2, color3, color4;
    if (DEPTH == 1) {
        color1 = 1;
        color2 = 1;
        color3 = 1;
        color4 = 1;
    } else if (DEPTH == 2) {
        color1 = 1;
        color2 = 2;
        color3 = 3;
        color4 = 1;
    } else {
        color1 = 1 + rand() % (COLORS - 1);
        do {
            color2 = 1 + rand() % (COLORS - 1);
        } while (color2 == color1);
        do {
            color3 = 1 + rand() % (COLORS - 1);
        } while (color3 == color1 || color3 == color2);
        do {
            color4 = 1 + rand() % (COLORS - 1);
        } while (color4 == color1 || color4 == color2 || color4 == color3);
    }
    hagl_color_t colors[4] = { color1, color2, color3, color4 };
    uint16_t x0, y0, x1, y1;
    /* TILED BACKGROUND IN 4BPP MODE */
    if (DEPTH==4) {
        int zoom = 1;
        for (int row = 0; row < window.h / tile1.height / zoom; row++)
        {
            for (int col = 0; col < window.w / tile1.width / zoom; col++)
            {
                hagl_blit_xywh(
                    hagl_backend, 
                    window.x + col * tile1.width * zoom, 
                    window.y + row * tile1.height * zoom, 
                    tile1.width * zoom, 
                    tile1.height * zoom, 
                    (row + col) % 2 == 0 ? &tile1 : &tile2
                );
            }
        }        
    }
    /* TITLE LINES */
    /*                            1234567890123456789      1234567890 */
    lines[0] = window.w > 160 ? L"Raspberry Pi Pico"   : L"RPi Pico"  ;
    lines[1] = window.w > 160 ? L"VGA Demo Board"      : L"VGA Board" ;
    lines[2] = window.w > 160 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL"  ;
    lines[3] = window.w > 160 ? L"github.com/CHiPs44"  : L"by CHiPs44";
    y0 = window.y;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_char_style_t style1 = {
        .font = font->fontx,
        .background_color = 0,
        .foreground_color = 0,
        .mode = HAGL_CHAR_MODE_TRANSPARENT,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
    bool compact = window.h / (font->h * style1.scale_y_numerator / style1.scale_y_denominator) <= NLINES + NLABELS;
#else
    bool compact = window.h / font->h <= NLINES + NLABELS;
#endif
    if (!compact) { 
        y0 += font->h;
    }
    for (uint8_t i = 0; i < NLINES; i += 1)
    {
        size_t len = wcslen(lines[i]);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        x0 = window.x + (window.w - font->w * len * style1.scale_x_numerator / style1.scale_x_denominator) / 2;
        style1.foreground_color = colors[i % 4];
        specs_text(x0, y0, lines[i], &style1);
        y0 += font->h * style1.scale_y_numerator / style1.scale_y_denominator;
#else
        x0 = window.x + (window.w - font->w * len) / 2;
        hagl_put_text(hagl_backend, lines[i], x0, y0, colors[i % 4], font->fontx);
        y0 += font->h;
#endif
    }
    if (!compact) { 
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        y0 += font->h * style1.scale_y_numerator / style1.scale_y_denominator;
#else
        y0 += font->h;
#endif
    }
    /* DISPLAY LABELS & VALUES */
    specs_calc(false);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_char_style_t style2 = {
        .font = font->fontx,
        .background_color = 0,
        .foreground_color = COLORS - 1,
        .mode = HAGL_CHAR_MODE_TRANSPARENT,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
    x0 = window.x + font->w * style2.scale_x_numerator / style2.scale_x_denominator;
#else
    x0 = window.x + font->w;
#endif
    for(uint8_t i = 0; i < NLABELS; i += 1)
    {
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        x1 = x0 + (wcslen(labels[i]) + 1) * font->w * style2.scale_x_denominator / style2.scale_y_denominator;
        y1 = y0 + i * font->h * style2.scale_y_numerator / style2.scale_y_denominator;
        style2.foreground_color = colors[i % 4];
        specs_text(x0, y1, labels[i], &style2);
        specs_text(x1, y1, values[i], &style2);
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
    return true;
}

void specs_draw()
{
    // Nothing!
}

/* EOF */
