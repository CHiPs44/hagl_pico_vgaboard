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

#include <assert.h>
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

void pico_vgaboard_framebuffer_init_palette_32(pico_vgaboard_framebuffer *fb)
{
    uint8_t count;
    switch (fb->flags.depth)
    {
    case 1: /* 1bpp: palette_32 is 2*2=4 entries of 32bits */
        count = 2;
        break;
    case 2: /* 2bpp: palette_32 is 4*4=16 entries of 32bits */
        count = 4;
        break;
    case 4: /* 4bpp: palette_32 is 16*16=256 entries of 32bits */
        count = 16;
        break;
    default:
        return;
    }
    /* i.e. all 2 pixels combinations */
    uint32_t *palette_32 = fb->palette_32;
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            *palette_32 = (fb->palette[i] << 16) | fb->palette[j];
            ++palette_32;
        }
    }
}

bool pico_vgaboard_framebuffer_set_border_color(pico_vgaboard_framebuffer *fb, int border, BGAR5515 color)
{
    switch (border)
    {
    case POS_TOP:
    case POS_LEFT:
    case POS_BOTTOM:
    case POS_RIGHT:
        fb->borders[border] = color;
        fb->borders_32[border] = (uint32_t)(color << 16) || (uint32_t)(color);
        return true;
    }
    return false;
}

bool pico_vgaboard_framebuffer_set_palette(pico_vgaboard_framebuffer *fb, const BGAR5515 *palette)
{
    if (fb->flags.depth > 8)
    {
        return false;
    }
    for (uint16_t i = 0; i < (1 << fb->flags.depth); i += 1)
    {
        fb->palette[i] = palette[i];
    }
    pico_vgaboard_framebuffer_init_palette_32(fb);
    return true;
}

void pico_vgaboard_framebuffer_init(
    pico_vgaboard_framebuffer *fb, int plane,
    uint8_t *fb0, uint8_t *fb1, bool double_buffer,
    uint8_t depth, const uint16_t *palette,
    uint16_t screen_width, uint16_t screen_height,
    uint16_t window_width, uint16_t window_height,
    BGAR5515 border_color)
{
    /* clang-format off */
#if PICO_VGABOARD_DEBUG
    printf("\t=> pico_vgaboard_framebuffer_init INIT\n");
    printf("Screen: %dx%d Window: %dx%d\n", screen_width, screen_height, window_width, window_height);
#endif
    /* Check if parameters are sane */
    assert(fb0  != NULL);
    assert(depth  == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16);
    assert((depth<=8 && palette != NULL)||(depth==16 && palette == NULL));
    assert(window_width  <= screen_width);
    assert(window_height <= screen_height);
    /* Base parameters */
    fb->flags.depth                 = depth;
    fb->colors                      = 1 << depth;
    fb->screen_width                = screen_width;
    fb->screen_height               = screen_height;
    /* Framebuffer(s) itself / themselves */
    fb->framebuffer_size            = pico_vgaboard_framebuffer_get_size(fb->flags.depth, fb->window_width, fb->window_height);
    fb->framebuffers[0]             = fb0;
    fb->framebuffers[1]             = fb1;
    fb->flags.double_buffer         = double_buffer;
    /* these won't change if double buffer is OFF */
    fb->flags.buffer_index          = 0;
    fb->flags.buffer_change         = false;
    fb->framebuffer                 = fb->framebuffers[0];
    /* Window / margins */
    fb->window_width                = window_width  > 0 && window_width  < fb->screen_width  ? window_width  : fb->screen_width;
    fb->window_height               = window_height > 0 && window_height < fb->screen_height ? window_height : fb->screen_height;
    fb->margins[POS_TOP   ]    = (screen_height - fb->window_height) / 2;
    fb->margins[POS_LEFT  ]    = (screen_width  - fb->window_width ) / 2;
    fb->margins[POS_BOTTOM]    = (screen_height - fb->window_height) / 2;
    fb->margins[POS_RIGHT ]    = (screen_width  - fb->window_width ) / 2;
    fb->flags.has_margins           = fb->margins[POS_TOP   ] > 0 || 
                                      fb->margins[POS_LEFT  ] > 0 || 
                                      fb->margins[POS_BOTTOM] > 0 || 
                                      fb->margins[POS_RIGHT ] > 0;
    /* Colors */
    pico_vgaboard_framebuffer_set_border_color(fb, POS_TOP   , border_color);
    pico_vgaboard_framebuffer_set_border_color(fb, POS_LEFT  , border_color);
    pico_vgaboard_framebuffer_set_border_color(fb, POS_BOTTOM, border_color);
    pico_vgaboard_framebuffer_set_border_color(fb, POS_RIGHT , border_color);
    pico_vgaboard_framebuffer_set_palette(fb, palette);
    pico_vgaboard_init_plane(
        plane, 
        PICO_VGABOARD_PLANE_FRAMEBUFFER, 
        0, 
        fb, 
        pico_vgaboard_framebuffer_init_plane, 
        pico_vgaboard_framebuffer_render_scanline);
#if PICO_VGABOARD_DEBUG
    printf("Screen: %dx%d Window: %dx%d\n", fb->screen_width, fb->screen_height, fb->window_width, fb->window_height);
    printf("\t=> pico_vgaboard_framebuffer_init DONE\n");
#endif
    /* clang-format on */
}

