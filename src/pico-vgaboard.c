/*

MIT License

Copyright (c) 2021-2022 Christophe "CHiPs44" Petit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico VGA board HAL for the HAGL graphics library:
 - https://github.com/CHiPs44/hagl_pico_vgaboard
 - https://github.com/tuupola/hagl

SPDX-License-Identifier: MIT

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "pico-vgaboard.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

#define RAM __not_in_flash("pico_vgaboard_data")

uint8_t RAM _vgaboard_framebuffer[PICO_VGABOARD_FRAMEBUFFER_SIZE];
uint8_t RAM *vgaboard_framebuffer = (u_int8_t *)(&_vgaboard_framebuffer);
vgaboard_t RAM _vgaboard;
vgaboard_t RAM *vgaboard = &_vgaboard;

/* 1, 2, 4 & 8bpp palette */
uint16_t RAM _vgaboard_palette[256];

/* Specific to 1 bit depth / 2 colors mode */
uint32_t RAM vgaboard_double_palette_1bpp[2 * 2];

/* Specific to 2 bit depth / 4 colors mode */
uint32_t RAM vgaboard_double_palette_2bpp[4 * 4];

/* Specific to 4 bits depth / 16 colors mode */
uint32_t RAM vgaboard_double_palette_4bpp[16 * 16];

/* Specific to 8 bits depth / 256 colors mode */
uint16_t RAM vgaboard_palette_8bpp_default[256];

void vgaboard_init_led()
{
#if USE_LED == 1
    // We use the onboard LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
#endif
}

void vgaboard_flash_led_and_wait()
{
#if USE_LED == 1
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(250);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(250);
#else
    sleep_ms(500);
#endif
}

void vgaboard_toggle_led()
{
#if USE_LED == 1
    gpio_put(PICO_DEFAULT_LED_PIN, gpio_get(PICO_DEFAULT_LED_PIN) ? 0 : 1);
#endif
}

void vgaboard_init_default_palette_8bpp()
{
    /*                          0          96         160         224        */
    /*                          0           3           5           7        */
    /*                        ---         ---         ---         ---        */
    const uint8_t msb[4] = {0b00000000, 0b01100000, 0b10100000, 0b11100000};
    /*                               0           8          16          24   */
    /*                           -----       -----       -----       -----   */
    const uint8_t lsb[4] = {0b00000000, 0b00001000, 0b00010000, 0b00011000};
    uint8_t c = 0;
    uint8_t r, g, b;
    uint16_t rgb;
    for (uint8_t _r = 0; _r < 4; _r++)
    {
        for (uint8_t _g = 0; _g < 4; _g++)
        {
            for (uint8_t _b = 0; _b < 4; _b++)
            {
                for (uint8_t _i = 0; _i < 4; _i++)
                {
                    r = msb[_r] | lsb[_i];
                    g = msb[_g] | lsb[_i];
                    b = msb[_b] | lsb[_i];
                    rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
                    vgaboard_palette_8bpp_default[c++] = rgb;
                }
            }
        }
    }
}

