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

#include "pico-vgaboard-framebuffer.h"

#if !PICO_NO_HARDWARE
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

void pico_vgaboard_framebuffer_start_double_palette_1bpp(pico_vgaboard_framebuffer_t *fb)
{
    if (fb->depth != 1)
    {
        return;
    }
    /* double_palette_1bpp is 4 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_1 = fb->double_palette_1bpp;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            *double_palette_1 = (fb->palette[j] << 16) | fb->palette[i];
            ++double_palette_1;
        }
    }
}

void pico_vgaboard_framebuffer_start_double_palette_2bpp(pico_vgaboard_framebuffer_t *fb)
{
    if (fb->depth != 2)
    {
        return;
    }
    /* double_palette_2bpp is 16 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_2 = fb->double_palette_2bpp;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            *double_palette_2 = (fb->palette[j] << 16) | fb->palette[i];
            ++double_palette_2;
        }
    }
}

void pico_vgaboard_framebuffer_start_double_palette_4bpp(pico_vgaboard_framebuffer_t *fb)
{
    if (fb->depth != 4)
    {
        return;
    }
    /* double_palette_4bpp is 256 entries of 32bits */
    /* i.e. all 2 pixels combinations */
    uint32_t *double_palette_4 = fb->double_palette_4bpp;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            *double_palette_4 = (fb->palette[i] << 16) | fb->palette[j];
            ++double_palette_4;
        }
    }
}

void pico_vgaboard_set_palette(pico_vgaboard_framebuffer_t *fb, const BGAR5515 *palette)
{
    if (fb->depth > 8)
    {
        return;
    }
    // Copy palette to PICO_VGABOARD_DATA
    for (uint16_t i = 0; i < fb->colors; i += 1)
    {
        fb->palette[i] = palette[i];
    }
    // Setup double palettes
    pico_vgaboard_framebuffer_start_double_palette_1bpp(fb);
    pico_vgaboard_framebuffer_start_double_palette_2bpp(fb);
    pico_vgaboard_framebuffer_start_double_palette_4bpp(fb);
}

void pico_vgaboard_framebuffer_init(pico_vgaboard_framebuffer_t *fb, bool double_buffer, uint8_t *vram, uint8_t depth, uint16_t *palette, uint16_t width, uint16_t height, uint16_t display_width, uint16_t display_height, BGAR5515 border_color)
{
    /* clang-format off */
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_start INIT\n");
#endif
    fb->depth                = depth;
    fb->colors               = 1 << depth;
    fb->width                = width;
    fb->height               = height;
    pico_vgaboard_framebuffer_set_palette(fb, palette);
    fb->display_width        = display_width  > 0 && display_width  < fb->width  ? display_width  : fb->width ;
    fb->display_height       = display_height > 0 && display_height < fb->height ? display_height : fb->height;
    fb->horizontal_margin    = (width  - fb->display_width ) / 2;
    fb->vertical_margin      = (height - fb->display_height) / 2;
    fb->has_margins          = fb->horizontal_margin > 0 || fb->vertical_margin > 0;
    fb->border_color_top     = border_color;
    fb->border_color_left    = border_color;
    fb->border_color_bottom  = border_color;
    fb->border_color_right   = border_color;
    fb->vram_size            = PICO_VGABOARD_VRAM_SIZE;
    fb->vram                 = vram;
    fb->framebuffer_size     = pico_vgaboard_framebuffer_get_size(fb->depth, fb->display_width, fb->display_height);
    fb->double_buffer        = double_buffer;
    if (fb->double_buffer)
    {
        if (fb->framebuffer_size * 2 > fb->vram_size)
        {
#if PICO_VGABOARD_DEBUG
            printf(
                "\t=> pico_vgaboard_framebuffer_init /!\\ FRAMEBUFFER_SIZE * 2 (%d) > VRAM_SIZE (%d) /!\\\n", 
                fb->framebuffer_size * 2, fb->vram_size
            );
#endif
            fb->framebuffer_size = fb->vram_size / 2;
        }
        // For now, always have framebuffer0 at offset 0 of vram and framebuffer1 after
        fb->framebuffers[0]    = fb->vram;
        fb->framebuffers[1]    = fb->vram + fb->framebuffer_size;
        fb->framebuffer_index  = 0;
        fb->framebuffer_change = false;
        fb->framebuffer        = fb->framebuffers[0];
    }
    else
    {
        if (fb->framebuffer_size > fb->vram_size)
        {
#if PICO_VGABOARD_DEBUG
            printf(
                "\t=> pico_vgaboard_framebuffer_init /!\\ FRAMEBUFFER_SIZE (%d) > VRAM_SIZE (%d) /!\\\n", 
                fb->framebuffer_size, fb->vram_size
            );
#endif
        fb->framebuffer_size = fb->vram_size;
        }
        // For now, always have framebuffer at offset 0 of vram
        fb->framebuffer      = fb->vram;
    }
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_framebuffer_init DONE\n");
#endif
    /* clang-format on */
}

