/*

MIT License

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

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
#include <stdlib.h>
#include <string.h>

#if !PICO_NO_HARDWARE
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#endif
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/stdlib.h"

#include "pico-vgaboard.h"

#if !PICO_NO_HARDWARE
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

#define RAM __not_in_flash("pico_vgaboard_data")

// static mutex_t RAM vgaboard_mutex;

uint8_t RAM _vgaboard_framebuffer[PICO_VGABOARD_FRAMEBUFFER_SIZE];
uint8_t RAM *vgaboard_framebuffer = (u_int8_t *)(&_vgaboard_framebuffer);

/* 1, 2, 4 & 8bpp palette [RAM, 512 bytes] */
uint16_t RAM _vgaboard_palette[256];

/* Specific to 1 bit depth / 2 colors mode [RAM, 16 bytes] */
uint32_t RAM vgaboard_double_palette_1bpp[2 * 2];

/* Specific to 2 bit depth / 4 colors mode [RAM, 64 bytes] */
uint32_t RAM vgaboard_double_palette_2bpp[4 * 4];

/* Specific to 4 bits depth / 16 colors mode [RAM, 1024 bytes] */
uint32_t RAM vgaboard_double_palette_4bpp[16 * 16];

vgaboard_t RAM _vgaboard = {
    .palette = _vgaboard_palette};
vgaboard_t RAM *vgaboard = &_vgaboard;

void vgaboard_init_led()
{
#if USE_ONBOARD_LED == 1
    // We use the onboard LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
#endif
}

void vgaboard_flash_led_and_wait()
{
#if USE_ONBOARD_LED == 1
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
#if USE_ONBOARD_LED == 1
    gpio_put(PICO_DEFAULT_LED_PIN, gpio_get(PICO_DEFAULT_LED_PIN) ? 0 : 1);
#endif
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
    if (vgaboard->depth > 8)
    {
        return;
    }
    // #if PICO_VGABOARD_DEBUG
    //     printf("VGABOARD: PALETTE %p\n", palette);
    // #endif
    // Copy palette to RAM
    for (uint16_t i = 0; i < vgaboard->colors; i += 1)
    {
        vgaboard->palette[i] = palette[i];
        // #if PICO_VGABOARD_DEBUG
        //         printf("VGABOARD: PALETTE[%d] = %d\n", i, palette[i]);
        // #endif
    }
    // Setup double palettes
    vgaboard_setup_double_palette_1bpp();
    vgaboard_setup_double_palette_2bpp();
    vgaboard_setup_double_palette_4bpp();
}

void scanvideo_dump(const scanvideo_mode_t *scanvideo_mode)
{
#if PICO_VGABOARD_DEBUG
    printf("*** SCANVIDEO %p ***\n", scanvideo_mode);
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
#if !PICO_NO_HARDWARE
    // Set system clock
#if PICO_VGABOARD_DEBUG
    printf("SYSTEM CLOCK: SETUP INIT: %d kHz\n", sys_clock_khz);
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
#else
    return true;
#endif
}

