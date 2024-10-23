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

#ifndef _PICO_VGABOARD_H
#define _PICO_VGABOARD_H

#include "pico/scanvideo.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define PICO_VGABOARD_DATA __not_in_flash("pico_vgaboard_data")

/* No debuggging by default */
#ifndef PICO_VGABOARD_DEBUG
#define PICO_VGABOARD_DEBUG 0
#endif

/* Use LED for activity */
#ifndef USE_ONBOARD_LED
#define USE_ONBOARD_LED 1
#endif

/* Use interpolator in 16 colors mode? */
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

/* Video RAM size defaults to 64K */
#ifndef PICO_VGABOARD_VRAM_SIZE
#define PICO_VGABOARD_VRAM_SIZE (64 * 1024)
#endif

    /** @brief 16 bits color: 5 bits for blue, 5 for green, 1 for alpha, 5 for red */
    typedef uint16_t BGAR5515;

    typedef enum _pico_vgaboard_plane_type
    {
        PICO_VGABOARD_PLANE_FRAMEBUFFER,
        PICO_VGABOARD_PLANE_TILES,
        PICO_VGABOARD_PLANE_SPRITES,
        PICO_VGABOARD_PLANE_CONSOLE,
    } pico_vgaboard_plane_type_t;

    /** @brief pointer to plane init function */
    typedef uint16_t (*t_plane_init_func)(void *plane_params, uint32_t scanline_id, uint32_t *data, uint16_t data_max);

    /** @brief pointer to plane render scanline function */
    typedef uint16_t (*t_plane_render_scanline_func)(void *plane_params, uint32_t scanline_id, uint32_t *data, uint16_t data_max);

    /** @brief Type, flags, state, initialization & render functions for one plane */
    typedef struct _pico_vgaboard_plane
    {
        uint8_t type;
        uint8_t flags;
        void *state;
        t_plane_render_scanline_func initialize;
        t_plane_render_scanline_func render_scanline;
    } pico_vgaboard_plane_t;

    /** @brief VGA board internals */
    typedef struct _pico_vgaboard
    {
        const scanvideo_mode_t *scanvideo_mode; /* VGA timings and scale                                                    */
        uint16_t width;                         /* Screen width                                                             */
        uint16_t height;                        /* Screen height                                                            */
        uint8_t freq_hz;                        /* Info: refresh rate                                                       */
        uint32_t sys_clock_khz;                 /* 0 = do not change system clock at startup                                */
        uint8_t vreg_voltage;                   /* 0 = do not change VREG voltage at startup                                */
        bool scanvideo_active;                  /* true if scanvideo has been enabled                                       */
        pico_vgaboard_plane_t planes[3]         /* */
    } pico_vgaboard_t;

    // /** @brief VGA board mutex */
    // extern static mutex_t RAM pico_vgaboard_mutex;

    /** @brief VGA board internals */
    extern pico_vgaboard_t *pico_vgaboard;

    /** @brief Specific to 1 bit depth / 2 colors mode */
    extern uint32_t double_palette_1bpp[2 * 2];

    /** @brief Specific to 2 bit depth / 4 colors mode */
    extern uint32_t double_palette_2bpp[4 * 4];

    /** @brief Specific to 4 bits depth / 16 colors mode */
    extern uint32_t double_palette_4bpp[16 * 16];

    /** @brief Dump scanvideo mode to console */
    void scanvideo_dump(const scanvideo_mode_t *scanvideo_mode);

    /** @brief Dump VGA board state to console */
    void pico_vgaboard_dump(const pico_vgaboard_t *pico_vgaboard);

    /** @brief Frame counter */
    extern uint64_t pico_vgaboard_frame_counter;

    /**
     * @brief Init onboard LED if USE_ONBOARD_LED is 1
     */
    void pico_vgaboard_init_led();

    /**
     * @brief Flash onboard LED if USE_ONBOARD_LED is 1 for 500ms in total
     */
    void pico_vgaboard_flash_led_and_wait();

    /**
     * @brief Toggle onboard LED if USE_ONBOARD_LED is 1
     */
    void pico_vgaboard_toggle_led();

    /** @brief Set VGA board palette */
    void pico_vgaboard_set_palette(const BGAR5515 *palette);

    /**
     * @brief VGA board initialization of LED and possibly other stuff,
     *        to be called once at startup
     *        (NB: interpolator init for 4bpp / 16 colors has to be done on VGA core)
     */
    void pico_vgaboard_init(bool double_buffer);

    /** @brief Flips framebuffer from 0 to 1 or 1 to 0 at VSYNC period */
    void pico_vgaboard_framebuffer_flip();

    /** @brief Set system clock if needed (sys_clock_khz > 0) */
    bool pico_vgaboard_set_system_clock(uint32_t sys_clock_khz);

    /** @brief Setup double palette for 1bpp */
    void pico_vgaboard_start_double_palette_1bpp();

    /** @brief Setup double palette for 2bpp */
    void pico_vgaboard_start_double_palette_2bpp();

    /** @brief Setup double palette for 4bpp */
    void pico_vgaboard_start_double_palette_4bpp();

    /** @brief VGA board initialization, should not be called several times for now */
    void pico_vgaboard_start(const pico_vgaboard_t *model, uint16_t display_width, uint16_t display_height, BGAR5515 border_color);

    // /** @brief TODO VGA board change mode, with hopefully a compatible one */
    // void pico_vgaboard_change(const pico_vgaboard_t *model);

    // /** @brief TODO Enable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void pico_vgaboard_enable();

    // /** @brief TODO Disable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void pico_vgaboard_disable();

    /** @brief VGA render loop using scanvideo (on core1) */
    void pico_vgaboard_render_loop(void);

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

#endif /* _PICO_VGABOARD_H */
