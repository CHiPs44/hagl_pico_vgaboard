/* SPDX-License-Identifier: MIT-0 */

#include "hagl/char.h"

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
    const wchar_t *lines[] = {
        //                 1234567890123456789      1234567890
        window.w > 160 ? L"Raspberry Pi Pico"   : L"RPi Pico"  ,
        window.w > 160 ? L"VGA Demo Board"      : L"VGA Board" ,
        window.w > 160 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL"  ,
        window.w > 160 ? L"github.com/CHiPs44"  : L"by CHiPs44",
    };
    const size_t nlines = sizeof(lines) / sizeof(wchar_t *);
    color_t colors[4] = { color1, color2, color3, color4 };
    /* LABELS */
    wchar_t *labels[] = {
        //                 1234567890123456      1234
        window.w > 160 ? L"BASE MODE......." : L"BASE",
        window.w > 160 ? L"HORIZONTAL CLOCK" : L"HCLK",
        window.w > 160 ? L"VERTICAL REFRESH" : L"VREF",
        window.w > 160 ? L"VIEW MODE......." : L"VIEW",
        window.w > 160 ? L"BPP / COLORS...." : L" B/C",
        window.w > 160 ? L"FRAMEBUFFER SIZE" : L"VRAM",
        window.w > 160 ? L"SYSTEM CLOCK...." : L"SCLK",
        window.w > 160 ? L"VREG VOLTAGE...." : L"VREG",
    };
    const size_t nlabels = sizeof(labels) / sizeof(wchar_t *);
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
    wchar_t values[sizeof(labels)][20];
    //                                       1234567890123
    swprintf(values[0], sizeof(values[0]), L"%dx%d" , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[1], sizeof(values[1]), L"%d MHz", vgaboard->scanvideo_mode->default_timing->clock_freq / 1000 / 1000);
    swprintf(values[2], sizeof(values[2]), L"%d Hz" , vgaboard->freq_hz);
    swprintf(values[3], sizeof(values[3]), L"%dx%d" , WIDTH, HEIGHT);
    swprintf(values[4], sizeof(values[4]), L"%d/%d" , DEPTH, COLORS);
    swprintf(values[5], sizeof(values[5]), L"%d/%d" , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[6], sizeof(values[6]), L"%d MHz", clock_get_hz(clk_sys) / 1000 / 1000);
    swprintf(values[7], sizeof(values[7]), L"%ls V" , vreg_voltage);
    /* DISPLAY LINES */
    y0 = window.y;
    bool compact = window.h / font->h <= nlines + nlabels;
    if (!compact) { 
        y0 += font->h;
    }
    for (uint8_t i = 0; i < nlines; i += 1)
    {
        size_t len = wcslen(lines[i]);
        hagl_put_text(
            hagl_backend, 
            lines[i], 
            window.x + (window.w - font->w * len) / 2, 
            y0, 
            colors[i % 3], 
            font->fontx
        );
        // wprintf(L"Line %d: %ls\r\n", i, lines[i]);
        y0 += font->h;
    }
    if (!compact) { 
        y0 += font->h;
    }
    /* DISPLAY LABELS & VALUES */
    x0 = window.x + font->w;
#ifdef STYLED
    hagl_char_style_t style = {
        .font = font->fontx,
        .background_color = color1,
        .foreground_color = color2,
        .mode = HAGL_CHAR_MODE_OPAQUE,
        .scale_x_numerator = 1, .scale_x_denominator = 1,
        .scale_y_numerator = 1, .scale_y_denominator = 1,
    };
#endif
    for(uint8_t i = 0; i < nlabels; i += 1)
    {
#ifdef STYLED
        x1 = x0 + (wcslen(labels[i]) + 2) * font->w * style.scale_x_denominator / style.scale_y_denominator;
        y1 = y0 + i * font->h * style.scale_y_numerator / style.scale_y_denominator;
        style.mode = HAGL_CHAR_MODE_OPAQUE;
        hagl_put_text_styled(hagl_backend, labels[i], x0 + font->w * style.scale_x_numerator / style.scale_x_denominator, y1, &style);
        style.mode = HAGL_CHAR_MODE_REVERSE;
        hagl_put_text_styled(hagl_backend, values[i], x1          , y1, &style);
#else
        x1 = x0 + (wcslen(labels[i]) + 1) * font->w;
        y1 = y0 + i * font->h;
        hagl_put_text(hagl_backend, labels[i], x0, y1, color1, font->fontx);
        hagl_put_text(hagl_backend, values[i], x1, y1, color2, font->fontx);
#endif
        // wprintf(
        //     L"spec #%d: label=%ls value=%ls (%d-%d) (%d-%d)\r\n", 
        //     i, labels[i], values[i],
        //     x0, y0, x1, y1
        // );
    }
}

void specs_draw()
{
    // Nothing!
}

/* EOF */