void vgaboard_setup(const vgaboard_t *model, uint16_t display_width, uint16_t display_height, uint16_t border_color)
{
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_setup INIT\n");
#endif
    // mutex_init(&vgaboard_mutex);
    vgaboard->scanvideo_mode = model->scanvideo_mode;
    vgaboard->freq_hz = model->freq_hz;
    vgaboard->width = model->scanvideo_mode->width / model->scanvideo_mode->xscale;
    vgaboard->height = model->scanvideo_mode->height / model->scanvideo_mode->yscale;
    // NB: yscale_denominator ignored
    vgaboard->depth = model->depth;
    vgaboard->colors = 1 << model->depth;
    vgaboard->framebuffer = vgaboard_framebuffer;
    vgaboard->framebuffer_size = PICO_VGABOARD_FRAMEBUFFER_SIZE;
    vgaboard->sys_clock_khz = model->sys_clock_khz;
    vgaboard->vreg_voltage = model->vreg_voltage;
#if !PICO_NO_HARDWARE
    if (vgaboard->vreg_voltage == 0)
    {
        vgaboard->vreg_voltage = VREG_VOLTAGE_DEFAULT;
    }
    else
    {
#if PICO_VGABOARD_DEBUG
        printf("\t=> vgaboard_setup VREG_VOLTAGE=%08b\n", vgaboard->vreg_voltage);
#endif
        vreg_set_voltage(vgaboard->vreg_voltage);
    }
#endif
    vgaboard_set_system_clock(vgaboard->sys_clock_khz);
    vgaboard_set_palette(model->palette);
    // display window
    /* clang-format off */
    vgaboard->display_width       = display_width  > 0 && display_width  < vgaboard->width  ? display_width  : vgaboard->width ;
    vgaboard->display_height      = display_height > 0 && display_height < vgaboard->height ? display_height : vgaboard->height;
    vgaboard->horizontal_margin   = (vgaboard->width  - vgaboard->display_width ) / 2;
    vgaboard->vertical_margin     = (vgaboard->height - vgaboard->display_height) / 2;
    vgaboard->has_margins         = vgaboard->horizontal_margin > 0 || vgaboard->vertical_margin > 0;
    vgaboard->border_color_top    = border_color;
    vgaboard->border_color_left   = border_color;
    vgaboard->border_color_bottom = border_color;
    vgaboard->border_color_right  = border_color;
    /* clang-format on */
    // scanvideo_setup(vgaboard->scanvideo_mode);
#if PICO_VGABOARD_DEBUG
    printf("\t=> vgaboard_setup DONE\n");
#endif
}

// void vgaboard_change(const vgaboard_t *model)
// {
// #if PICO_VGABOARD_DEBUG
//     printf("\t=> vgaboard_change INIT\n");
// #endif
//     vgaboard->scanvideo_mode = model->scanvideo_mode;
//     vgaboard->width          = model->scanvideo_mode->width / model->scanvideo_mode->xscale;
//     vgaboard->height         = model->scanvideo_mode->height / model->scanvideo_mode->yscale;
//     // NB: yscale_denominator ignored
//     vgaboard->depth          = model->depth;
//     vgaboard->colors         = 1 << model->depth;
//     vgaboard_set_palette(model->palette);
//     scanvideo_setup(vgaboard->scanvideo_mode);
// #if PICO_VGABOARD_DEBUG
//     printf("\t=> vgaboard_change DONE\n");
// #endif
// }

// void vgaboard_enable()
// {
// #if PICO_VGABOARD_DEBUG
//     printf("VGABOARD: ENABLE\n");
//     sleep_ms(100);
// #endif
//     scanvideo_timing_enable(true);
// }

// void vgaboard_disable()
// {
// #if PICO_VGABOARD_DEBUG
//     printf("VGABOARD: DISABLE\n");
//     sleep_ms(100);
// #endif
//     scanvideo_timing_enable(false);
// }

#if PICO_VGABOARD_DEBUG
uint64_t vgaboard_frame_counter = 0;
#endif

