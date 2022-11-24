/*

MIT No Attribution

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

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

uint32_t counter = 0;
int led = 0;
clock_t startTime, endTime, elapsedTime;
int hours, minutes, seconds, milliseconds, fps;

#define WIDTH   (hagl_backend->width)
#define HEIGHT  (hagl_backend->height)
#define DEPTH   (hagl_backend->depth)
#define COLORS  (vgaboard->colors)
#define FREQ_HZ (vgaboard->freq_hz)

clock_t get_time()
{
    /* cf. https://lindevs.com/measure-execution-time-of-code-using-raspberry-pi-pico */
    return (clock_t) time_us_64() / 10000;
}

void start_time()
{
    counter = 0;
    led = 0;
    startTime = get_time();
}

void cycle_time(color_t color)
{
    wchar_t text[80];

    // Draw counter & elapsed time HH:MM:SS.mmm
    endTime = get_time();
    elapsedTime = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
    fps = 1000 * counter / elapsedTime;
    hours = elapsedTime / 1000 / 60 / 60;
    minutes = (elapsedTime / 1000 / 60) % 60;
    seconds = (elapsedTime / 1000) % 60;
    milliseconds = elapsedTime % 1000;
    swprintf(
        text, sizeof(text), 
        //123456789012345678901234567890
        //0000000 00:00:00.000 000
        L"%07d %02d:%02d:%02d.%03d %03d", 
        counter % 10000000, hours, minutes, seconds, milliseconds, fps
    );
    /*WIDTH / 2 - wcslen(text) * 8 / 2*/
    hagl_put_text(hagl_backend, text, 0, HEIGHT - 8, color, BIOS_F08_fnt);
    // Next cycle
    counter += 1;
    gpio_put(PICO_DEFAULT_LED_PIN, led);
    // sleep_ms(1);
    led = 1 - led;
}

void draw_borders_and_axis(color_t color1, color_t color2, color_t color3)
{
    // Borders
    hagl_draw_rectangle_xywh(hagl_backend, 0, 0, WIDTH, HEIGHT, color1);
    // Missing pixel at bottom right corner (HAGL bug?)
    hagl_put_pixel(hagl_backend, WIDTH - 1, HEIGHT - 1, color1);
    // X axis
    hagl_draw_hline(hagl_backend, 1, HEIGHT / 2 - 1, WIDTH - 2, color2);
    // Y axis
    hagl_draw_vline(hagl_backend, WIDTH / 2 - 1, 1, HEIGHT - 2, color3);
}

/**
 * @brief Draw title with frame & shadow
 * 
 * @param color1 text color
 * @param color2 frame color
 * @param color3 frame shadow color
 */
void draw_title(color_t color1, color_t color2, color_t color3)
{
    wchar_t title[80];
    uint16_t x, y, w, h;
    swprintf(
        title, sizeof(title),
        //1234567890123456789012345678901234567890
        L"Raspberry Pi Pico VGA HAL for HAGL");
    w = wcslen(title) * 8;
    h = 8;
    x = WIDTH / 2 - w / 2;
    y = 2;
    // hagl_draw_rounded_rectangle_xywh(hagl_backend, x - 1, y - 1, w + 4, h + 4, 2, color3);
    // hagl_draw_rounded_rectangle_xywh(hagl_backend, x - 2, y - 2, w + 4, h + 4, 2, color2);
    hagl_put_text(hagl_backend, title, x, y, color1, BIOS_F08_fnt);
}

/**
 * @brief Draw "specs" of current VGA mode: 
 *        width, height, bpp, colors, frequency
 * 
 * @param color1 labels colors
 * @param color2 values colors
 * @param color3 lines colors
 */