void pico_vgaboard_framebuffer_flip(pico_vgaboard_framebuffer_t *fb)
{
    if (!fb->double_buffer)
        return;
    // uint64_t start = time_us_64();
    fb->framebuffer_change = true;
    while (fb->framebuffer_change)
    {
#if !PICO_NO_HARDWARE
        __wfe();
#else
        tight_loop_contents();
#endif
    }
    // uint64_t finish = time_us_64();
    // printf("FLIP! %lld => %d\n", finish - start, fb->framebuffer_index);
}

uint16_t __not_in_flash("pico_vgaboard_code")(pico_vgaboard_framebuffer_render_scanline)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max)
{
    pico_vgaboard_framebuffer_t *fb = plane_state;
    uint32_t *scanline_colors;
    uint8_t *framebuffer_line_start;
    uint8_t bits;
    bool in_letterbox;
    uint16_t display_line;
    uint8_t *framebuffer;
#if !PICO_NO_HARDWARE && USE_INTERP == 1
    if (fb->depth == 4)
    {
        // Configure interpolator lanes for 4bbp
        interp_config c = interp_default_config();
        interp_config_set_shift(&c, 22);
        interp_config_set_mask(&c, 2, 9);
        interp_set_config(interp0, 0, &c);
        interp_config_set_shift(&c, 14);
        interp_config_set_cross_input(&c, true);
        interp_set_config(interp0, 1, &c);
        interp_set_base(interp0, 0, (uintptr_t)double_palette_4bpp);
        interp_set_base(interp0, 1, (uintptr_t)double_palette_4bpp);
    }
#endif
    /* clang-format off */
        fb->border_color_top_32    = (uint32_t)(fb->border_color_top   ) << 16 | (uint32_t)(fb->border_color_top   );
        fb->border_color_left_32   = (uint32_t)(fb->border_color_left  ) << 16 | (uint32_t)(fb->border_color_left  );
        fb->border_color_bottom_32 = (uint32_t)(fb->border_color_bottom) << 16 | (uint32_t)(fb->border_color_bottom);
        fb->border_color_right_32  = (uint32_t)(fb->border_color_right ) << 16 | (uint32_t)(fb->border_color_right );
    /* clang-format on */
    buffer = scanvideo_begin_scanline_generation(true);
    if (scanline_number >= fb->height - 1)
    {
        pico_vgaboard_frame_counter += 1;
        if (fb->double_buffer)
            // flip/swap of framebuffer required?
            if (fb->framebuffer_change)
            {
                // yes => acknowledge
                fb->framebuffer_change = false;
                fb->framebuffer_index = 1 - fb->framebuffer_index;
                fb->framebuffer = fb->framebuffers[fb->framebuffer_index];
                fb->framebuffer_flips += 1;
            }
    }
    if (fb->double_buffer)
    {
        // use *other* framebuffer for rendering, not the one we are drawing onto
        framebuffer = (uint8_t *)(fb->framebuffers[1 - fb->framebuffer_index]);
    }
    else
    {
        // always use same & only framebuffer
        framebuffer = (uint8_t *)(fb->framebuffer);
    }
    scanline_colors = data;
    in_letterbox = true;
    display_line = scanline_number;
    if (fb->has_margins)
    {
        if ((scanline_number < fb->vertical_margin) ||
            (scanline_number > fb->display_height + fb->vertical_margin - 1))
        {
            /* in top margin or bottom margin => 1 line of pixels with corresponding border color */
            in_letterbox = false;
            uint32_t border_color_32 = scanline_number < fb->vertical_margin
                                           ? fb->border_color_top_32
                                           : fb->border_color_bottom_32;
            for (uint16_t i = 0; i < fb->width / 2; i++)
            {
                *++scanline_colors = border_color_32;
            }
            ++scanline_colors;
        }
        else
        {
            display_line = scanline_number - fb->vertical_margin;
        }
    }
    if (in_letterbox)
    {
        // left margin
        if (fb->horizontal_margin > 0)
        {
            for (uint16_t i = 0; i < fb->horizontal_margin / 2; ++i)
            {
                ++scanline_colors;
                *scanline_colors = fb->border_color_left_32;
            }
        }
        // image from framebuffer
        switch (fb->depth)
        {
        case 1: // 1bpp, 8 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->display_width / 8) * display_line]);
            for (uint16_t byte = 0; byte < fb->display_width / 8; ++byte)
            {
                // 76543210 => 8 pixels to 8 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                ++scanline_colors;
                *scanline_colors = double_palette_1bpp[(bits & 0b11000000) >> 6];
                ++scanline_colors;
                *scanline_colors = double_palette_1bpp[(bits & 0b00110000) >> 4];
                ++scanline_colors;
                *scanline_colors = double_palette_1bpp[(bits & 0b00001100) >> 2];
                ++scanline_colors;
                *scanline_colors = double_palette_1bpp[(bits & 0b00000011) >> 0];
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 2: // 2bpp, 4 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->display_width / 4) * display_line]);
            for (uint16_t x = 0; x < fb->display_width / 4; ++x)
            {
                // 76543210 => 4 pixels to 4 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                *++scanline_colors = double_palette_2bpp[(bits & 0b1111000) >> 4];
                *++scanline_colors = double_palette_2bpp[(bits & 0b0001111) >> 0];
                // Next byte / 4 pixels
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 4: // 4bpp, 2 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->display_width / 2) * display_line]);
#if !PICO_NO_HARDWARE && USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, framebuffer_line_start, fb->display_width / 2);
            scanline_colors += fb->display_width / 2;
#else
            for (uint16_t x = 0; x < fb->display_width / 2; ++x)
            {
                bits = *framebuffer_line_start;
                ++scanline_colors;
                *scanline_colors = double_palette_4bpp[bits];
                ++framebuffer_line_start;
            }
            ++scanline_colors;
#endif
            break;
        case 8: // 8bpp, 1 pixel per byte
            framebuffer_line_start = &(framebuffer[(fb->display_width / 1) * display_line]);
            // append 2 16 bits pixels in the scanline, hence width / 2
            uint32_t color1, color2;
            for (uint16_t x = 0; x < fb->display_width / 2; ++x)
            {
                color1 = fb->palette[*framebuffer_line_start++];
                color2 = fb->palette[*framebuffer_line_start++];
                ++scanline_colors;
                *scanline_colors = (color2 << 16) | color1;
            }
            ++scanline_colors;
            break;
        case 16: // 16bpp, 1 pixel per word / 2 bytes per pixel
            framebuffer_line_start = &(framebuffer[(fb->display_width * 2) * display_line]);
            for (uint16_t x = 0; x < fb->display_width; ++x)
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
        if (fb->horizontal_margin > 0)
        {
            for (uint16_t i = 0; i < fb->horizontal_margin / 2; ++i)
            {
                // we already point to a free location
                // ++scanline_colors;
                *scanline_colors = fb->border_color_right_32;
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
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (fb->width - 2);
    // data_used
    return (fb->width + 4) / 2; // 2 16 bits pixels in each 32 bits word
}

void pico_vgaboard_put_pixel(pico_vgaboard_framebuffer_t *fb, uint16_t x, uint16_t y, BGAR5515 pixel)
{
    volatile uint8_t *byte;
    int32_t offset;
    uint8_t bit, bits, mask;

    switch (fb->depth)
    {
    case 1: // 8 pixels per byte, monochrome
        offset = (fb->display_width / 8) * y + x / 8;
        if (offset < fb->display_width * fb->display_height / 8)
        {
            byte = &fb->framebuffer[offset];
            bit = 7 - (x % 8);
            mask = 1 << bit;
            if (pixel & 1)
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
        offset = (fb->display_width / 4) * y + x / 4;
        if (offset < fb->display_width * fb->display_height / 4)
        {
            byte = &fb->framebuffer[offset];
            switch (x % 4)
            {
            case 0: /* bits 7-6 */
                bits = (pixel & 0b11) << 6;
                mask = 0b00111111;
                break;
            case 1: /* bits 5-4 */
                bits = (pixel & 0b11) << 4;
                mask = 0b11001111;
                break;
            case 2: /* bits 3-2 */
                bits = (pixel & 0b11) << 2;
                mask = 0b11110011;
                break;
            case 3: /* bits 1-0 */
                bits = (pixel & 0b11) << 0;
                mask = 0b11111100;
                break;
            }
            *byte &= mask;
            *byte |= bits;
        }
        break;
    case 4: // 2 pixels per byte, 16 colors
        offset = (fb->display_width / 2) * y + x / 2;
        if (offset < fb->display_width * fb->display_height / 2)
        {
            byte = &fb->framebuffer[offset];
            if (x & 1)
            {
                // left / high nibble
                *byte = ((pixel & 0x0f) << 4) | (*byte & 0x0f);
            }
            else
            {
                // right / low nibble
                *byte = (*byte & 0xf0) | (pixel & 0x0f);
            }
        }
        break;
    case 8: // 1 pixel per byte, 256 colors
        offset = fb->display_width * y + x;
        if (offset < fb->display_width * fb->display_height)
        {
            fb->framebuffer[offset] = pixel;
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel, 32768 colors
        offset = (fb->display_width * y + x) * 2;
        if (offset < fb->display_width * fb->display_height * 2)
        {
            fb->framebuffer[offset + 0] = pixel >> 8;
            fb->framebuffer[offset + 1] = pixel & 0xff;
        }
        break;
    default:
        break;
    }
}

BGAR5515 pico_vgaboard_get_pixel_index(pico_vgaboard_framebuffer_t *fb, uint16_t x, uint16_t y)
{
    BGAR5515 pixel = 0;
    int32_t offset;
    uint8_t bit, bits, mask;

    switch (fb->depth)
    {
    case 1: // 8 pixels per byte
        offset = (fb->display_width / 8) * y + x / 8;
        if (offset < fb->display_width * fb->display_height / 8)
        {
            bit = 7 - (x % 8);
            mask = 1 << bit;
            pixel = fb->framebuffer[offset] & mask ? 1 : 0;
        }
        break;
    case 2: // 4 pixels per byte
        offset = (fb->display_width / 4) * y + x / 4;
        if (offset < fb->display_width * fb->display_height / 4)
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
            pixel = (fb->framebuffer[offset] & mask) >> bits;
        }
        break;
    case 4: // 2 pixels per byte
        offset = (fb->display_width / 2) * y + x / 2;
        if (offset < fb->display_width * fb->display_height / 2)
        {
            if (x & 1)
            {
                // odd pixel => right nibble (LSB)
                pixel = fb->framebuffer[offset] & 0x0f;
            }
            else
            {
                // even pixel => left nibble (MSB)
                pixel = fb->framebuffer[offset] >> 4;
            }
        }
        break;
    case 8: // 1 pixel per byte
        offset = fb->display_width * y + x;
        if (offset < fb->display_width * fb->display_height)
        {
            pixel = fb->framebuffer[offset];
        }
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel
        offset = (fb->display_width * y + x) * 2;
        if (offset < fb->display_width * fb->display_height * 2)
        {
            pixel =
                fb->framebuffer[offset + 0] << 8 |
                fb->framebuffer[offset + 1];
        }
        break;
    }
    return pixel;
}

BGAR5515 pico_vgaboard_get_palette_color(pico_vgaboard_framebuffer_t *fb, uint8_t index)
{
    switch (fb->depth)
    {
    case 1: // 0-1
        return fb->palette[index & 0b00000001];
    case 2: // 0-3
        return fb->palette[index & 0b00000011];
    case 4: // 0-15
        return fb->palette[index & 0b00001111];
    case 8: // 0-255
        return fb->palette[index];
    }
    return 0;
}

/**
 * @brief Retrieve BGAR5515 color for pixel at (x, y) coordinates
 */
BGAR5515 pico_vgaboard_get_pixel_color(pico_vgaboard_framebuffer_t *fb, uint16_t x, uint16_t y)
{
    uint16_t pixel = pico_vgaboard_get_pixel_index(x, y);
    if (fb->depth == 16)
    {
        return pixel;
    }
    return pico_vgaboard_get_palette_color(fb, pixel & 0xff);
}

size_t pico_vgaboard_framebuffer_get_size(uint8_t depth, uint16_t width, uint16_t height)
{
    size_t size = width * height;
    switch (depth)
    {
    case 1:
        return size / 8;
    case 2:
        return size / 4;
    case 4:
        return size / 2;
    case 8:
        return size / 1;
    case 16:
        return size * 2;
    default:
        return 0;
    }
}

// EOF
