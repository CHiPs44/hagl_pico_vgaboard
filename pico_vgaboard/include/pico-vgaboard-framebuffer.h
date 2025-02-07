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

#ifndef _PICO_VGABOARD_FRAMEBUFFER_H
#define _PICO_VGABOARD_FRAMEBUFFER_H

#include "pico-vgaboard.h"
#include "pico/scanvideo.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Use interpolator in 16 colors mode? */
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

    #define CONSOLE_ERROR_MSG_MAX_LEN (128U)
    #define CONSOLE_DEBUG_MSG_MAX_LEN (128U)

    /** @brief Framebuffer flags */
    typedef struct _pico_vgaboard_framebuffer_flags
    {
        uint8_t depth : 4;        /** @brief 1, 2, 4, 8 or 16 bits per pixel */
        bool double_buffer : 1;   /** @brief false if single buffer, true if double buffer */
        bool buffer_change : 1;   /** @brief true to change framebuffer at next vertical sync */
        uint8_t buffer_index : 1; /** @brief 0 or 1 */
        bool has_margins : 1;     /** @brief true if window screen_width/screen_height is less than screen screen_width/screen_height */
        uint8_t reserved : 8;     /** @brief 16 bits total */
    } pico_vgaboard_framebuffer_flags;

    /** @brief Framebuffer state */
    typedef struct _pico_vgaboard_framebuffer
    {
        pico_vgaboard_framebuffer_flags flags; /** @brief    2 flags */
        uint32_t colors;                       /** @brief    4 2, 4, 16, 256 or 65536 (which does not fit in an uint16_t) */
        uint16_t screen_width;                 /** @brief    2 Same as vgaboard->model */
        uint16_t screen_height;                /** @brief    2 Same as vgaboard->model */
        uint32_t framebuffer_size;             /** @brief    4 Bytes, computed from window size */
        uint8_t *framebuffers[2];              /** @brief    8 Each one must be 32 bits aligned */
        uint8_t *framebuffer;                  /** @brief    4 Currently displayed framebuffer */
        uint16_t window_width;                 /** @brief    2 Display width , 0 for screen width */
        uint16_t window_height;                /** @brief    2 Display height, 0 for screen_height */
        uint16_t margins[4];                   /** @brief    2 EVEN number of pixels for margins (centered by default) */
        BGAR5515 borders[4];                   /** @brief    8 Margin colors (16 bits values, not palette indexes) */
        uint32_t borders_32[4];                /** @brief   16 Pre-calculated double pixels for margins (should be replaced with COMPOSABLE_RAW_RUN at least for top & bottom)  */
        BGAR5515 palette[256];                 /** @brief  512 256 BGAR5515 values, unused for 16 bits depth / 65536 colors */
        uint32_t palette_32[256];              /** @brief 1024 Pre-calculated pixel combinations for 1/2/4 bit depths */
        char error_message[CONSOLE_ERROR_MSG_MAX_LEN]; /** @brief  128 Error message */
        char debug_message[CONSOLE_DEBUG_MSG_MAX_LEN]; /** @brief  128 Debug message */
    } pico_vgaboard_framebuffer;               /* Total: should be less than 2048 bytes */

    /** @brief Initialize framebuffer plane */
    void pico_vgaboard_framebuffer_init(
        pico_vgaboard_framebuffer *fb, int plane,
        uint8_t *fb0, uint8_t *fb1, bool double_buffer,
        uint8_t depth, const uint16_t *palette,
        uint16_t screen_width, uint16_t screen_height,
        uint16_t window_width, uint16_t window_height,
        BGAR5515 border_color);

    /** @brief Initialize framebuffer plane (on core1) */
    void pico_vgaboard_framebuffer_init_plane(void *plane_state);

    /** @brief Render framebuffer plane scanline (on core1) */
    uint16_t pico_vgaboard_framebuffer_render_scanline(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max);

    /** @brief Flips framebuffer from 0 to 1 or 1 to 0 at next VSYNC period */
    bool pico_vgaboard_framebuffer_flip(pico_vgaboard_framebuffer *fb);

    /** @brief Change TOP/LEFT/BOTTOM/RIGHT border color */
    bool pico_vgaboard_framebuffer_set_border_color(pico_vgaboard_framebuffer *fb, int border, BGAR5515 color);

    /** @brief Copy new palette to current palette */
    bool pico_vgaboard_framebuffer_set_palette(pico_vgaboard_framebuffer *fb, const BGAR5515 *palette);

    /** @brief Setup double palette for 1/2/4bpp */
    void pico_vgaboard_framebuffer_init_palette_32(pico_vgaboard_framebuffer *fb);

    /** @brief Put pixel at (x, y) with color index in current palette or true color */
    bool pico_vgaboard_framebuffer_put_pixel(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y, BGAR5515 index_or_color);

    /** @brief Get RGB color from index in current palette, returns 0 in 16bpp depth */
    BGAR5515 pico_vgaboard_framebuffer_get_palette_color(pico_vgaboard_framebuffer *fb, uint8_t index);

    /** @brief Get color index or RGB color for given pixel */
    BGAR5515 pico_vgaboard_framebuffer_get_pixel_index(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y);

    /** @brief Get RGB color for given pixel */
    BGAR5515 pico_vgaboard_framebuffer_get_pixel_color(pico_vgaboard_framebuffer *fb, uint16_t x, uint16_t y);

    /** @brief Compute framebufer size from depth, screen_width & screen_height */
    uint32_t pico_vgaboard_framebuffer_get_size(uint8_t depth, uint16_t screen_width, uint16_t screen_height);

    /** @brief Dump framebuffer data */
    void pico_vgaboard_framebuffer_dump(pico_vgaboard_framebuffer *fb);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_FRAMEBUFFER_H */
