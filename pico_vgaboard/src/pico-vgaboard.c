/*

MIT License

Copyright (c) 2021-2024 CHiPs44 <chips44@gmail.com>

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

pico_vgaboard_t PICO_VGABOARD_DATA _pico_vgaboard = {};
pico_vgaboard_t PICO_VGABOARD_DATA *pico_vgaboard = &_pico_vgaboard;

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
    // printf("\tWidth: %d\tHeight: %d\n\tDepth: %d\tColors: %d\n\tFramebuffer:%p\tFramebuffer Size: %d\n\tPalette: %p\n",
    //        pico_vgaboard->width, pico_vgaboard->height,
    //        pico_vgaboard->depth, pico_vgaboard->colors,
    //        pico_vgaboard->framebuffer, pico_vgaboard->framebuffer_size,
    //        pico_vgaboard->palette);
#endif
}

void pico_vgaboard_init()
{
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_init INIT\n");
#endif
    // One time initializations
    pico_vgaboard_init_led();
    pico_vgaboard->planes[0].render_scanline = NULL;
    pico_vgaboard->planes[0].state = NULL;
    pico_vgaboard->planes[1].render_scanline = NULL;
    pico_vgaboard->planes[1].state = NULL;
    pico_vgaboard->planes[2].render_scanline = NULL;
    pico_vgaboard->planes[2].state = NULL;
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
    bool ok;
    if (sys_clock_khz == 292500L)
    {
        /*
        Requested: 292.5 MHz
        Achieved: 292.5 MHz
        REFDIV: 2
        FBDIV: 195 (VCO = 1170.0 MHz)
        PD1: 4
        PD2: 1
        */
        printf("292.5 MHz!!!\n");
        sleep_ms(250);
        set_sys_clock_pll(1170000000L, 4, 1);
        ok = true;
    }
    else
    {
        ok = set_sys_clock_khz(sys_clock_khz, false);
    }
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

