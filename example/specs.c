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

#include <malloc.h>
#if !PICO_NO_HARDWARE
#include <pico/unique_id.h>
#endif
#include <pico/version.h>

#include "hagl/char.h"

/**
 * @brief Get free RAM using malloc() and dichotomy
 *
 * @return size_t
 */
size_t get_free_ram_1()
{
#if !PICO_NO_HARDWARE
    size_t left = 0, right = 512 * 1024, middle;
    void *ptr;
    while (left < right)
    {
        middle = (left + right) / 2;
        ptr = malloc(middle);
        if (ptr)
        {
            left = middle + 1;
            free(ptr);
        }
        else
        {
            right = middle;
        }
    }
    return left;
#else
    return 0;
#endif
}

/**
 * @brief Get free RAM using static memory defines
 *        cf. https://forums.raspberrypi.com/viewtopic.php?t=347638#p2082565
 *
 * @return size_t
 */
size_t get_free_ram_2()
{
#if !PICO_NO_HARDWARE
    extern char __StackLimit, __bss_end__;
    size_t total_heap = &__StackLimit - &__bss_end__;
    struct mallinfo info = mallinfo();
    return total_heap - info.uordblks;
#else
    return 0;
#endif
}

/* Make them global since they seem to don't fit in stack anymore */
#define NLINES 4
wchar_t *lines[NLINES];
#define NLABELS 14
wchar_t *labels[NLABELS];
wchar_t values[NLABELS][40];
wchar_t _specs_scroller[NLABELS * (40 + 40)];
wchar_t *specs_scroller = _specs_scroller;

hagl_color_t tile1_bitmap[] = {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x3,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xe,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xe,
    0xc,
    0xe,
    0x0,
    0x0,
    0x2,
    0x0,
    0x0,
    0x0,
    0xe,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};
hagl_bitmap_t tile1 = {
    .width = 8,
    .height = 8,
    .depth = 4,                   // bits
    .pitch = 16,                  // 8 * 2, bytes per row
    .size = sizeof(tile1_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile1_bitmap};

hagl_color_t tile2_bitmap[] = {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xd,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xd,
    0xc,
    0xd,
    0x0,
    0x0,
    0x0,
    0x0,
    0xe,
    0xe,
    0xd,
    0x0,
    0x0,
    0x0,
    0x0,
    0xe,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xe,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4,
    0x0,
    0x0,
};
hagl_bitmap_t tile2 = {
    .width = 8,
    .height = 8,
    .depth = 4,                   // bits
    .pitch = 16,                  // 8 * 2, bytes per row
    .size = sizeof(tile2_bitmap), // 8 * 8  = 64 bytes
    .buffer = (uint8_t *)&tile2_bitmap};

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
    hagl_put_text_styled(hagl_backend, text, x0 + 0, y0 + 0, style);
}
#endif

wchar_t *get_vreg_voltage_text(int vreg_voltage)
{
#if !PICO_NO_HARDWARE
    switch (vreg_voltage == 0 ? VREG_VOLTAGE_DEFAULT : vreg_voltage)
    {
    case VREG_VOLTAGE_0_85:
        return L"0.85";
    case VREG_VOLTAGE_0_90:
        return L"0.90";
    case VREG_VOLTAGE_0_95:
        return L"0.95";
    case VREG_VOLTAGE_1_00:
        return L"1.00";
    case VREG_VOLTAGE_1_05:
        return L"1.05";
    case VREG_VOLTAGE_1_10:
        return L"1.10";
    case VREG_VOLTAGE_1_15:
        return L"1.15";
    case VREG_VOLTAGE_1_20:
        return L"1.20";
    case VREG_VOLTAGE_1_25:
        return L"1.25";
    case VREG_VOLTAGE_1_30:
        return L"1.30";
    }
#endif
    return L"?.??";
}

