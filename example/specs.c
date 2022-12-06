/**
 * @brief Draw specs of current VGA mode
 * 
 * @param color1 labels colors
 * @param color2 values colors
 * @param color3 lines colors
 */
void specs(rect_t *window, color_t color1, color_t color2, color_t color3)
{
    font_t *font = get_small_font(window);
    uint16_t x0, x1, y0, y1;
    wchar_t *labels[] = {
        //              1234567890      1234
        WIDTH > 160 ? L"BASE MODE " : L"BASE",
        WIDTH > 160 ? L"H CLOCK   " : L"HCLK",
        WIDTH > 160 ? L"V REFRESH " : L"VREF",
        WIDTH > 160 ? L"VIEW MODE " : L"VIEW",
        WIDTH > 160 ? L"BPP/COLORS" : L"B/C ",
        WIDTH > 160 ? L"VRAM      " : L"VRAM",
        WIDTH > 160 ? L"SYS CLOCK " : L"SCLK",
    };
    wchar_t values[sizeof(labels)][20];
    //                                       1234567890
    swprintf(values[0], sizeof(values[0]), L"%dx%d" , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[1], sizeof(values[1]), L"%d MHz", vgaboard->scanvideo_mode->default_timing->clock_freq / 1000 / 1000);
    swprintf(values[2], sizeof(values[2]), L"%d Hz" , FREQ_HZ);
    swprintf(values[3], sizeof(values[3]), L"%dx%d" , WIDTH, HEIGHT);
    swprintf(values[4], sizeof(values[4]), L"%d/%d" , DEPTH, COLORS);
    swprintf(values[5], sizeof(values[5]), L"%d/%d" , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    swprintf(values[6], sizeof(values[6]), L"%d MHz", clock_get_hz(clk_sys) / 1000 / 1000);
    x0 = WIDTH / 2;
    y0 = 0;
    wchar_t *lines[4] = {
        //              12345678901234567890     1234567890123456
        WIDTH > 160 ? L"Raspberry Pi Pico"   : L"RPi Pico"      ,
        WIDTH > 160 ? L"VGA Demo Board"      : L"VGA Demo Board",
        WIDTH > 160 ? L"HAGL HAL by CHiPs44" : L"HAGL HAL"      ,
        WIDTH > 160 ? L"github.com/CHiPs44"  : L"by CHiPs44"    ,
    };
    color_t colors[3] = { color1, color2, color3 };
    // hagl_draw_rectangle_xywh(hagl_backend, x0 + 2, y0 + 2, window->w - 4, window->h - 4, color3);
    y0 += font->h;
    for (uint8_t i = 0; i < 4; i += 1)
    {
        size_t len = wcslen(lines[i]);
        hagl_put_text(hagl_backend, lines[i], x0 + (WIDTH / 2 - font->w * len) / 2, y0, colors[i % 3], font->fontx);
        y0 += font->h;
    }
    y0 += font->h;
    for(uint8_t i = 0; i <= 6; i += 1)
    {
        x1 = x0 + (wcslen(labels[i]) + 2) * font->w;
        y1 = y0 + i * font->h;
        hagl_put_text(hagl_backend, labels[i], x0 + font->w, y1, color1, font->fontx);
        hagl_put_text(hagl_backend, values[i], x1, y1, color2, font->fontx);
    }
}
