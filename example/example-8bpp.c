// TODO HEADER

void example_8bpp()
{
    const uint16_t width = hagl_backend->width;
    const uint16_t height = hagl_backend->height;
    const uint16_t depth = vgaboard->depth;
    const uint16_t colors = vgaboard->colors;
    const uint16_t half_width = width / 2;
    const uint16_t half_height = height / 2;

    uint16_t x, y, w, h;
    wchar_t demo[40];

    printf("*** EXAMPLE_%dX%dX%d ***\n", width, height, depth);

    hagl_set_clip(hagl_backend, 0, 0, width - 1, height - 1);

    /* Borders */
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, width, height, 0x7f);
    // Missing pixel at bottom right corner (HAGL bug?)
    hagl_put_pixel(hagl_backend, vgaboard->width - 1, vgaboard->height - 1, 0x7f);
    // X axis
    // hagl_draw_hline(hagl_backend, 0, half_height - 1, vgaboard->width - 1, 0x7f);
    // Y axis
    // hagl_draw_vline(hagl_backend, half_width - 1, 0, vgaboard->height - 1, 0x7f);

    /* Title */
    swprintf(
        demo, sizeof(demo),
        L"VGA: %dx%dx%d/%d@%dMHz",
        width, height, depth, colors,
        clock_get_hz(clk_sys) / 1000000);
    wprintf(L"demo: %ls (%d)\n", &demo, wcslen(demo));
    w = wcslen(demo) * 8 - 1;
    h = 13;
    x = half_width - w / 2;
    y = 8;
    // hagl_fill_rounded_rectangle_xywh(hagl_backend, x - 4, y - 4, w + 4, h + 4, 3, 0x42);
    hagl_put_text(hagl_backend, demo, x, y, 0xff, font8x13);

    w = 15;
    h = 9;
    for (uint16_t c = 0; c < 256; c += 1)
    {
        x = 8 + (c % 16) * (w + 1);
        y = 8 * 3 + (c / 16) * (h + 1);
        hagl_fill_rectangle_xywh(hagl_backend, x, y, w, h, c);
        // hagl_draw_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 1, h + 1, c == 0xff ? 0x7f : 0xff);
    }

    uint32_t counter = 0;
    int led = 0;
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

        /*
         * Random lines
         */
        // x = rand() % width;
        // y = rand() % height;
        // w = rand() % width;
        // h = rand() % height;
        // hagl_draw_line(hagl_backend, x, y, x + w - 1, y + h - 1, counter % colors);
        // // hagl_draw_hline_xyw(hagl_backend, x, y, w, counter % colors);
        // // hagl_draw_vline_xyh(hagl_backend, x, y, h, colors - counter % (colors-1));

        /*
         * Blink LED to show activity and wait a bit
         */
        counter += 1;
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        sleep_ms(100);
        led = 1 - led;
    }
}

/* EOF */