void pico_vgaboard_framebuffer_init_plane(void *plane_state)
{
#if !PICO_NO_HARDWARE && USE_INTERP == 1
#if PICO_VGABOARD_DEBUG
    printf("*** PICO_VGABOARD_FRAMEBUFFER_INIT_PLANE WITH INTERPOLATOR ***\n");
#endif
    pico_vgaboard_framebuffer *fb = plane_state;
    if (fb->flags.depth == 4)
    {
        // Configure interpolator lanes for 4bbp
        // TODO How to manage several framebuffers / interpolators?
        interp_config c = interp_default_config();
        interp_config_set_shift(&c, 22);
        interp_config_set_mask(&c, 2, 9);
        interp_set_config(interp0, 0, &c);
        interp_config_set_shift(&c, 14);
        interp_config_set_cross_input(&c, true);
        interp_set_config(interp0, 1, &c);
        interp_set_base(interp0, 0, (uintptr_t)(fb->palette_32));
        interp_set_base(interp0, 1, (uintptr_t)(fb->palette_32));
    }
#else
#if PICO_VGABOARD_DEBUG
    printf("*** PICO_VGABOARD_FRAMEBUFFER_INIT_PLANE ***\n");
#endif
#endif
}

bool pico_vgaboard_framebuffer_flip(pico_vgaboard_framebuffer *fb)
{
    if (!fb->flags.double_buffer)
        return false;
    // uint64_t start = time_us_64();
    fb->flags.buffer_change = true;
    while (fb->flags.buffer_change)
    {
#if !PICO_NO_HARDWARE
        __wfe();
#else
        tight_loop_contents();
#endif
    }
    // uint64_t finish = time_us_64();
    // printf("FLIP! %lld => %d\n", finish - start, fb->flags.buffer_index);
    return true;
}

