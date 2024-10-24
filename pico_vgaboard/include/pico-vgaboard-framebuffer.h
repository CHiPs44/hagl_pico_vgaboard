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

#include "pico/scanvideo.h"
#include "pico-vgaboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Use interpolator in 16 colors mode? */
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

/** @brief VGA board framebuffer */
typedef struct _pico_vgaboard_framebuffer
{
    /* Base parameters                                                                                                      */
    uint16_t                height;             /* same as vgaboard->model                                                  */
    uint16_t                width;              /* same as vgaboard->model                                                  */
    uint8_t                 depth;              /* 1, 2,  4,    8 or    16 bits per pixel                                   */
    uint32_t                colors;             /* 2, 4, 16,  256 or 65536 (which does not fit in an uint16_t)              */
    BGAR5515                palette[256];       /* 256 BGAR5515 values, unused for 16 bits depth / 65536 colors             */
    /* VRAM / framebuffer                                                                                                   */
    uint32_t                vram_size;          /* in bytes, should be less than PICO_VGABOARD_VRAM_SIZE                    */
    uint8_t                *vram;               /* global static video RAM since mallocing framebuffer doesn't work for now */
    uint32_t                framebuffer_size;   /* in bytes, computed from display size                                     */
    bool                    double_buffer;      /* false if single buffer, true if double buffer                            */
    uint8_t                *framebuffers[2];    /* 0=vram, 1=vram + something, must be 32 bits aligned                      */
    volatile uint8_t        framebuffer_index;  /* 0 or 1                                                                   */
    volatile bool           framebuffer_change; /* true to change at next vertical sync                                     */
    volatile uint8_t       *framebuffer;        /* videoram + something, must be 32 bits aligned                            */
    uint64_t                framebuffer_flips;  /* just an informative counter                                              */
    /* Borders / Window / Letterbox                                                                                         */
    bool                    has_margins;        /* true if display width/height is less than screen width/height            */
    uint16_t                display_width;      /* Display width  = Screen width  - 2 * Horizontal margin                   */
    uint16_t                display_height;     /* Display height = Screen height - 2 * Vertical   margin                   */
    uint8_t                 horizontal_margin;  /* EVEN number of pixels to fill with border color at left and right        */
    uint8_t                 vertical_margin;    /* EVEN number of pixels to fill with border color at top and bottom        */
    /* Margin colors (16 bits values, not palette indexes)                                                                  */
    BGAR5515                border_color_top;
    BGAR5515                border_color_left;
    BGAR5515                border_color_bottom;
    BGAR5515                border_color_right;
    /* Pre-calculated double pixels for margins (should be replaced with COMPOSABLE_RAW_RUN at least for top & bottom)      */
    uint32_t                border_color_top_32;
    uint32_t                border_color_left_32;
    uint32_t                border_color_bottom_32;
    uint32_t                border_color_right_32;
    /* Pre-calculated pixel combinations for 1/2/4 bit depths                                                               */
    uint32_t                double_palette_1bpp[ 2 *  2]; /* Specific to 1 bit  depth /  2 colors mode                      */
    uint32_t                double_palette_2bpp[ 4 *  4]; /* Specific to 2 bits depth /  4 colors mode                      */
    uint32_t                double_palette_4bpp[16 * 16]; /* Specific to 4 bits depth / 16 colors mode                      */
} pico_vgaboard_framebuffer_t;

/** @brief Start framebuffer */
void pico_vgaboard_framebuffer_init(pico_vgaboard_framebuffer_t *fb, bool double_buffer, uint8_t *vram, uint8_t depth, uint16_t *palette, uint16_t width, uint16_t height, uint16_t display_width, uint16_t display_height, BGAR5515 border_color);

/** @brief Flips framebuffer from 0 to 1 or 1 to 0 at next VSYNC period */
void pico_vgaboard_framebuffer_flip(pico_vgaboard_framebuffer_t *fb);

/** @brief Setup double palette for 1bpp */
void pico_vgaboard_framebuffer_start_double_palette_1bpp();

/** @brief Setup double palette for 2bpp */
void pico_vgaboard_framebuffer_start_double_palette_2bpp();

/** @brief Setup double palette for 4bpp */
void pico_vgaboard_framebuffer_start_double_palette_4bpp();

/** @brief Put pixel at (x, y) with color index in current palette or true color */
void pico_vgaboard_put_pixel(uint16_t x, uint16_t y, BGAR5515 index_or_color);

/** @brief Get RGB color from index in current palette, returns 0 in 16bpp depth */
BGAR5515 pico_vgaboard_get_palette_color(uint8_t index);

/** @brief Get color index or RGB color for given pixel */
BGAR5515 pico_vgaboard_get_pixel_index(uint16_t x, uint16_t y);

/** @brief Get RGB color for given pixel */
BGAR5515 pico_vgaboard_get_pixel_color(uint16_t x, uint16_t y);

/** @brief Compute framebufer size from depth, width & height */
size_t pico_vgaboard_get_framebuffer_size(uint8_t depth, uint16_t width, uint16_t height);

/** @brief Get luminance of RGB color (between 0 and 310,000) */
int pico_vgaboard_get_luminance(BGAR5515 rgb);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_FRAMEBUFFER_H */