void specs_calc(bool for_scroller)
{
    uint8_t i = 0;
    /* LABELS */
    /* clang-format off */
    /*                            123456789012345678                         12345678901      1234 */
    labels[i++] = for_scroller ? L"VGA mode"           : (demo_window.w > 160 ? L"VGA MODE   " : L"MODE");
    labels[i++] = for_scroller ? L"Display mode"       : (demo_window.w > 160 ? L"DISP. MODE " : L"DISP");
    labels[i++] = for_scroller ? L"Letterbox mode"     : (demo_window.w > 160 ? L"LETTERBOX  " : L"BOX ");
    labels[i++] = for_scroller ? L"Horizontal clock"   : (demo_window.w > 160 ? L"HORIZ. CLK " : L"HORZ");
    labels[i++] = for_scroller ? L"Vertical refresh"   : (demo_window.w > 160 ? L"V. REFRESH " : L"VERT");
    labels[i++] = for_scroller ? L"BPP / colors"       : (demo_window.w > 160 ? L"BPP/COLORS " : L"BPP ");
    labels[i++] = for_scroller ? L"Framebuffer"        : (demo_window.w > 160 ? L"FRAMEBUFFER" : L"FBUF");
    labels[i++] = for_scroller ? L"System clock"       : (demo_window.w > 160 ? L"SYSTEM CLK " : L"SCLK");
    labels[i++] = for_scroller ? L"Voltage regulator"  : (demo_window.w > 160 ? L"VOLTAGE REG" : L"VREG");
    labels[i++] = for_scroller ? L"Palette"            : (demo_window.w > 160 ? L"PALETTE    " : L"PAL ");
    labels[i++] = for_scroller ? L"Pico SDK"           : (demo_window.w > 160 ? L"PICO SDK   " : L"SDK ");
    labels[i++] = for_scroller ? L"Pico serial number" : (demo_window.w > 160 ? L"SERIAL NUM " : L"S/N ");
    labels[i++] = for_scroller ? L"RP2040 ROM rev."    : (demo_window.w > 160 ? L"RP2040 ROM " : L"ROM ");
    labels[i++] = for_scroller ? L"Free memory"        : (demo_window.w > 160 ? L"FREE RAM   " : L"FREE");
    /* clang-format on */
    /* VALUES */
    wchar_t *vreg_voltage = get_vreg_voltage_text(vgaboard->vreg_voltage);
#if !PICO_NO_HARDWARE
    char unique_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(unique_id, 2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1);
    uint8_t rom = rp2040_rom_version();
    wchar_t *rev = rom == 1 ? L"B0" : rom == 2 ? L"B1"
                                  : rom == 3   ? L"B2"
                                               : L"B?";
    int sys_clock_mhz = clock_get_hz(clk_sys) / 1000 / 1000;
#else
    char *unique_id = "?";
    uint8_t rom = 0;
    wchar_t *rev = L"B?";
    int sys_clock_mhz = 0;
#endif
    i = 0;
    swprintf(values[i++], sizeof(values[0]), L"%dx%d", vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[i++], sizeof(values[0]), L"%dx%d", vgaboard->width, vgaboard->height);
    swprintf(values[i++], sizeof(values[0]), L"%dx%d", vgaboard->display_width, vgaboard->display_height);
    swprintf(values[i++], sizeof(values[0]), L"%d kHz", vgaboard->scanvideo_mode->default_timing->clock_freq / 1000);
    swprintf(values[i++], sizeof(values[0]), L"%d Hz", vgaboard->freq_hz);
    swprintf(values[i++], sizeof(values[0]), L"%d/%d", DEPTH, COLORS);
    swprintf(values[i++], sizeof(values[0]), L"%d/%d", WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[i++], sizeof(values[0]), L"%d MHz", sys_clock_mhz);
    swprintf(values[i++], sizeof(values[0]), L"%ls V", vreg_voltage);
    swprintf(values[i++], sizeof(values[0]), L"%ls", DEPTH == 16 ? L"N/A" : palette_name);
    swprintf(values[i++], sizeof(values[0]), L"v%s", PICO_SDK_VERSION_STRING);
    swprintf(values[i++], sizeof(values[0]), L"%s", unique_id);
    swprintf(values[i++], sizeof(values[0]), L"%d/%ls", rom, rev);
    swprintf(values[i++], sizeof(values[0]), L"%d/%d", get_free_ram_1(), get_free_ram_2());
    if (for_scroller)
    {
        wchar_t buffer[40];
        for (i = 0; i < NLABELS; i++)
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
    demo_window.x = DEMO.x;
    demo_window.y = DEMO.y;
    demo_window.w = DEMO.w;
    demo_window.h = DEMO.h;
    const font_t *font = &FONT8X8;
    hagl_color_t color1, color2, color3, color4;
    if (DEPTH == 1)
    {
        color1 = 1;
        color2 = 1;
        color3 = 1;
        color4 = 1;
    }
    else if (DEPTH == 2)
    {
        color1 = 1;
        color2 = 2;
        color3 = 3;
        color4 = 1;
    }
    else
    {
        color1 = 1 + rand() % (COLORS - 1);
        do
        {
            color2 = 1 + rand() % (COLORS - 1);
        } while (color2 == color1);
        do
        {
            color3 = 1 + rand() % (COLORS - 1);
        } while (color3 == color1 || color3 == color2);
        do
        {
            color4 = 1 + rand() % (COLORS - 1);
        } while (color4 == color1 || color4 == color2 || color4 == color3);
    }
    hagl_color_t colors[4] = {color1, color2, color3, color4};
    uint16_t x0, y0, x1, y1;
    // /* TILED BACKGROUND IN 4BPP MODE */
    // if (DEPTH==4) {
    //     int zoom = 1;
    //     for (int row = 0; row < window.h / tile1.height / zoom; row++)
    //     {
    //         for (int col = 0; col < window.w / tile1.width / zoom; col++)
    //         {
    //             hagl_blit_xywh(
    //                 hagl_backend,
    //                 window.x + col * tile1.width * zoom,
    //                 window.y + row * tile1.height * zoom,
    //                 tile1.width * zoom,
    //                 tile1.height * zoom,
    //                 (row + col) % 2 == 0 ? &tile1 : &tile2
    //             );
    //         }
    //     }
    // }
    /* TITLE LINES */
    /*                            1234567890123456789      1234567890 */
    lines[0] = demo_window.w > 160 ? L"Raspberry Pi Pico" : L"RPi Pico";
    lines[1] = demo_window.w > 160 ? L"VGA Demo Board" : L"VGA Board";
    lines[2] = demo_window.w > 160 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL";
    lines[3] = demo_window.w > 160 ? L"github.com/CHiPs44" : L"by CHiPs44";
    y0 = demo_window.y;
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
    hagl_char_style_t style1 = {
        .font = font->fontx,
        .background_color = 0,
        .foreground_color = 0,
        .mode = HAGL_CHAR_MODE_TRANSPARENT,
        .scale_x_numerator = 1,
        .scale_x_denominator = 1,
        .scale_y_numerator = 1,
        .scale_y_denominator = 1,
    };
    bool compact = demo_window.h / (font->h * style1.scale_y_numerator / style1.scale_y_denominator) <= NLINES + NLABELS;
#else
    bool compact = window.h / font->h <= NLINES + NLABELS;
#endif
    if (!compact)
    {
        y0 += font->h;
    }
    for (uint8_t i = 0; i < NLINES; i += 1)
    {
        size_t len = wcslen(lines[i]);
#ifdef HAGL_HAS_STYLED_TEXT_AND_TRANSPARENCY
        x0 = demo_window.x + (demo_window.w - font->w * len * style1.scale_x_numerator / style1.scale_x_denominator) / 2;
        style1.foreground_color = colors[i % 4];
        specs_text(x0, y0, lines[i], &style1);
        y0 += font->h * style1.scale_y_numerator / style1.scale_y_denominator;
#else
        x0 = window.x + (window.w - font->w * len) / 2;
        hagl_put_text(hagl_backend, lines[i], x0, y0, colors[i % 4], font->fontx);
        y0 += font->h;
#endif
    }
    if (!compact)
    {
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
        .scale_x_numerator = 1,
        .scale_x_denominator = 1,
        .scale_y_numerator = 1,
        .scale_y_denominator = 1,
    };
    x0 = demo_window.x + font->w * style2.scale_x_numerator / style2.scale_x_denominator;
#else
    x0 = window.x + font->w;
#endif
    for (uint8_t i = 0; i < NLABELS; i += 1)
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
