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

#ifndef _PICO_VGABOARD_H
#define _PICO_VGABOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"

#ifndef PICO_VGABOARD_DEBUG
#define PICO_VGABOARD_DEBUG 0
#endif

/* Use LED for activity? */
#ifndef USE_LED
#define USE_LED == 0
#endif

/* Use interpolator in 16 colors mode? */
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

/* Framebuffer size defaults to 64K */
#ifndef PICO_VGABOARD_FRAMEBUFFER_SIZE
#define PICO_VGABOARD_FRAMEBUFFER_SIZE (64 * 1024)
#endif

/** @brief VGA board internals */
typedef struct _vgaboard
{
    const scanvideo_mode_t *scanvideo_mode;
    uint8_t                 freq_hz;            /* Info */
    uint16_t                width;
    uint16_t                height;
    uint8_t                 depth;
    uint32_t                colors;             /* 65536 does not fit in an uint16_t */
    uint16_t               *palette;            /* NULL for 16 bits depth / 65536 colors */
    uint32_t                framebuffer_size;   /* bytes */
    uint8_t                *framebuffer;        /* PICO_VGABOARD_FRAMEBUFFER_SIZE bytes */
    uint32_t                sys_clock_khz;      /* 0 to not change system clock at startup */
} vgaboard_t;

/** @brief VGA default palette 8 bpp / 256 colors */
extern uint16_t vgaboard_palette_8bpp_default[256];

/** @brief VGA board internals */
extern vgaboard_t *vgaboard;

/** @brief Dump scanvideo mode */
void scanvideo_dump(scanvideo_mode_t *scanvideo_mode);

/** @brief Dump VGA board state */
void vgaboard_dump(vgaboard_t *vgaboard);

#if PICO_VGABOARD_DEBUG
extern uint32_t vgaboard_counter;
#endif

/**
 * @brief VGA board initialization of interpolation for 4bpp / 16 colors,
 *        to be called once at startup
 */
void vgaboard_init();

/** @brief Set system clock if needed */
bool vgaboard_set_system_clock(uint32_t sys_clock_khz);

/** @brief Setup double palette for 1bpp */
void vgaboard_setup_double_palette_1bpp();

/** @brief Setup double palette for 2bpp */
void vgaboard_setup_double_palette_2bpp();

/** @brief Setup double palette for 4bpp */
void vgaboard_setup_double_palette_4bpp();

/** @brief Setup default palette for 8bpp */
void vgaboard_init_default_palette_8bpp();

/** @brief VGA board initialization, could be called several times */
void vgaboard_setup(const vgaboard_t *model);

/** @brief Set VGA board palette */
void vgaboard_set_palette(const uint16_t *palette);

/** @brief Enable VGA board (timers, PIO, DMA, interrupts, ...) */
void vgaboard_enable();

/** @brief Disable VGA board (timers, PIO, DMA, interrupts, ...) */
void vgaboard_disable();

/** @brief VGA render loop using scanvideo's functions */
void vgaboard_render_loop(void);

/** @brief Put pixel at (x, y) with color index in current palette or true color */
void vgaboard_put_pixel(uint16_t x, uint16_t y, uint16_t index_or_color);

/** @brief Get RGB color from index in current palette, returns 0 in 16bpp depth */
uint16_t vgaboard_get_palette_color(uint8_t index);

/** @brief Get color index for given pixel */
uint8_t vgaboard_get_pixel_index(uint16_t x, uint16_t y);

/** @brief Get RGB color for given pixel */
uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_H */