void pico_vgaboard_start(const pico_vgaboard_t *model)
{
    /* clang-format off */
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_start INIT\n");
#endif
    // mutex_init(&vgaboard_mutex);
    pico_vgaboard->scanvideo_active     = false;
    pico_vgaboard->scanvideo_mode       = model->scanvideo_mode;
    pico_vgaboard->freq_hz              = model->freq_hz;
    pico_vgaboard->width                = model->width;
    pico_vgaboard->height               = model->height;
    // NB: yscale_denominator ignored
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
    // => on core1
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

void pico_vgaboard_init_plane(int plane, uint8_t type, uint8_t flags, void *state, t_plane_init_func initialize, t_plane_render_scanline_func render_scanline)
{
    pico_vgaboard->planes[plane].type = type;
    pico_vgaboard->planes[plane].flags = flags;
    pico_vgaboard->planes[plane].initialize = initialize;
    pico_vgaboard->planes[plane].render_scanline = render_scanline;
    pico_vgaboard->planes[plane].state = state;
}

// uint16_t __not_in_flash("pico_vgaboard_code")(pico_vgaboard_render_plane2)(uint16_t scanline_number, uint32_t *data, uint16_t data_max)
// {
//     const uint16_t width = pico_vgaboard->width;
//     uint16_t data_used;
//     uint32_t pixel1 = PICO_SCANVIDEO_PIXEL_FROM_RGB5(pico_vgaboard_frame_counter % 32, 0x00, 0x00) | PICO_SCANVIDEO_ALPHA_MASK;
//     // uint32_t pixel2 = PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x00, 31 - pico_vgaboard_frame_counter % 32, 0x00) | PICO_SCANVIDEO_ALPHA_MASK;
//     uint32_t pixel2 = ~PICO_SCANVIDEO_ALPHA_MASK;
//     uint32_t pixels = (pixel1 << 16) | pixel2;

//     int16_t top = pico_vgaboard_frame_counter % pico_vgaboard->height;
//     if (scanline_number >= top && scanline_number < top + 16)
//     {
//         uint32_t *scanline_colors = data;
//         // 4 pixels at a time
//         for (uint16_t i = 0; i < pico_vgaboard->width / 2; i += 2)
//         {
//             *++scanline_colors = pixels;
//             *++scanline_colors = ~pixels;
//         }
//         ++scanline_colors;
//         data[0] = COMPOSABLE_RAW_RUN | (data[1] << 16);
//         data[1] = width - 3 | (data[1] & 0xffff0000);
//         data[width / 2 + 1] = COMPOSABLE_RAW_1P | (0 << 16);
//         data[width / 2 + 2] = COMPOSABLE_EOL_SKIP_ALIGN;
//         data_used = width / 2 + 3;
//     }
//     else
//     {
//         data[0] = COMPOSABLE_RAW_1P | (0 << 16);
//         data[1] = COMPOSABLE_EOL_SKIP_ALIGN;
//         data_used = 2;
//     }

//     return data_used;
// }

// uint16_t __not_in_flash("pico_vgaboard_code")(pico_vgaboard_render_plane3)(uint16_t scanline_number, uint32_t *data, uint16_t data_max)
// {
//     uint16_t data_used;
//     const uint16_t width = pico_vgaboard->width;

//     if (scanline_number >= 48 * 3 && scanline_number < 64 * 3)
//     {
//         data[0] = COMPOSABLE_RAW_RUN | (data[1] << 16);
//         data[1] = width - 3 | (data[1] & 0xffff0000);
//         data[width / 2 + 1] = COMPOSABLE_RAW_1P | (0 << 16);
//         data[width / 2 + 2] = COMPOSABLE_EOL_SKIP_ALIGN;
//         data_used = width / 2 + 3;
//     }
//     else
//     {
//         data[0] = COMPOSABLE_RAW_1P | (0 << 16);
//         data[1] = COMPOSABLE_EOL_SKIP_ALIGN;
//         data_used = 2;
//     }

//     return data_used;
// }

void __not_in_flash("pico_vgaboard_code")(pico_vgaboard_render_loop)(void)
{
    struct scanvideo_scanline_buffer *buffer;
    uint16_t scanline_number;
    uint32_t *scanline_colors;
    uint8_t *framebuffer_line_start;
    uint8_t bits, bits76, bits54, bits32, bits10, bits7654, bits3210;
    bool in_letterbox;
    uint16_t display_line;
    uint8_t *framebuffer;
#if USE_ONBOARD_LED == 1
    int scanvideo_scanline_counter = 0;
#endif
    if (pico_vgaboard->planes[0].initialize != NULL)
    {
        pico_vgaboard->planes[0].initialize(pico_vgaboard->planes[0].state);
    }
#if PICO_SCANVIDEO_PLANE_COUNT > 1
    if (pico_vgaboard->planes[1].initialize != NULL)
    {
        pico_vgaboard->planes[1].initialize(pico_vgaboard->planes[1].state);
    }
#endif
#if PICO_SCANVIDEO_PLANE_COUNT > 2
    if (pico_vgaboard->planes[2].initialize != NULL)
    {
        pico_vgaboard->planes[2].initialize(pico_vgaboard->planes[2].state);
    }
#endif
    // Let's go for the show!
    scanvideo_setup(pico_vgaboard->scanvideo_mode);
    scanvideo_timing_enable(true);
    pico_vgaboard->scanvideo_active = true;
    pico_vgaboard_flash_led_and_wait();
    while (true)
    {
        buffer = scanvideo_begin_scanline_generation(true);
        scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        if (pico_vgaboard->planes[0].render_scanline == NULL)
        {
            // No plane #1?
            buffer->data[0] = COMPOSABLE_RAW_1P | (0 << 16);
            buffer->data[1] = COMPOSABLE_EOL_SKIP_ALIGN;
            buffer->data_used = 2;
        }
        else
        {
            buffer->data_used = (pico_vgaboard->planes[0].render_scanline)(
                pico_vgaboard->planes[0].state,
                scanline_number, buffer->data, buffer->data_max);
        }
#if PICO_SCANVIDEO_PLANE_COUNT > 1
        if (pico_vgaboard->planes[1].render_scanline == NULL)
        {
            // No plane #2?
            buffer->data2[0] = COMPOSABLE_RAW_1P | (0 << 16);
            buffer->data2[1] = COMPOSABLE_EOL_SKIP_ALIGN;
            buffer->data2_used = 2;
        }
        else
        {
            buffer->data2_used = pico_vgaboard->planes[1].render_scanline(
                pico_vgaboard->planes[1].state, scanline_number, buffer->data2, buffer->data2_max);
        }
#endif
#if PICO_SCANVIDEO_PLANE_COUNT > 2
        if (pico_vgaboard->planes[2].render_scanline == NULL)
        {
            // No plane #3?
            buffer->data3[0] = COMPOSABLE_RAW_1P | (0 << 16);
            buffer->data3[1] = COMPOSABLE_EOL_SKIP_ALIGN;
            buffer->data3_used = 2;
        }
        else
        {
            buffer->data3_used = pico_vgaboard->planes[2].render_scanline(
                pico_vgaboard->planes[2].state, scanline_number, buffer->data3, buffer->data2_max);
        }
#endif
        scanvideo_end_scanline_generation(buffer);
#if USE_ONBOARD_LED == 1
        scanvideo_scanline_counter += 1;
        if (scanvideo_scanline_counter > 8 * pico_vgaboard->height)
        {
            // printf("LED!\n");
            scanvideo_scanline_counter = 0;
            pico_vgaboard_toggle_led();
        }
#endif
    } /* loop forever */
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