uint16_t __not_in_flash("pico_vgaboard_code")(pico_vgaboard_framebuffer_render_scanline)(
    void *plane_state,
    uint16_t scanline_number,
    uint32_t *data,
    uint16_t data_max)
{
    static uint32_t counter = 0;
    counter += 1;
    int debug_left = 0, debug_image = 0, debug_right = 0;

    pico_vgaboard_framebuffer *fb = plane_state;
    uint32_t *scanline_colors;
    uint8_t *framebuffer_line_start;
    uint8_t bits;
    bool in_window;
    uint16_t window_line;
    uint8_t *framebuffer;
    // At end of screen?
    if (scanline_number >= fb->screen_height - 1)
    {
        // flip/swap of framebuffer required?
        if (fb->flags.double_buffer && fb->flags.buffer_change)
        {
            // yes => acknowledge
            fb->flags.buffer_change = false;
            fb->flags.buffer_index = 1 - fb->flags.buffer_index;
            fb->framebuffer = fb->framebuffers[fb->flags.buffer_index];
        }
    }
    if (fb->flags.double_buffer)
    {
        // use *other* framebuffer for rendering, not the one we are drawing onto
        framebuffer = (uint8_t *)(fb->framebuffers[1 - fb->flags.buffer_index]);
    }
    else
    {
        // always use same & only framebuffer
        framebuffer = (uint8_t *)(fb->framebuffer);
    }
    scanline_colors = data;
    in_window = true;
    window_line = scanline_number;
    if (fb->flags.has_margins)
    {
        if ((scanline_number < fb->margins[POS_TOP]) ||
            (scanline_number > fb->window_height + fb->margins[POS_TOP] - 1))
        {
            /* in top margin or bottom margin => 1 line of pixels with corresponding border color */
            in_window = false;
            fb->borders_32[POS_TOP] = (uint32_t)(fb->borders[POS_TOP]) << 16 | (uint32_t)(fb->borders[POS_TOP]);
            fb->borders_32[POS_BOTTOM] = (uint32_t)(fb->borders[POS_BOTTOM]) << 16 | (uint32_t)(fb->borders[POS_BOTTOM]);
            uint32_t border_color_32 = scanline_number < fb->margins[POS_TOP]
                                           ? fb->borders_32[POS_TOP]
                                           : fb->borders_32[POS_BOTTOM];
            for (uint16_t i = 0; i < fb->screen_width / 2; i++)
            {
                *++scanline_colors = border_color_32;
            }
            ++scanline_colors;
        }
        else
        {
            window_line = scanline_number - fb->margins[POS_TOP];
        }
    }
    if (in_window)
    {
        // left margin
        if (fb->margins[POS_LEFT] > 0)
        {
            fb->borders_32[POS_LEFT] = (uint32_t)(fb->borders[POS_LEFT]) << 16 | (uint32_t)(fb->borders[POS_LEFT]);
            for (uint16_t i = 0; i < fb->margins[POS_LEFT] / 2; ++i)
            {
                ++scanline_colors;
                *scanline_colors = fb->borders_32[POS_LEFT];
                debug_left++;
            }
        }
        // image from framebuffer
        switch (fb->flags.depth)
        {
        case 1: // 1bpp, 8 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->window_width / 8) * window_line]);
            for (uint16_t byte = 0; byte < fb->window_width / 8; ++byte)
            {
                // 76543210 => 8 pixels to 8 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                ++scanline_colors;
                *scanline_colors = fb->palette_32[(bits & 0b11000000) >> 6];
                debug_image++;
                ++scanline_colors;
                *scanline_colors = fb->palette_32[(bits & 0b00110000) >> 4];
                debug_image++;
                ++scanline_colors;
                *scanline_colors = fb->palette_32[(bits & 0b00001100) >> 2];
                debug_image++;
                ++scanline_colors;
                *scanline_colors = fb->palette_32[(bits & 0b00000011) >> 0];
                debug_image++;
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 2: // 2bpp, 4 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->window_width / 4) * window_line]);
            for (uint16_t x = 0; x < fb->window_width / 4; ++x)
            {
                // 76543210 => 4 pixels to 4 x 16 bits => 4 x 32 bits in buffer
                bits = *framebuffer_line_start;
                *++scanline_colors = fb->palette_32[(bits & 0b1111000) >> 4];
                debug_image++;
                *++scanline_colors = fb->palette_32[(bits & 0b0001111) >> 0];
                debug_image++;
                // Next byte / 4 pixels
                ++framebuffer_line_start;
            }
            ++scanline_colors;
            break;
        case 4: // 4bpp, 2 pixels per byte
            framebuffer_line_start = &(framebuffer[(fb->window_width / 2) * window_line]);
#if !PICO_NO_HARDWARE && USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, framebuffer_line_start, fb->window_width / 2);
            scanline_colors += fb->window_width / 2;
            debug_image += fb->window_width / 2;
#else
            for (uint16_t x = 0; x < fb->window_width / 2; ++x)
            {
                bits = *framebuffer_line_start;
                ++scanline_colors;
                *scanline_colors = fb->palette_32[bits];
                debug_image++;
                ++framebuffer_line_start;
            }
            ++scanline_colors;
#endif
            break;
        case 8: // 8bpp, 1 pixel per byte
            framebuffer_line_start = &(framebuffer[(fb->window_width / 1) * window_line]);
            // append 2 16 bits pixels in the scanline, hence screen_width / 2
            uint32_t color1, color2;
            for (uint16_t x = 0; x < fb->window_width / 2; ++x)
            {
                color1 = fb->palette[*framebuffer_line_start++];
                color2 = fb->palette[*framebuffer_line_start++];
                ++scanline_colors;
                *scanline_colors = (color2 << 16) | color1;
                debug_image++;
            }
            ++scanline_colors;
            break;
        case 16: // 16bpp, 1 pixel per word / 2 bytes per pixel
            framebuffer_line_start = &(framebuffer[(fb->window_width * 2) * window_line]);
            for (uint16_t x = 0; x < fb->window_width; ++x)
            {
                ++scanline_colors;
                // get 4 bytes at a time
                *scanline_colors = *((uint32_t *)(framebuffer_line_start));
                framebuffer_line_start += 4;
                debug_image++;
            }
            ++scanline_colors;
            break;
        }
        // right margin
        if (fb->margins[POS_LEFT] > 0)
        {
            fb->borders_32[POS_RIGHT] = (uint32_t)(fb->borders[POS_RIGHT]) << 16 | (uint32_t)(fb->borders[POS_RIGHT]);
            for (uint16_t i = 0; i < fb->margins[POS_LEFT] / 2; ++i)
            {
                // we already point to a free location
                // ++scanline_colors;
                *scanline_colors = fb->borders_32[POS_RIGHT];
                ++scanline_colors;
                debug_right++;
            }
            // we already point to a free location
            // ++scanline_colors;
        }
    }
    // scanline end
    *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
    scanline_colors = data;
    scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
    scanline_colors[1] = (scanline_colors[1] & 0xffff0000) | (fb->screen_width - 2);

    // data_used
    uint16_t data_used = (fb->screen_width + 4) / 2; // 2 16 bits pixels in each 32 bits word

    if (counter > 10000 && fb->debug_message[0] == '\0')
    {
        snprintf(fb->debug_message, CONSOLE_DEBUG_MSG_MAX_LEN-1, "%05d [FB] data_used: %d/%d left=%d, image=%d, right=%d, total=%d",
                 scanline_number, data_used, data_max, debug_left, debug_image, debug_right, debug_left + debug_image + debug_right);
        counter = 0;
    }
    return data_used;
}

bool __not_in_flash("pico_vgaboard_code")(pico_vgaboard_framebuffer_put_pixel)(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y, BGAR5515 pixel)
{
    volatile uint8_t *byte;
    int32_t offset;
    uint8_t bit, bits, mask;

    // printf("START: put_pixel(%d, %d, %d)\n", x, y, pixel);

    switch (fb->flags.depth)
    {
    case 1: // 8 pixels per byte, monochrome
        offset = (fb->window_width / 8) * y + x / 8;
        if (offset > (fb->window_width * fb->window_height / 8) - 1)
            return false;
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
        break;
    case 2: // 4 pixels per byte, 4 colors
        offset = (fb->window_width / 4) * y + x / 4;
        if (offset > (fb->window_width * fb->window_height / 4) - 1)
            return false;
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
        break;
    case 4: // 2 pixels per byte, 16 colors
        offset = (fb->window_width / 2) * y + x / 2;
        if (offset > (fb->window_width * fb->window_height / 2) - 1)
            return false;
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
        break;
    case 8: // 1 pixel per byte, 256 colors
        offset = fb->window_width * y + x;
        if (offset > (fb->window_width * fb->window_height) - 1)
            return false;
        fb->framebuffer[offset] = pixel;
        break;
    case 16: // 1 pixel per word <=> 2 bytes per pixel, 32768 colors
        offset = (fb->window_width * y + x) * 2;
        if (offset > (fb->window_width * fb->window_height * 2) - 1)
            return false;
        fb->framebuffer[offset + 0] = pixel >> 8;
        fb->framebuffer[offset + 1] = pixel & 0xff;
        break;
    default:
        return false;
    }
    // printf("FINISH: put_pixel(%d, %d, %d)\n", x, y, pixel);
    return false;
}

BGAR5515 __not_in_flash("pico_vgaboard_code")(pico_vgaboard_framebuffer_get_pixel_index)(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y)
{
    BGAR5515 pixel = 0;
    int32_t offset;
    uint8_t bit, bits, mask;

    switch (fb->flags.depth)
    {
    case 1: // 8 pixels per byte
        offset = (fb->window_width / 8) * y + x / 8;
        bit = 7 - (x % 8);
        mask = 1 << bit;
        pixel = fb->framebuffer[offset] & mask ? 1 : 0;
        break;
    case 2: // 4 pixels per byte
        offset = (fb->window_width / 4) * y + x / 4;
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
        break;
    case 4: // 2 pixels per byte
        offset = (fb->window_width / 2) * y + x / 2;
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
        break;
    case 8: // 1 pixel per byte
        offset = fb->window_width * y + x;
        pixel = fb->framebuffer[offset];
        break;
    case 16: // 1 pixel per word => 2 bytes per pixel
        offset = (fb->window_width * y + x) * 2;
        pixel = fb->framebuffer[offset + 0] << 8 | fb->framebuffer[offset + 1];
        break;
    }
    return pixel;
}

BGAR5515 pico_vgaboard_framebuffer_get_palette_color(pico_vgaboard_framebuffer *fb, uint8_t index)
{
    switch (fb->flags.depth)
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
BGAR5515 pico_vgaboard_framebuffer_get_pixel_color(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y)
{
    uint16_t pixel = pico_vgaboard_framebuffer_get_pixel_index(fb, x, y);
    if (fb->flags.depth == 16)
    {
        return pixel;
    }
    return pico_vgaboard_framebuffer_get_palette_color(fb, pixel & 0xff);
}

uint32_t pico_vgaboard_framebuffer_get_size(uint8_t depth, uint16_t screen_width, uint16_t screen_height)
{
    uint32_t size = screen_width * screen_height;
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

void pico_vgaboard_framebuffer_dump(pico_vgaboard_framebuffer *fb)
{
    printf("*** FRAMEBUFFER@%p\n", fb);
    printf("  Screen: %dx%d\n  Window %dx%d@%d/%d\n  Framebuffer: %p %d\n  Palette: %p\n",
           fb->screen_width, fb->screen_height,
           fb->window_width, fb->window_height,
           fb->flags.depth, fb->colors,
           fb->framebuffer, fb->framebuffer_size,
           fb->palette);
    printf("    ");
    for (uint8_t color = 0; color < 16; color += 1)
    {
        printf("%02d: %02x%02x%02x ",
               color,
               PICO_SCANVIDEO_R5_FROM_PIXEL(fb->palette[color]) << 3,
               PICO_SCANVIDEO_G5_FROM_PIXEL(fb->palette[color]) << 3,
               PICO_SCANVIDEO_B5_FROM_PIXEL(fb->palette[color]) << 3);
        if ((color + 1) % 4 == 0 && color < 15)
            printf("\n    ");
    }
    printf("\n");
}

// EOF