void __not_in_flash("pico_vgaboard_code")(vgaboard_render_loop)(void)
{
#if USE_ONBOARD_LED
    int counter = 0;
#endif
#if PICO_VGABOARD_DEBUG
#if !PICO_NO_HARDWARE
    printf("VGABOARD: Starting render screen: %dx%dx%d/%d@%dHz display: %dx%d margins: %d/%d (%dMHz)\n",
           vgaboard->width, vgaboard->height, vgaboard->depth, vgaboard->colors, vgaboard->freq_hz,
           vgaboard->display_width, vgaboard->display_height,
           vgaboard->horizontal_margin, vgaboard->vertical_margin,
           clock_get_hz(clk_sys) / 1000000);
#else
    printf("VGABOARD: Starting render screen: %dx%dx%d/%d@%dHz display: %dx%d margins: %d/%d\n",
           vgaboard->width, vgaboard->height, vgaboard->depth, vgaboard->colors, vgaboard->freq_hz,
           vgaboard->display_width, vgaboard->display_height,
           vgaboard->horizontal_margin, vgaboard->vertical_margin);
#endif
#endif
#if !PICO_NO_HARDWARE
#if USE_INTERP == 1
    if (vgaboard->depth == 4)
    {
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
    }
#endif
#endif
    // Let's go for the show!
    scanvideo_setup(vgaboard->scanvideo_mode);
    scanvideo_timing_enable(true);
    /* clang-format off */
    uint32_t border_color_top_32    = (uint32_t)(vgaboard->border_color_top   ) << 16 | (uint32_t)(vgaboard->border_color_top   );
    uint32_t border_color_left_32   = (uint32_t)(vgaboard->border_color_left  ) << 16 | (uint32_t)(vgaboard->border_color_left  );
    uint32_t border_color_bottom_32 = (uint32_t)(vgaboard->border_color_bottom) << 16 | (uint32_t)(vgaboard->border_color_bottom);
    uint32_t border_color_right_32  = (uint32_t)(vgaboard->border_color_right ) << 16 | (uint32_t)(vgaboard->border_color_right );
    // uint32_t border_color_left_32   = (uint32_t)(vgaboard->border_color_left  ) << 16 | (uint32_t)(vgaboard->border_color_right );
    // uint32_t border_color_right_32  = (uint32_t)(vgaboard->border_color_right ) << 16 | (uint32_t)(vgaboard->border_color_left  );
    /* clang-format on */
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        uint16_t scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *scanline_colors = buffer->data;
        uint8_t *framebuffer_line_start;
        uint8_t bits, bits76, bits54, bits32, bits10, bits7654, bits3210;
        bool in_display_area = true;
        uint16_t display_line = scanline_number;
        // uint32_t *scanline_colors0 = buffer->data;
        // uint16_t pixel_count = 0;
        if (vgaboard->has_margins)
        {
            if ((scanline_number < vgaboard->vertical_margin) ||
                (scanline_number > vgaboard->display_height + vgaboard->vertical_margin - 1))
            {
                /* in top margin or bottom margin => 1 line of pixels with corresponding border color */
                in_display_area = false;
                // uint32_t border_color_32 = scanline_number < vgaboard->vertical_margin ? border_color_top_32 : border_color_bottom_32;
                // uint32_t border_color = (uint32_t)(vgaboard->palette[scanline_number % vgaboard->colors]);
                uint32_t border_color = scanline_number % 2 == 0 
                    ? PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55) 
                    : PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa);
                if (
                    scanline_number == vgaboard->vertical_margin - 1 ||
                    scanline_number == vgaboard->display_height + vgaboard->vertical_margin)
                {
                    border_color = PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xff, 0xff, 0xff);
                }
                uint32_t border_color_32 = border_color << 16 | border_color;
                for (uint16_t i = 0; i < vgaboard->width / 2; i++)
                {
                    *++scanline_colors = border_color_32;
                }
                ++scanline_colors;
                // pixel_count += vgaboard->width;
            }
            else
            {
                display_line = scanline_number - vgaboard->vertical_margin;
            }
        }
        if (in_display_area)
        {
            // left margin
            if (vgaboard->horizontal_margin > 0)
            {
                for (uint16_t i = 0; i < vgaboard->horizontal_margin / 2; ++i)
                {
                    ++scanline_colors;
                    *scanline_colors = border_color_left_32;
                    // *scanline_colors = (uint32_t)(vgaboard->palette[i % 16]) << 16 | (uint32_t)(vgaboard->palette[i % 16]);
                    // pixel_count += 2;
                }
                // ++scanline_colors;
            }
            // image from framebuffer
            switch (vgaboard->depth)
            {
            case 1: // 1bpp, 8 pixels per byte
                framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->display_width / 8) * display_line]);
                for (uint16_t byte = 0; byte < vgaboard->display_width / 8; ++byte)
                {
                    // 76543210 => 8 pixels to 8 x 16 bits => 4 x 32 bits in buffer
                    // mutex_enter_blocking(&vgaboard_mutex);
                    bits = *framebuffer_line_start;
                    // mutex_exit(&vgaboard_mutex);
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
                    // pixel_count += 8;
                }
                ++scanline_colors;
                break;
            case 2: // 2bpp, 4 pixels per byte
                framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->display_width / 4) * display_line]);
                for (uint16_t x = 0; x < vgaboard->display_width / 4; ++x)
                {
                    // 76543210 => 4 pixels to 4 x 16 bits => 4 x 32 bits in buffer
                    bits = *framebuffer_line_start;
                    bits7654 = (bits & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) >> 4;
                    *++scanline_colors = vgaboard_double_palette_2bpp[bits7654];
                    bits3210 = (bits & (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0)) >> 0;
                    *++scanline_colors = vgaboard_double_palette_2bpp[bits3210];
                    // Next byte / 4 pixels
                    ++framebuffer_line_start;
                    // pixel_count += 4;
                }
                ++scanline_colors;
                break;
            case 4: // 4bpp, 2 pixels per byte
                framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->display_width / 2) * display_line]);
