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
#ifndef USE_ONBOARD_LED
#define USE_ONBOARD_LED == 0
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
    const scanvideo_mode_t *scanvideo_mode;     /* VGA timings and scale */
    uint16_t                width;              /* Screen width */
    uint16_t                height;             /* Screen height */
    uint8_t                 freq_hz;            /* Info: refresh rate */
    uint8_t                 depth;              /* 1, 2,  4,    8 or    16 bits per pixel */
    uint32_t                colors;             /* 2, 4, 16,  256 or 65536 (which does not fit in an uint16_t) */
    uint16_t               *palette;            /* Up to 256 BGAR5515 values, may be NULL for 16 bits depth / 65536 colors */
    uint32_t                framebuffer_size;   /* in bytes */
    uint8_t                *framebuffer;        /* PICO_VGABOARD_FRAMEBUFFER_SIZE bytes */
    uint32_t                sys_clock_khz;      /* 0 = do not change system clock at startup */
    uint8_t                 vreg_voltage;       /* 0 = do not change VREG voltage at startup */
    /* WINDOW / LETTERBOX */
    bool                    has_window;         /* true if display width/height is less than screen width/height */
    uint16_t                display_width;      /* Display width  = Screen width  - 2 * Horizontal margin */
    uint16_t                display_height;     /* Display height = Screen height - 2 * Vertical   margin */
    uint8_t                 horizontal_margin;  /* EVEN number of pixels to fill with border color at left and right */
    uint8_t                 vertical_margin;    /* EVEN number of pixels to fill with border color at top and bottom */
    uint16_t                border_color;       /* Margins color (BGAR5515, not a palette index) */
} vgaboard_t;

// /** @brief VGA board mutex */
// extern static mutex_t RAM vgaboard_mutex;

/** @brief VGA board internals */
extern vgaboard_t *vgaboard;

/** @brief Specific to 1 bit depth / 2 colors mode */
extern uint32_t vgaboard_double_palette_1bpp[2 * 2];

/** @brief Specific to 2 bit depth / 4 colors mode */
extern uint32_t vgaboard_double_palette_2bpp[4 * 4];

/** @brief Specific to 4 bits depth / 16 colors mode */
extern uint32_t vgaboard_double_palette_4bpp[16 * 16];

/** @brief Dump scanvideo mode */
void scanvideo_dump(const scanvideo_mode_t *scanvideo_mode);

/** @brief Dump VGA board state */
void vgaboard_dump(const vgaboard_t *vgaboard);

#if PICO_VGABOARD_DEBUG
extern uint64_t vgaboard_frame_counter;
#endif

/**
 * @brief Init onboard LED if USE_ONBOARD_LED is 1
 */
void vgaboard_init_led();

/**
 * @brief Flash onboard LED if USE_ONBOARD_LED is 1 for 500ms in total
 */
void vgaboard_flash_led_and_wait();

/**
 * @brief Toggle onboard LED if USE_ONBOARD_LED is 1
 */
void vgaboard_toggle_led();

/** @brief Set VGA board palette */
void vgaboard_set_palette(const uint16_t *palette);

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

/** @brief VGA board initialization, should not be called several times for now */
void vgaboard_setup(const vgaboard_t *model, uint16_t display_width, uint16_t display_height, uint16_t border_color);

// /** @brief VGA board change mode, with hopefully a compatible one */
// void vgaboard_change(const vgaboard_t *model);

// /** @brief Enable VGA board (timers, PIO, DMA, interrupts, ...) */
// void vgaboard_enable();

// /** @brief Disable VGA board (timers, PIO, DMA, interrupts, ...) */
// void vgaboard_disable();

/** @brief VGA render loop using scanvideo's functions */
void vgaboard_render_loop(void);

/** @brief Put pixel at (x, y) with color index in current palette or true color */
void vgaboard_put_pixel(uint16_t x, uint16_t y, uint16_t index_or_color);

/** @brief Get RGB color from index in current palette, returns 0 in 16bpp depth */
uint16_t vgaboard_get_palette_color(uint8_t index);

/** @brief Get color index for given pixel */
uint16_t vgaboard_get_pixel_index(uint16_t x, uint16_t y);

/** @brief Get RGB color for given pixel */
uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_H */