void vgaboard_setup_double_palette_1bpp()
{
    if (vgaboard->depth != 1 || vgaboard->palette == NULL)
    {
        return;
    }
    /* vgaboard_double_palette_1bpp is 4 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_1 = vgaboard_double_palette_1bpp;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            *double_palette_1 = (vgaboard->palette[j] << 16) | vgaboard->palette[i];
            ++double_palette_1;
        }
    }
}

void vgaboard_setup_double_palette_2bpp()
{
    if (vgaboard->depth != 2 || vgaboard->palette == NULL)
    {
        return;
    }
    /* vgaboard_double_palette_2bpp is 16 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_2 = vgaboard_double_palette_2bpp;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            *double_palette_2 = (vgaboard->palette[j] << 16) | vgaboard->palette[i];
            ++double_palette_2;
        }
    }
}

void vgaboard_setup_double_palette_4bpp()
{
    if (vgaboard->depth != 4 || vgaboard->palette == NULL)
    {
        return;
    }
    /* vgaboard_double_palette_4bpp is 256 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_4 = vgaboard_double_palette_4bpp;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            *double_palette_4 = (vgaboard->palette[i] << 16) | vgaboard->palette[j];
            ++double_palette_4;
        }
    }
}

void vgaboard_set_palette(const uint16_t *palette)
{
    if (vgaboard->depth > 8) {
        return;
    }
#if PICO_VGABOARD_DEBUG
    printf("VGABOARD: PALETTE %p\n", palette);
#endif
    // Copy palette to RAM
    for (uint16_t i = 0; i < vgaboard->colors; i += 1)
    {
        vgaboard->palette[i] = palette[i];
#if PICO_VGABOARD_DEBUG
        printf("VGABOARD: PALETTE[%d] = %d\n", i, palette[i]);
#endif
    }
    // Setup double palettes
    vgaboard_setup_double_palette_1bpp();
    vgaboard_setup_double_palette_2bpp();
    vgaboard_setup_double_palette_4bpp();
}

void scanvideo_dump(const scanvideo_mode_t *scanvideo_mode)
{
#if PICO_VGABOARD_DEBUG
    printf("*** SCANVIDEO_MODE ***\n");
    printf("\tW: %d\tH: %d\tX: %d\tY: %d\tD: %d\n",
           scanvideo_mode->width, scanvideo_mode->height,
           scanvideo_mode->xscale, scanvideo_mode->yscale,
           scanvideo_mode->yscale_denominator);
#endif
}

void vgaboard_dump(const vgaboard_t *vgaboard)
{
#if PICO_VGABOARD_DEBUG
    printf("*** VGABOARD %p ***\n", vgaboard);
    printf("\tWidth: %d\tHeight: %d\n\tDepth: %d\tColors: %d\n\tFramebuffer:%p\tFramebuffer Size: %d\n\tPalette: %p\n",
           vgaboard->width, vgaboard->height,
           vgaboard->depth, vgaboard->colors,
           vgaboard->framebuffer, vgaboard->framebuffer_size,
           vgaboard->palette);
#endif
}

void vgaboard_init()
{
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_init INIT\n");
#endif
    // One time initializations
    vgaboard_init_led();
    // (only necessary when using 8bpp modes)
    vgaboard_init_default_palette_8bpp();
#if USE_INTERP == 1
    // Configure interpolater lanes for 4bbp
    interp_config c = interp_default_config();
    interp_config_set_shift(&c, 22);
    interp_config_set_mask(&c, 2, 9);
    interp_set_config(interp0, 0, &c);
    interp_config_set_shift(&c, 14);
    interp_config_set_cross_input(&c, true);
    interp_set_config(interp0, 1, &c);
    interp_set_base(interp0, 0, (uintptr_t)vgaboard_double_palette_4bpp);
    interp_set_base(interp0, 1, (uintptr_t)vgaboard_double_palette_4bpp);
#endif
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_init DONE\n");
#endif
}

bool vgaboard_set_system_clock(uint32_t sys_clock_khz)
{
    if (sys_clock_khz == 0)
    {
        return true;
    }
    // Set system clock
#if PICO_VGABOARD_DEBUG
    printf("SYSTEM CLOCK: SETUP INIT\n");
#endif
    uint32_t old_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    bool ok = set_sys_clock_khz(sys_clock_khz, false);
    uint32_t new_sys_clock_khz = clock_get_hz(clk_sys) / 1000;
    vgaboard_flash_led_and_wait();
    stdio_init_all();
    vgaboard_flash_led_and_wait();
    vgaboard_flash_led_and_wait();
#if PICO_VGABOARD_DEBUG
    printf("*** System clock speed %d kHz (before: %d, asked %d kHz: %s) ***\n",
        new_sys_clock_khz,
        old_sys_clock_khz,
        sys_clock_khz,
        ok ? "OK" : "KO");
#endif
#if PICO_VGABOARD_DEBUG
    printf("SYSTEM CLOCK: SETUP DONE\n");
#endif
    return ok;
}

void vgaboard_setup(const vgaboard_t *model)
{
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_setup INIT\n");
#endif
    vgaboard->scanvideo_mode    = model->scanvideo_mode;
    vgaboard->freq_hz           = model->freq_hz;
    vgaboard->width             = model->scanvideo_mode->width / model->scanvideo_mode->xscale;
    vgaboard->height            = model->scanvideo_mode->height / model->scanvideo_mode->yscale;
    // NB: yscale_denominator ignored
    vgaboard->depth             = model->depth;
    vgaboard->colors            = 1 << model->depth;
    vgaboard->palette           = _vgaboard_palette;
    vgaboard_set_palette(model->palette);
    vgaboard->framebuffer       = vgaboard_framebuffer;
    vgaboard->framebuffer_size  = PICO_VGABOARD_FRAMEBUFFER_SIZE;
    vgaboard->sys_clock_khz     = model->sys_clock_khz;
    vgaboard_set_system_clock(vgaboard->sys_clock_khz);
    scanvideo_setup(vgaboard->scanvideo_mode);
    // scanvideo_timing_enable(true);
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_setup DONE\n");
#endif
}

void vgaboard_enable()
{
#if PICO_VGABOARD_DEBUG
    printf("VGABOARD: ENABLE\n");
#endif
    scanvideo_timing_enable(true);
    // sleep_ms(100);
}

void vgaboard_disable()
{
#if PICO_VGABOARD_DEBUG
    printf("VGABOARD: DISABLE\n");
#endif
    scanvideo_timing_enable(false);
    // sleep_ms(100);
}

#if PICO_VGABOARD_DEBUG
uint32_t vgaboard_counter = 0;
#endif

void __not_in_flash("pico_vgaboard_code")(vgaboard_render_loop)(void)
{
    int counter = 0;
#if PICO_VGABOARD_DEBUG
    printf("VGABOARD: Starting render %dx%dx%d/%d@%dHz (%dMHz)\n",
           vgaboard->width, vgaboard->height,
           vgaboard->depth, vgaboard->colors,
           vgaboard->freq_hz, clock_get_hz(clk_sys) / 1000000);
#endif
    // Let's go for the show!
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        uint16_t scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *scanline_colors = buffer->data;
        uint8_t *framebuffer_line_start;
        uint8_t bits, bits76, bits54, bits32, bits10, bits7654, bits3210;
        switch (vgaboard->depth)
        {
        case 1: // 1bpp, 8 pixels per byte
            framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width / 8) * scanline_number]);
            for (uint16_t byte = 0; byte < vgaboard->width / 8; ++byte)
            {
                // 76543210 => 8 pixels to 8 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                bits76 = (bits & 0b11000000) >> 6;
                bits54 = (bits & 0b00110000) >> 4;
                bits32 = (bits & 0b00001100) >> 2;
                bits10 = (bits & 0b00000011) >> 0;
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_1bpp[bits76];
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_1bpp[bits54];
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_1bpp[bits32];
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_1bpp[bits10];
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 2: // 2bpp, 4 pixels per byte
            framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width / 4) * scanline_number]);
            for (uint16_t x = 0; x < vgaboard->width / 4; ++x)
            {
                // 76543210 => 4 pixels to 4 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                bits7654 = (bits & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) >> 4;
                // ++scanline_colors;
                *++scanline_colors = vgaboard_double_palette_2bpp[bits7654];
                bits3210 = (bits & (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0)) >> 0;
                // ++scanline_colors;
                *++scanline_colors = vgaboard_double_palette_2bpp[bits3210];
                // Next byte / 4 pixels
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 4: // 4bpp, 2 pixels per byte
            /* OK */
            framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width / 2) * scanline_number]);