#if USE_INTERP == 1
#if !PICO_NO_HARDWARE
                ++scanline_colors;
                convert_from_pal16(scanline_colors, framebuffer_line_start, vgaboard->display_width / 2);
                scanline_colors += vgaboard->display_width / 2;
                // pixel_count += vgaboard->display_width;
#endif
#else
                for (uint16_t x = 0; x < vgaboard->display_width / 2; ++x)
                {
                    bits = *framebuffer_line_start;
                    ++scanline_colors;
                    *scanline_colors = vgaboard_double_palette_4bpp[bits];
                    ++framebuffer_line_start;
                    // pixel_count += 2;
                }
                ++scanline_colors;
#endif
                break;
            case 8: // 8bpp, 1 pixel per byte
                framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->display_width / 1) * display_line]);
                // append 2 16 bits pixels in the scanline, hence width / 2
                for (uint16_t x = 0; x < vgaboard->display_width / 2; ++x)
                {
                    uint8_t pixel1 = *framebuffer_line_start++;
                    uint8_t pixel2 = *framebuffer_line_start++;
                    uint32_t color1 = vgaboard->palette[pixel1];
                    uint32_t color2 = vgaboard->palette[pixel2];
                    ++scanline_colors;
                    *scanline_colors = (color2 << 16) | color1;
                    // pixel_count += 1;
                }
                ++scanline_colors;
                break;
            case 16: // 16bpp, 1 pixel per word / 2 bytes per pixel
                framebuffer_line_start = &(vgaboard->framebuffer[(vgaboard->display_width * 2) * display_line]);
                for (uint16_t x = 0; x < vgaboard->display_width; ++x)
                {
                    ++scanline_colors;
                    // get 4 bytes at a time
                    *scanline_colors = *((uint32_t *)(framebuffer_line_start));
                    framebuffer_line_start += 4;
                    // pixel_count += 2;
                }
                ++scanline_colors;
                break;
            }
            // right margin
            if (vgaboard->horizontal_margin > 0)
            {
                for (uint16_t i = 0; i < vgaboard->horizontal_margin / 2; ++i)
                {
                    // we already point to a free location
                    // ++scanline_colors;
                    *scanline_colors = border_color_right_32;
                    ++scanline_colors;
                    // pixel_count += 2;
                }
                // we already point to a free location
                // ++scanline_colors;
            }
        }
        // uint32_t *scanline_colors1 = scanline_colors;
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (vgaboard->width - 2);
        buffer->data_used = (vgaboard->width + 4) / 2; // 2 16 bits pixels in each 32 bits word
        scanvideo_end_scanline_generation(buffer);
#if USE_ONBOARD_LED
        counter += 1;
        if (counter > 1000)
        {
            counter = 0;
            vgaboard_toggle_led();
            // printf(
            //     "scanlines_color: %s %04d %p %p %ld %d\n",
            //     in_display_area ? "DISPLAY" : "BORDER ",
            //     scanline_number, scanline_colors0, scanline_colors1,
            //     scanline_colors1 - scanline_colors0,
            //     pixel_count);
        }
#endif
    }
}

