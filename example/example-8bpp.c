// TODO HEADER

void example_8bpp()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t depth = vgaboard->depth;
    const uint16_t colors = vgaboard->colors;
    const uint16_t freq_hz = vgaboard->freq_hz;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;

    uint16_t x, y, w, h;
    wchar_t demo[40];
    wchar_t text[80];

    printf("*** EXAMPLE_%dX%dX%d ***\n", width, height, depth);

    hagl_set_clip(hagl_backend, 0, 0, width - 1, height - 1);

    /* Borders */
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, width, height, 0xff);
    // Missing pixel at bottom right corner (HAGL bug?)
    hagl_put_pixel(hagl_backend, vgaboard->width - 1, vgaboard->height - 1, 0xff);
    // X axis
    hagl_draw_hline(hagl_backend, 0, half_height - 1, vgaboard->width - 1, 0xff);
    // Y axis
    hagl_draw_vline(hagl_backend, half_width - 1, 0, vgaboard->height - 1, 0xff);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        L" VGA %dx%dx%d/%d@%d %dMHz ",
        width, height, depth, colors, freq_hz,
        clock_get_hz(clk_sys) / 1000000);
    // wprintf(L"demo: %ls (%d)\n", &demo, wcslen(demo));
    w = wcslen(demo) * 8 - 1;
    h = 13;
    x = half_width - w / 2;
    y = 4;
    hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 4, y - 4, w + 8, h + 8, 2, 0xff);
    hagl_put_text(hagl_backend, demo, x, y, 0xff, font8x13B);

    w = 7;
    h = 7;
    uint16_t left = (width / 2 - 16 * (w + 1)) /2; 
    uint16_t top = (height - 16 * (h + 1)) /2;
    for (uint16_t c = 0; c < 256; c += 1)
    {
        x = left + (c % 16) * (w + 1);
        y = top + (c / 16) * (h + 1);
        hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
        // hagl_draw_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 1, h + 1, c == 0xff ? 0x7f : 0xff);
        swprintf(text, sizeof(text), L"%02d\u2192%04X", c, vgaboard_get_palette_color(c));
        // hagl_put_text(hagl_backend, text, x0 + w + 5, y0 + 1, 15, font5x7);
        wprintf(L"%s\n", text);
    }

    uint32_t counter = 0;
    int led = 0;
    // wchar_t text[80];
    clock_t startTime = clock();
    clock_t endTime, elapsedTime;
    int hours, minutes, seconds, milliseconds;
    while (true)
    {
        /*
         * Palette swap test
         * TODO /!\ gets darker and darker with time /!\
         */
        // uint8_t i = rand() % 256;
        // uint8_t j = rand() % 256;
        // uint16_t tmp = vgaboard->palette[i];
        // vgaboard->palette[i] = vgaboard->palette[j];
        // vgaboard->palette[j] = tmp;

        // /*
        //  * Random lines
        //  */
        // x = (width / 2) + rand() % (width / 2);
        // y = 32 + rand() % (height - 32);
        // w = rand() % (width/2);
        // h = rand() % (height/2);
        // hagl_draw_line(hagl_backend, x, y, x + w - 1, y + h - 1, counter % colors);
        // // hagl_draw_hline_xyw(hagl_backend, x, y, w, counter % colors);
        // // hagl_draw_vline_xyh(hagl_backend, x, y, h, colors - counter % (colors-1));

        // Draw counter & elapsed time HH:MM:SS.mmm
        endTime = clock();
        elapsedTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
        hours = elapsedTime / 1000 / 60 / 60;
        minutes = (elapsedTime / 1000 / 60) % 60;
        seconds = (elapsedTime / 1000) % 60;
        milliseconds = elapsedTime % 1000;
        swprintf(text, sizeof(text), L"%d - %02d:%02d:%02d.%03d", counter, hours, minutes, seconds, milliseconds);
        hagl_put_text(hagl_backend, text, width - wcslen(text) * 8, height - 13, 0xff, font8x13B);

        // Next cycle
        counter += 1;
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        sleep_ms(1);
        led = 1 - led;
    }
}

/* EOF */
