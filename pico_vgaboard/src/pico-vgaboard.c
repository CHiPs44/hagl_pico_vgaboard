/*

MIT License

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

// #if PICO_VGABOARD_FRAMEBUFFER_SIZE > 0
// uint8_t RAM _pico_vgaboard_framebuffer[PICO_VGABOARD_FRAMEBUFFER_SIZE];
// #endif

uint8_t RAM _pico_vgaboard_vram[PICO_VGABOARD_VRAM_SIZE];

/* 1, 2, 4 & 8bpp palette [RAM, 512 bytes] */
BGAR5515 RAM _pico_vgaboard_palette[256];

/* Specific to 1 bit depth / 2 colors mode [RAM, 16 bytes] */
uint32_t RAM pico_vgaboard_double_palette_1bpp[2 * 2];

/* Specific to 2 bit depth / 4 colors mode [RAM, 64 bytes] */
uint32_t RAM pico_vgaboard_double_palette_2bpp[4 * 4];

/* Specific to 4 bits depth / 16 colors mode [RAM, 1024 bytes] */
uint32_t RAM pico_vgaboard_double_palette_4bpp[16 * 16];

pico_vgaboard_t RAM _pico_vgaboard = {
    .palette = _pico_vgaboard_palette};
pico_vgaboard_t RAM *pico_vgaboard = &_pico_vgaboard;

uint64_t pico_vgaboard_frame_counter = 0;

void pico_vgaboard_init_led()
{
#if USE_ONBOARD_LED == 1
    // We use the onboard LED to show activity
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
#endif
}

void pico_vgaboard_flash_led_and_wait()
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

void pico_vgaboard_toggle_led()
{
#if USE_ONBOARD_LED == 1
    gpio_put(PICO_DEFAULT_LED_PIN, gpio_get(PICO_DEFAULT_LED_PIN) ? 0 : 1);
#endif
}