void vgaboard_put_pixel(uint16_t x, uint16_t y, uint16_t pixel)
{
    uint8_t *byte;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (vgaboard->depth)
    {
    case 1: // 8 pixels per byte, monochrome
        offset = (vgaboard->display_width / 8) * y + x / 8;
        if (offset < vgaboard->display_width * vgaboard->display_height / 8)
        {
            byte = &vgaboard_framebuffer[offset];
            bit = 7 - (x % 8);
            mask = 1 << bit;
            if (pixel)
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
    case 2: // 4 pixels per byte, 4 colors
        offset = (vgaboard->display_width / 4) * y + x / 4;
        if (offset < vgaboard->display_width * vgaboard->display_height / 4)
        {
            byte = &vgaboard_framebuffer[offset];
            switch (x % 4)
            {
            case 0: /* bits 7-6 */
                bits = (pixel % 4) << 6;
                mask = 0b00111111;
                break;
            case 1: /* bits 5-4 */
                bits = (pixel % 4) << 4;
                mask = 0b11001111;
                break;
            case 2: /* bits 3-2 */
                bits = (pixel % 4) << 2;
                mask = 0b11110011;
                break;
            case 3: /* bits 1-0 */
                bits = (pixel % 4);
                mask = 0b11111100;
                break;
            }
            *byte &= mask;
            *byte |= bits;
        }
        break;
    case 4: // 2 pixels per byte, 16 colors
        offset = (vgaboard->display_width / 2) * y + x / 2;
        if (offset < vgaboard->display_width * vgaboard->display_height / 2)
        {
            byte = &vgaboard_framebuffer[offset];
            if (x & 1)
            {
                *byte = ((pixel & 0x0f) << 4) | (*byte & 0x0f);
            }
            else
            {
                *byte = (*byte & 0xf0) | (pixel & 0x0f);
            }
        }
        break;
    case 8: // 1 pixel per byte, 256 colors
        offset = vgaboard->display_width * y + x;
        if (offset < vgaboard->display_width * vgaboard->display_height)
        {
            vgaboard_framebuffer[offset] = pixel;
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel, 32768 colors
        offset = (vgaboard->display_width * y + x) * 2;
        if (offset < vgaboard->display_width * vgaboard->display_height * 2)
        {
            vgaboard_framebuffer[offset + 0] = pixel >> 8;
            vgaboard_framebuffer[offset + 1] = pixel & 0xff;
        }
        break;
    default:
        break;
    }
}

uint16_t vgaboard_get_pixel_index(uint16_t x, uint16_t y)
{
    uint16_t pixel = 0;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (vgaboard->depth)
    {
    case 1: // 8 pixels per byte
        offset = (vgaboard->display_width / 8) * y + x / 8;
        if (offset < vgaboard->display_width * vgaboard->display_height / 8)
        {
            bit = 7 - (x % 8);
            mask = 1 << bit;
            pixel = vgaboard_framebuffer[offset] & mask ? 1 : 0;
        }
        break;
    case 2: // 4 pixels per byte
        offset = (vgaboard->display_width / 4) * y + x / 4;
        if (offset < vgaboard->display_width * vgaboard->display_height / 4)
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
            pixel = (vgaboard_framebuffer[offset] & mask) >> bits;
        }
        break;
    case 4: // 2 pixels per byte
        offset = (vgaboard->display_width / 2) * y + x / 2;
        if (offset < vgaboard->display_width * vgaboard->display_height / 2)
        {
            if (x & 1)
            {
                // odd pixel => right nibble (LSB)
                pixel = vgaboard_framebuffer[offset] & 0x0f;
            }
            else
            {
                // even pixel => left nibble (MSB)
                pixel = vgaboard_framebuffer[offset] >> 4;
            }
        }
        break;
    case 8: // 1 pixel per byte
        offset = vgaboard->display_width * y + x;
        if (offset < vgaboard->display_width * vgaboard->display_height)
        {
            pixel = vgaboard_framebuffer[offset];
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel
        offset = (vgaboard->display_width * y + x) * 2;
        if (offset < vgaboard->display_width * vgaboard->display_height * 2)
        {
            pixel =
                vgaboard_framebuffer[offset + 0] << 8 |
                vgaboard_framebuffer[offset + 1];
        }
        break;
    }
    return pixel;
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
    }
    return 0;
}

/**
 * @brief Retrieve BGAR5515 color for pixel at (x, y) coordinates
 */
uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y)
{
    uint16_t pixel = vgaboard_get_pixel_index(x, y);
    if (vgaboard->depth == 16)
    {
        return pixel;
    }
    return vgaboard_get_palette_color(pixel & 0xff);
}

// EOF
