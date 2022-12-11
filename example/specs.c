/* SPDX-License-Identifier: MIT-0 */

void specs_init()
{
    // hagl_fill_rectangle_xywh(hagl_backend, window.x, window.y, window.w, window.h, COLORS - 1);
    color_t color1 = 1 + rand() % (COLORS - 1);
    color_t color2 = 1 + rand() % (COLORS - 1);
    color_t color3 = 1 + rand() % (COLORS - 1);
    font_t *font = &FONT8X8;// get_small_font(window);
    uint16_t x0, y0, x1, y1;
    x0 = window.x;
    y0 = window.y;
    /* TITLE LINES */
    wchar_t *lines[4] = {
        //                 12345678901234567890     1234567890123456
        window.w > 80 ? L"Raspberry Pi Pico"   : L"RPi Pico"      ,
        window.w > 80 ? L"VGA Demo Board"      : L"VGA Demo Board",
        window.w > 80 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL"      ,
        window.w > 80 ? L"github.com/CHiPs44"  : L"by CHiPs44"    ,
    };
    color_t colors[3] = { color1, color2, color3 };
    for (uint8_t i = 0; i < 4; i += 1)
    {
        size_t len = wcslen(lines[i]);
        hagl_put_text(hagl_backend, lines[i], x0 + (window.w / 2 - font->w * len) / 2, y0, colors[i % 3], font->fontx);
        y0 += font->h;
    }
    y0 += font->h;
    /* LABELS & VALUES */
    wchar_t *labels[] = {
        //                 1234567890      1234
        window.w > 80 ? L"BASE MODE " : L"BASE",
        window.w > 80 ? L"H CLOCK   " : L"HCLK",
        window.w > 80 ? L"V REFRESH " : L"VREF",
        window.w > 80 ? L"VIEW MODE " : L"VIEW",
        window.w > 80 ? L"BPP/COLORS" : L"B/C ",
        window.w > 80 ? L"VRAM      " : L"VRAM",
        window.w > 80 ? L"SYS CLOCK " : L"SCLK",
    };
    wchar_t values[sizeof(labels)][20];
    //                                       1234567890
    swprintf(values[0], sizeof(values[0]), L"%dx%d" , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[1], sizeof(values[1]), L"%d MHz", vgaboard->scanvideo_mode->default_timing->clock_freq / 1000 / 1000);
    swprintf(values[2], sizeof(values[2]), L"%d Hz" , vgaboard->freq_hz);
    swprintf(values[3], sizeof(values[3]), L"%dx%d" , WIDTH, HEIGHT);
    swprintf(values[4], sizeof(values[4]), L"%d/%d" , DEPTH, COLORS);
    swprintf(values[5], sizeof(values[5]), L"%d/%d" , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[6], sizeof(values[6]), L"%d MHz", clock_get_hz(clk_sys) / 1000 / 1000);
    for(uint8_t i = 0; i < 7; i += 1)
    {
        x1 = x0 + (wcslen(labels[i]) + 2) * font->w;
        y1 = y0 + i * font->h;
        hagl_put_text(hagl_backend, labels[i], x0 + font->w, y1, color1, font->fontx);
        hagl_put_text(hagl_backend, values[i], x1          , y1, color2, font->fontx);
        // wprintf(
        //     L"spec #%d: label=%ls value=%ls (%d-%d) (%d-%d)\r\n", 
        //     i, labels[i], values[i],
        //     x0, y0, x1, y1
        // );
    }
}

/**
 * @brief Draw specs of current VGA mode
 * 
 * @param color1 labels colors
 * @param color2 values colors
 * @param color3 lines colors
 */
void specs_draw()
{
    // Nothing!
}

/* EOF */