void pico_vgaboard_start_double_palette_1bpp()
{
    if (pico_vgaboard->depth != 1 || pico_vgaboard->palette == NULL)
    {
        return;
    }
    /* pico_vgaboard_double_palette_1bpp is 4 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_1 = pico_vgaboard_double_palette_1bpp;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            *double_palette_1 = (pico_vgaboard->palette[j] << 16) | pico_vgaboard->palette[i];
            ++double_palette_1;
        }
    }
}

void pico_vgaboard_start_double_palette_2bpp()
{
    if (pico_vgaboard->depth != 2 || pico_vgaboard->palette == NULL)
    {
        return;
    }
    /* pico_vgaboard_double_palette_2bpp is 16 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_2 = pico_vgaboard_double_palette_2bpp;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            *double_palette_2 = (pico_vgaboard->palette[j] << 16) | pico_vgaboard->palette[i];
            ++double_palette_2;
        }
    }
}

void pico_vgaboard_start_double_palette_4bpp()
{
    if (pico_vgaboard->depth != 4 || pico_vgaboard->palette == NULL)
    {
        return;
    }
    /* pico_vgaboard_double_palette_4bpp is 256 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_4 = pico_vgaboard_double_palette_4bpp;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            *double_palette_4 = (pico_vgaboard->palette[i] << 16) | pico_vgaboard->palette[j];
            ++double_palette_4;
        }
    }
}

void pico_vgaboard_set_palette(const BGAR5515 *palette)
{
    if (pico_vgaboard->depth > 8)
    {
        return;
    }
    // #if PICO_VGABOARD_DEBUG
    //     printf("VGABOARD: PALETTE %p\n", palette);
    // #endif
    // Copy palette to RAM
    for (uint16_t i = 0; i < pico_vgaboard->colors; i += 1)
    {
        pico_vgaboard->palette[i] = palette[i];
        // #if PICO_VGABOARD_DEBUG
        //         printf("VGABOARD: PALETTE[%d] = %d\n", i, palette[i]);
        // #endif
    }
    // Setup double palettes
    pico_vgaboard_start_double_palette_1bpp();
    pico_vgaboard_start_double_palette_2bpp();
    pico_vgaboard_start_double_palette_4bpp();
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

void pico_vgaboard_dump(const pico_vgaboard_t *pico_vgaboard)
{
#if PICO_VGABOARD_DEBUG
    printf("*** VGABOARD %p ***\n", pico_vgaboard);
    printf("\tWidth: %d\tHeight: %d\n\tDepth: %d\tColors: %d\n\tFramebuffer:%p\tFramebuffer Size: %d\n\tPalette: %p\n",
           pico_vgaboard->width, pico_vgaboard->height,
           pico_vgaboard->depth, pico_vgaboard->colors,
           pico_vgaboard->framebuffer, pico_vgaboard->framebuffer_size,
           pico_vgaboard->palette);
#endif
}

void pico_vgaboard_init()
{
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_init INIT\n");
#endif
    // One time initializations
    pico_vgaboard_init_led();
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_init DONE\n");
#endif
}

bool pico_vgaboard_set_system_clock(uint32_t sys_clock_khz)
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
    pico_vgaboard_flash_led_and_wait();
    stdio_init_all();
    pico_vgaboard_flash_led_and_wait();
    pico_vgaboard_flash_led_and_wait();
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

void pico_vgaboard_start(const pico_vgaboard_t *model, uint16_t display_width, uint16_t display_height, BGAR5515 border_color)
{
    /* clang-format off */
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_start INIT\n");
#endif
    // mutex_init(&vgaboard_mutex);
    pico_vgaboard->scanvideo_active     = false;
    pico_vgaboard->scanvideo_mode       = model->scanvideo_mode;
    pico_vgaboard->freq_hz              = model->freq_hz;
    pico_vgaboard->width                = model->scanvideo_mode->width;
    pico_vgaboard->height               = model->scanvideo_mode->height;
    // NB: yscale_denominator ignored
    pico_vgaboard->depth                = model->depth;
    pico_vgaboard->colors               = 1 << model->depth;
    pico_vgaboard->sys_clock_khz        = model->sys_clock_khz;
    pico_vgaboard->vreg_voltage         = model->vreg_voltage;
#if !PICO_NO_HARDWARE
    if (pico_vgaboard->vreg_voltage == 0)
    {
        pico_vgaboard->vreg_voltage     = VREG_VOLTAGE_DEFAULT;
    }
    else
    {
#if PICO_VGABOARD_DEBUG
        printf("\t=> pico_vgaboard_start VREG_VOLTAGE=%08b\n", pico_vgaboard->vreg_voltage);
#endif
        vreg_set_voltage(pico_vgaboard->vreg_voltage);
    }
#endif
    pico_vgaboard_set_system_clock(pico_vgaboard->sys_clock_khz);
    pico_vgaboard_set_palette(model->palette);
    // Display window
    pico_vgaboard->display_width        = display_width  > 0 && display_width  < pico_vgaboard->width  ? display_width  : pico_vgaboard->width ;
    pico_vgaboard->display_height       = display_height > 0 && display_height < pico_vgaboard->height ? display_height : pico_vgaboard->height;
    pico_vgaboard->horizontal_margin    = (pico_vgaboard->width  - pico_vgaboard->display_width ) / 2;
    pico_vgaboard->vertical_margin      = (pico_vgaboard->height - pico_vgaboard->display_height) / 2;
    pico_vgaboard->has_margins          = pico_vgaboard->horizontal_margin > 0 || pico_vgaboard->vertical_margin > 0;
    pico_vgaboard->border_color_top     = border_color;
    pico_vgaboard->border_color_left    = border_color;
    pico_vgaboard->border_color_bottom  = border_color;
    pico_vgaboard->border_color_right   = border_color;
    // VRAM
    pico_vgaboard->vram_size            = PICO_VGABOARD_VRAM_SIZE;
    pico_vgaboard->vram                 = _pico_vgaboard_vram;
    pico_vgaboard->framebuffer_size     = pico_vgaboard->depth <= 8 
        ? pico_vgaboard->display_width * pico_vgaboard->display_height * pico_vgaboard->depth / 8
        : pico_vgaboard->display_width * pico_vgaboard->display_height * pico_vgaboard->depth * 2
    ;
    if (pico_vgaboard->framebuffer_size > pico_vgaboard->vram_size) {
#if PICO_VGABOARD_DEBUG
        printf(
            "\t=> pico_vgaboard_start /!\\ FRAMEBUFFER_SIZE (%d) > VRAM_SIZE (%d) /!\\\n", 
            pico_vgaboard->framebuffer_size, pico_vgaboard->vram_size
        );
#endif
        pico_vgaboard->framebuffer_size = pico_vgaboard->vram_size;
    }
    // For now, always have framebuffer at offset 0 of vram
    pico_vgaboard->framebuffer          = pico_vgaboard->vram;
    // #if PICO_VGABOARD_FRAMEBUFFER_SIZE > 0
    //     pico_vgaboard->framebuffer          = pico_vgaboard_framebuffer;
    //     pico_vgaboard->framebuffer          = _pico_vgaboard_framebuffer;
    //     pico_vgaboard->framebuffer_size     = PICO_VGABOARD_FRAMEBUFFER_SIZE;
    // #else
    //     pico_vgaboard->framebuffer_size     = display_width * display_height * pico_vgaboard->depth / 8;
    //     // pico_vgaboard_framebuffer           = malloc(pico_vgaboard->framebuffer_size);
    //     // pico_vgaboard->framebuffer          = pico_vgaboard_framebuffer;
    //     pico_vgaboard->framebuffer          = malloc(pico_vgaboard->framebuffer_size);
    //     printf("MALLOC: %p (%d)\r\n", pico_vgaboard->framebuffer, pico_vgaboard->framebuffer_size);
    // #endif
    // scanvideo_setup(pico_vgaboard->scanvideo_mode);
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_start DONE\n");
#endif
    /* clang-format on */
}