void draw_specs(color_t color1, color_t color2, color_t color3)
{
    uint16_t x0, x1, y0, y1;
    const unsigned char *font = WIDTH <= 320 ? font5x7 : BIOS_F08_fnt;
    uint16_t font_w = WIDTH <= 320 ? 5 : 8;
    uint16_t font_h = WIDTH <= 320 ? 7 : 8;
    wchar_t *labels[] = {
        //1234567890
        L"Base   ",
        L"Clock  ",
        L"Refresh",
        L"Real   ",
        L"Depth  ",
        L"Colors ",
        L"Buffer ",
    };
    wchar_t values[sizeof(labels)][20];
    swprintf(values[0], sizeof(values[0]), L"%dx%d" , vgaboard->scanvideo_mode->width, vgaboard->scanvideo_mode->height);
    swprintf(values[1], sizeof(values[1]), L"%d kHz", vgaboard->scanvideo_mode->default_timing->clock_freq / 1000);
    swprintf(values[2], sizeof(values[2]), L"%d Hz" , FREQ_HZ);
    swprintf(values[3], sizeof(values[3]), L"%dx%d" , WIDTH, HEIGHT);
    swprintf(values[4], sizeof(values[4]), L"%d bpp", DEPTH);
    swprintf(values[5], sizeof(values[5]), L"%d"    , COLORS);
    swprintf(values[6], sizeof(values[6]), L"%d/%d" , WIDTH * HEIGHT * DEPTH / 8, PICO_VGABOARD_FRAMEBUFFER_SIZE);
    x0 = WIDTH / 2 + font_w;
    y0 = font_h;
    //                            12354687901235468790
    hagl_put_text(hagl_backend, L"Raspberry Pi Pico", x0 + (WIDTH / 2 - font_w * 18) / 2, y0, color1, font);
    y0 += font_h;
    hagl_put_text(hagl_backend, L"VGA Demo Board"   , x0 + (WIDTH / 2 - font_w * 15) / 2, y0, color2, font);
    y0 += font_h;
    hagl_put_text(hagl_backend, L"HAGL HAL"         , x0 + (WIDTH / 2 - font_w *  8) / 2, y0, color3, font);
    y0 += font_h;
    y0 += font_h;
    for(uint8_t i = 0; i <= 6; i += 1)
    {
        x1 = x0 + (wcslen(labels[i]) + 1) * font_w;
        y1 = y0 + i * font_h;
        hagl_put_text(hagl_backend, labels[i], x0, y1, color1, font);
        hagl_put_text(hagl_backend, values[i], x1, y1, color2, font);
    }
}

void draw_palette(color_t color1, color_t color2, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    const unsigned char *font = WIDTH <= 320 ? font5x7 : BIOS_F08_fnt;
    uint16_t font_w = WIDTH <= 320 ? 5 : 8;
    uint16_t font_h = WIDTH <= 320 ? 7 : 8;
    wchar_t text[80];

    switch (DEPTH)
    {
    case 1:
    case 2:
    case 4:
        /* Framed tile + index + value for each color in the palette */
        for (uint16_t c = 0; c < COLORS; c++)
        {
            uint16_t x0 = x + (c / 8) * (WIDTH / 2 / 2);
            uint16_t y0 = y + (c % 8) * (h + 2);
            hagl_fill_rectangle_xywh(hagl_backend, x0, y0, w, h, c);
            hagl_draw_rectangle_xywh(hagl_backend, x0, y0, w, h, c==color1 ? color2 : color1);
            // \u2192
            swprintf(text, sizeof(text), L"%02d %04X", c, vgaboard_get_palette_color(c));
            hagl_put_text(hagl_backend, text, x0 + w + font_w, y0 + (h - font_h) / 2, color2, font);
        }
        break;
    case 8:
        for (uint16_t c = 0; c < COLORS; c++)
        {
            uint16_t x0 = x + (c / 16) * (w + 1);
            uint16_t y0 = y + (c % 16) * (h + 1);
            hagl_fill_rectangle_xywh(hagl_backend, x0, y0, w, h, c);
        }
        break;
    default:
        break;
    }
}

void draw_figures()
{
    uint16_t x0 = WIDTH / 2;
    uint16_t y0 = HEIGHT / 2;
    hagl_set_clip(hagl_backend, x0, y0, x0 + WIDTH / 2 - 1, y0 + HEIGHT / 2 - 1);
    uint16_t x1 = rand() % (WIDTH / 2);
    uint16_t y1 = rand() % (HEIGHT / 2);
    uint16_t w  = rand() % (WIDTH / 2);
    uint16_t h  = rand() % (HEIGHT / 2);
    uint8_t  c  = 1 + rand() % 15;
    switch (rand() % 5)
    {
        case 0:
            hagl_draw_line(hagl_backend, x0 + x1, y0 + y1, x0 + w - 1, y0 + h - 1, c);            
            break;    
        case 1:
            hagl_draw_hline_xyw(hagl_backend, x0 + x1, y0 + y1, w, c);
            break;    
        case 2:
            hagl_draw_vline_xyh(hagl_backend, x0 + x1, y0 + y1, h, c);
            break;
        case 3:
            hagl_draw_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);
            break;
        case 4:
            hagl_fill_rectangle_xywh(hagl_backend, x0 + x1, y0 + y1, w, h, c);
            break;
    }
    hagl_set_clip(hagl_backend, 0, 0, WIDTH - 1, HEIGHT - 1);
}

/* EOF */