#if USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, framebuffer_line_start, vgaboard->width / 2);
            scanline_colors += vgaboard->width / 2;
#else
            for (uint16_t x = 0; x < vgaboard->width / 2; ++x)
            {
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_4bpp[*framebuffer_line_start];
                ++framebuffer_line_start;
            }
            ++scanline_colors;
#endif
            break;
        case 8: // 8bpp, 1 pixel per byte
            framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width / 1) * scanline_number]);
            // append 2 16 bits pixels in the scanline, hence width / 2
            for (uint16_t x = 0; x < vgaboard->width / 2; ++x)
            {
                ++scanline_colors;
                uint8_t pixel1 = *framebuffer_line_start++;
                uint8_t pixel2 = *framebuffer_line_start++;
                uint32_t color1 = vgaboard->palette[pixel1];
                uint32_t color2 = vgaboard->palette[pixel2];
                *scanline_colors = (color2 << 16) | color1;
            }
            ++scanline_colors;
            break;
        case 16: // 16bpp, 1 pixel per word / 2 bytes per pixel
            framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->width * 2) * scanline_number]);
            for (uint16_t x = 0; x < vgaboard->width; ++x)
            {
                ++scanline_colors;
                // get 4 bytes at a time
                *scanline_colors = *((uint32_t *)(framebuffer_line_start));
                framebuffer_line_start += 4;
            }
            ++scanline_colors;
            break;
        }
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (vgaboard->width - 2);
        buffer->data_used = (vgaboard->width + 4) / 2; // 2 16 bits pixels in each 32 bits word
        scanvideo_end_scanline_generation(buffer);
        counter += 1;
        if (counter>1000) {
            counter = 0;
            vgaboard_toggle_led();
        }
    }
}