// void pico_vgaboard_change(const pico_vgaboard_t *model)
// {
// #if PICO_VGABOARD_DEBUG
//     printf("\t=> pico_vgaboard_change INIT\n");
// #endif
//     pico_vgaboard->scanvideo_mode = model->scanvideo_mode;
//     vgaboard->width          = model->scanvideo_mode->width / model->scanvideo_mode->xscale;
//     pico_vgaboard->height         = model->scanvideo_mode->height / model->scanvideo_mode->yscale;
//     // NB: yscale_denominator ignored
//     pico_vgaboard->depth          = model->depth;
//     pico_vgaboard->colors         = 1 << model->depth;
//     pico_vgaboard_set_palette(model->palette);
//     scanvideo_setup(pico_vgaboard->scanvideo_mode);
// #if PICO_VGABOARD_DEBUG
//     printf("\t=> pico_vgaboard_change DONE\n");
// #endif
// }

// void pico_vgaboard_enable()
// {
// #if PICO_VGABOARD_DEBUG
//     printf("VGABOARD: ENABLE\n");
//     sleep_ms(100);
// #endif
//     scanvideo_timing_enable(true);
// }

// void pico_vgaboard_disable()
// {
// #if PICO_VGABOARD_DEBUG
//     printf("VGABOARD: DISABLE\n");
//     sleep_ms(100);
// #endif
//     scanvideo_timing_enable(false);
// }

void __not_in_flash("pico_vgaboard_code")(pico_vgaboard_render_loop)(void)
{
    uint32_t border_color_top_32, border_color_left_32, border_color_bottom_32, border_color_right_32;
    struct scanvideo_scanline_buffer *buffer;
    uint16_t scanline_number;
    uint32_t *scanline_colors;
    uint8_t *framebuffer_line_start;
    uint8_t bits, bits76, bits54, bits32, bits10, bits7654, bits3210;
    bool in_display_area;
    uint16_t display_line;
#if USE_ONBOARD_LED
    int scanvideo_line_counter = 0;
#endif
#if PICO_VGABOARD_DEBUG
#if !PICO_NO_HARDWARE
    printf("VGABOARD: Starting render screen: %dx%dx%d/%d@%dHz display: %dx%d margins: %d/%d (%dMHz)\n",
           pico_vgaboard->width, pico_vgaboard->height, pico_vgaboard->depth, pico_vgaboard->colors, pico_vgaboard->freq_hz,
           pico_vgaboard->display_width, pico_vgaboard->display_height,
           pico_vgaboard->horizontal_margin, pico_vgaboard->vertical_margin,
           clock_get_hz(clk_sys) / 1000000);
#else
    printf("VGABOARD: Starting render screen: %dx%dx%d/%d@%dHz display: %dx%d margins: %d/%d\n",
           pico_vgaboard->width, pico_vgaboard->height, pico_vgaboard->depth, pico_vgaboard->colors, pico_vgaboard->freq_hz,
           pico_vgaboard->display_width, pico_vgaboard->display_height,
           pico_vgaboard->horizontal_margin, pico_vgaboard->vertical_margin);
#endif
#endif
#if !PICO_NO_HARDWARE && USE_INTERP == 1
    if (pico_vgaboard->depth == 4)
    {
        // Configure interpolator lanes for 4bbp
        interp_config c = interp_default_config();
        interp_config_set_shift(&c, 22);
        interp_config_set_mask(&c, 2, 9);
        interp_set_config(interp0, 0, &c);
        interp_config_set_shift(&c, 14);
        interp_config_set_cross_input(&c, true);
        interp_set_config(interp0, 1, &c);
        interp_set_base(interp0, 0, (uintptr_t)pico_vgaboard_double_palette_4bpp);
        interp_set_base(interp0, 1, (uintptr_t)pico_vgaboard_double_palette_4bpp);
    }
#endif
    // Let's go for the show!
    scanvideo_setup(pico_vgaboard->scanvideo_mode);
    scanvideo_timing_enable(true);
    pico_vgaboard->scanvideo_active = true;
    while (true)
    {
        /* clang-format off */
        border_color_top_32    = (uint32_t)(pico_vgaboard->border_color_top   ) << 16 | (uint32_t)(pico_vgaboard->border_color_top   );
        border_color_left_32   = (uint32_t)(pico_vgaboard->border_color_left  ) << 16 | (uint32_t)(pico_vgaboard->border_color_left  );
        border_color_bottom_32 = (uint32_t)(pico_vgaboard->border_color_bottom) << 16 | (uint32_t)(pico_vgaboard->border_color_bottom);
        border_color_right_32  = (uint32_t)(pico_vgaboard->border_color_right ) << 16 | (uint32_t)(pico_vgaboard->border_color_right );
        /* clang-format on */
        buffer = scanvideo_begin_scanline_generation(true);
        scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        scanline_colors = buffer->data;
        in_display_area = true;
        display_line = scanline_number;
        if (pico_vgaboard->has_margins)
        {
            if ((scanline_number < pico_vgaboard->vertical_margin) ||
                (scanline_number > pico_vgaboard->display_height + pico_vgaboard->vertical_margin - 1))
            {
                /* in top margin or bottom margin => 1 line of pixels with corresponding border color */
                in_display_area = false;
                uint32_t border_color_32 = scanline_number < pico_vgaboard->vertical_margin
                                               ? border_color_top_32
                                               : border_color_bottom_32;
                for (uint16_t i = 0; i < pico_vgaboard->width / 2; i++)
                {
                    *++scanline_colors = border_color_32;
                }
                ++scanline_colors;
            }
            else
            {
                display_line = scanline_number - pico_vgaboard->vertical_margin;
            }
        }
        if (in_display_area)
        {
            // left margin
            if (pico_vgaboard->horizontal_margin > 0)
            {
                for (uint16_t i = 0; i < pico_vgaboard->horizontal_margin / 2; ++i)
                {
                    ++scanline_colors;
                    *scanline_colors = border_color_left_32;
                }
            }
            // image from framebuffer
            switch (pico_vgaboard->depth)
            {
            case 1: // 1bpp, 8 pixels per byte
                framebuffer_line_start = &(pico_vgaboard->framebuffer[(pico_vgaboard->display_width / 8) * display_line]);
                for (uint16_t byte = 0; byte < pico_vgaboard->display_width / 8; ++byte)
                {
                    // 76543210 => 8 pixels to 8 x 16 bits => 4 x 32 bits in buffer
                    bits = *framebuffer_line_start;
                    bits76 = (bits & 0b11000000) >> 6;
                    bits54 = (bits & 0b00110000) >> 4;
                    bits32 = (bits & 0b00001100) >> 2;
                    bits10 = (bits & 0b00000011) >> 0;
                    ++scanline_colors;
                    *scanline_colors = pico_vgaboard_double_palette_1bpp[bits76];
                    ++scanline_colors;
                    *scanline_colors = pico_vgaboard_double_palette_1bpp[bits54];
                    ++scanline_colors;
                    *scanline_colors = pico_vgaboard_double_palette_1bpp[bits32];
                    ++scanline_colors;
                    *scanline_colors = pico_vgaboard_double_palette_1bpp[bits10];
                    ++framebuffer_line_start;
                }
                ++scanline_colors;
                break;
            case 2: // 2bpp, 4 pixels per byte
                framebuffer_line_start = &(pico_vgaboard->framebuffer[(pico_vgaboard->display_width / 4) * display_line]);
                for (uint16_t x = 0; x < pico_vgaboard->display_width / 4; ++x)
                {
                    // 76543210 => 4 pixels to 4 x 16 bits => 4 x 32 bits in buffer
                    bits = *framebuffer_line_start;
                    bits7654 = (bits & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) >> 4;
                    *++scanline_colors = pico_vgaboard_double_palette_2bpp[bits7654];
                    bits3210 = (bits & (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0)) >> 0;
                    *++scanline_colors = pico_vgaboard_double_palette_2bpp[bits3210];
                    // Next byte / 4 pixels
                    ++framebuffer_line_start;
                }
                ++scanline_colors;
                break;
            case 4: // 4bpp, 2 pixels per byte
                framebuffer_line_start = &(pico_vgaboard->framebuffer[(pico_vgaboard->display_width / 2) * display_line]);
#if !PICO_NO_HARDWARE && USE_INTERP == 1
                ++scanline_colors;
                convert_from_pal16(scanline_colors, framebuffer_line_start, pico_vgaboard->display_width / 2);
                scanline_colors += pico_vgaboard->display_width / 2;
#else
                for (uint16_t x = 0; x < pico_vgaboard->display_width / 2; ++x)
                {
                    bits = *framebuffer_line_start;
                    ++scanline_colors;
                    *scanline_colors = pico_vgaboard_double_palette_4bpp[bits];
                    ++framebuffer_line_start;
                }
                ++scanline_colors;
#endif
                break;
            case 8: // 8bpp, 1 pixel per byte
                framebuffer_line_start = &(pico_vgaboard->framebuffer[(pico_vgaboard->display_width / 1) * display_line]);
                // append 2 16 bits pixels in the scanline, hence width / 2
                uint32_t color1, color2;
                for (uint16_t x = 0; x < pico_vgaboard->display_width / 2; ++x)
                {
                    color1 = pico_vgaboard->palette[*framebuffer_line_start++];
                    color2 = pico_vgaboard->palette[*framebuffer_line_start++];
                    ++scanline_colors;
                    *scanline_colors = (color2 << 16) | color1;
                }
                ++scanline_colors;
                break;
            case 16: // 16bpp, 1 pixel per word / 2 bytes per pixel
                framebuffer_line_start = &(pico_vgaboard->framebuffer[(pico_vgaboard->display_width * 2) * display_line]);
                for (uint16_t x = 0; x < pico_vgaboard->display_width; ++x)
                {
                    ++scanline_colors;
                    // get 4 bytes at a time
                    *scanline_colors = *((uint32_t *)(framebuffer_line_start));
                    framebuffer_line_start += 4;
                }
                ++scanline_colors;
                break;
            }
            // right margin
            if (pico_vgaboard->horizontal_margin > 0)
            {
                for (uint16_t i = 0; i < pico_vgaboard->horizontal_margin / 2; ++i)
                {
                    // we already point to a free location
                    // ++scanline_colors;
                    *scanline_colors = border_color_right_32;
                    ++scanline_colors;
                }
                // we already point to a free location
                // ++scanline_colors;
            }
        }
        // scanline end
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (pico_vgaboard->width - 2);
        buffer->data_used = (pico_vgaboard->width + 4) / 2; // 2 16 bits pixels in each 32 bits word
        scanvideo_end_scanline_generation(buffer);
#if USE_ONBOARD_LED
        scanvideo_line_counter += 1;
        if (scanvideo_line_counter > 5 * pico_vgaboard->height)
        {
            scanvideo_line_counter = 0;
            pico_vgaboard_toggle_led();
        }
#endif
        pico_vgaboard_frame_counter += 1;
    } /* loop forever */
}