void vgaboard_put_pixel(uint16_t x, uint16_t y, uint16_t index_or_color)
{
    uint8_t *byte;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (vgaboard->depth)
    {
    case 1: // 8 pixels per byte
        offset = (vgaboard->width / 8) * y + x / 8;
        if (offset < vgaboard->width * vgaboard->height / 8)
        {
            byte = &vgaboard_framebuffer[offset];
            bit = 7 - (x % 8);
            mask = 1 << bit;
            if (index_or_color)
            {
                // Set bit
                *byte |= mask;
            }
            else
            {
                // Unset bit
                *byte &= ~mask;
            }
        }
        break;
    case 2: // 4 pixels per byte
        offset = (vgaboard->width / 4) * y + x / 4;
        if (offset < vgaboard->width * vgaboard->height / 4)
        {
            byte = &vgaboard_framebuffer[offset];
            switch (x % 4)
            {
            case 0: /* bits 7-6 */
                bits = (index_or_color % 4) << 6;
                mask = 0b00111111;
                break;
            case 1: /* bits 5-4 */
                bits = (index_or_color % 4) << 4;
                mask = 0b11001111;
                break;
            case 2: /* bits 3-2 */
                bits = (index_or_color % 4) << 2;
                mask = 0b11110011;
                break;
            case 3: /* bits 1-0 */
                bits = (index_or_color % 4);
                mask = 0b11111100;
                break;
            }
            *byte &= mask;
            *byte |= bits;
        }
        break;
    case 4: // 2 pixels per byte
        offset = (vgaboard->width / 2) * y + x / 2;
        if (offset < vgaboard->width * vgaboard->height / 2)
        {
            byte = &vgaboard_framebuffer[offset];
            if (x & 1)
            {
                *byte = ((index_or_color & 0x0f) << 4) | (*byte & 0x0f);
            }
            else
            {
                *byte = (*byte & 0xf0) | (index_or_color & 0x0f);
            }
        }
        break;
    case 8: // 1 pixel per byte
        offset = vgaboard->width * y + x;
        if (offset < vgaboard->width * vgaboard->height)
        {
            vgaboard_framebuffer[offset] = index_or_color;
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel
        offset = (vgaboard->width * y + x) * 2;
        if (offset < vgaboard->width * vgaboard->height * 2)
        {
            vgaboard_framebuffer[offset + 0] = index_or_color >> 8;
            vgaboard_framebuffer[offset + 1] = index_or_color & 0xff;
        }
        break;
    default:
        break;
    }
}

uint8_t vgaboard_get_pixel_index(uint16_t x, uint16_t y)
{
    uint16_t index_or_color = 0;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (vgaboard->depth)
    {
    case 1: // 8 pixels per byte
        /* TEST! */
        offset = (vgaboard->width / 8) * y + x / 8;
        if (offset < vgaboard->width * vgaboard->height / 8)
        {
            bit = 7 - (x % 8);
            mask = 1 << bit;
            index_or_color = vgaboard_framebuffer[offset] & mask ? 1 : 0;
        }
        break;
    case 2: // 4 pixels per byte
        /* TEST! */
        offset = (vgaboard->width / 4) * y + x / 4;
        if (offset < vgaboard->width * vgaboard->height / 4)
        {
            switch (x % 4)
            {
            case 0: /* bits 7-6 */
                bits = 6;
                mask = 0b00111111;
                break;
            case 1: /* bits 5-4 */
                bits = 4;
                mask = 0b11001111;
                break;
            case 2: /* bits 3-2 */
                bits = 2;
                mask = 0b11110011;
                break;
            case 3: /* bits 1-0 */
                bits = 0;
                mask = 0b11111100;
                break;
            }
            index_or_color = (vgaboard_framebuffer[offset] & mask) >> bits;
        }
        break;
    case 4: // 2 pixels per byte
        offset = (vgaboard->width / 2) * y + x / 2;
        if (offset < vgaboard->width * vgaboard->height / 2)
        {
            if (x & 1)
            {
                // odd pixel => right nibble (LSB)
                index_or_color = vgaboard_framebuffer[offset] & 0x0f;
            }
            else
            {
                // even pixel => left nibble (MSB)
                index_or_color = vgaboard_framebuffer[offset] >> 4;
            }
        }
        break;
    case 8: // 1 pixel per byte
        offset = vgaboard->width * y + x;
        if (offset < vgaboard->width * vgaboard->height)
        {
            index_or_color = vgaboard_framebuffer[offset];
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel
        /* UNTESTED! */
        offset = (vgaboard->width * y + x) * 2;
        if (offset < vgaboard->width * vgaboard->height * 2)
        {
            index_or_color =
                vgaboard_framebuffer[offset + 0] << 8 |
                vgaboard_framebuffer[offset + 1];
        }
        break;
    default:
        break;
    }
    return index_or_color;
}

uint16_t vgaboard_get_palette_color(uint8_t index)
{
    switch (vgaboard->depth)
    {
    case 1: // 0-1
        return vgaboard->palette[index & 0b00000001];
    case 2: // 0-3
        return vgaboard->palette[index & 0b00000011];
    case 4: // 0-15
        return vgaboard->palette[index & 0b00001111];
    case 8: // 0-255
        return vgaboard->palette[index];
    default:
        return 0;
        break;
    }
}

/**
 * @brief Retrieve BGAR5515 color for pixel at (x, y) coordinates
 */
uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y)
{
    uint8_t index = vgaboard_get_pixel_index(x, y);
    if (vgaboard->depth == 16)
    {
        return index;
    }
    return vgaboard_get_palette_color(index);
}

// EOF