void pico_vgaboard_put_pixel(uint16_t x, uint16_t y, BGAR5515 pixel)
{
    uint8_t *byte;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (pico_vgaboard->depth)
    {
    case 1: // 8 pixels per byte, monochrome
        offset = (pico_vgaboard->display_width / 8) * y + x / 8;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 8)
        {
            byte = &pico_vgaboard->framebuffer[offset];
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
        offset = (pico_vgaboard->display_width / 4) * y + x / 4;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 4)
        {
            byte = &pico_vgaboard->framebuffer[offset];
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
        offset = (pico_vgaboard->display_width / 2) * y + x / 2;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 2)
        {
            byte = &pico_vgaboard->framebuffer[offset];
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
        offset = pico_vgaboard->display_width * y + x;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height)
        {
            pico_vgaboard->framebuffer[offset] = pixel;
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel, 32768 colors
        offset = (pico_vgaboard->display_width * y + x) * 2;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height * 2)
        {
            pico_vgaboard->framebuffer[offset + 0] = pixel >> 8;
            pico_vgaboard->framebuffer[offset + 1] = pixel & 0xff;
        }
        break;
    default:
        break;
    }
}

BGAR5515 pico_vgaboard_get_pixel_index(uint16_t x, uint16_t y)
{
    BGAR5515 pixel = 0;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (pico_vgaboard->depth)
    {
    case 1: // 8 pixels per byte
        offset = (pico_vgaboard->display_width / 8) * y + x / 8;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 8)
        {
            bit = 7 - (x % 8);
            mask = 1 << bit;
            pixel = pico_vgaboard->framebuffer[offset] & mask ? 1 : 0;
        }
        break;
    case 2: // 4 pixels per byte
        offset = (pico_vgaboard->display_width / 4) * y + x / 4;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 4)
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
            pixel = (pico_vgaboard->framebuffer[offset] & mask) >> bits;
        }
        break;
    case 4: // 2 pixels per byte
        offset = (pico_vgaboard->display_width / 2) * y + x / 2;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height / 2)
        {
            if (x & 1)
            {
                // odd pixel => right nibble (LSB)
                pixel = pico_vgaboard->framebuffer[offset] & 0x0f;
            }
            else
            {
                // even pixel => left nibble (MSB)
                pixel = pico_vgaboard->framebuffer[offset] >> 4;
            }
        }
        break;
    case 8: // 1 pixel per byte
        offset = pico_vgaboard->display_width * y + x;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height)
        {
            pixel = pico_vgaboard->framebuffer[offset];
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel
        offset = (pico_vgaboard->display_width * y + x) * 2;
        if (offset < pico_vgaboard->display_width * pico_vgaboard->display_height * 2)
        {
            pixel =
                pico_vgaboard->framebuffer[offset + 0] << 8 |
                pico_vgaboard->framebuffer[offset + 1];
        }
        break;
    }
    return pixel;
}

BGAR5515 pico_vgaboard_get_palette_color(uint8_t index)
{
    switch (pico_vgaboard->depth)
    {
    case 1: // 0-1
        return pico_vgaboard->palette[index & 0b00000001];
    case 2: // 0-3
        return pico_vgaboard->palette[index & 0b00000011];
    case 4: // 0-15
        return pico_vgaboard->palette[index & 0b00001111];
    case 8: // 0-255
        return pico_vgaboard->palette[index];
    }
    return 0;
}

/**
 * @brief Retrieve BGAR5515 color for pixel at (x, y) coordinates
 */
BGAR5515 pico_vgaboard_get_pixel_color(uint16_t x, uint16_t y)
{
    uint16_t pixel = pico_vgaboard_get_pixel_index(x, y);
    if (pico_vgaboard->depth == 16)
    {
        return pixel;
    }
    return pico_vgaboard_get_palette_color(pixel & 0xff);
}

int pico_vgaboard_get_luminance(BGAR5515 rgb)
{
    // cf. https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
    //  we'll stay at this simple formula that works...
    uint8_t r = PICO_SCANVIDEO_R5_FROM_PIXEL(rgb); // << 3;
    uint8_t g = PICO_SCANVIDEO_G5_FROM_PIXEL(rgb); // << 3;
    uint8_t b = PICO_SCANVIDEO_B5_FROM_PIXEL(rgb); // << 3;
    // From 0 to 310,000
    int luminance = 2126 * r + 7152 * g + 722 * b;
    return luminance;
}

// EOF
