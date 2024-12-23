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

#ifndef PICO_VGABOARD_VSYNC_PIN
#define PICO_VGABOARD_VSYNC_PIN (PICO_SCANVIDEO_COLOR_PIN_BASE + PICO_SCANVIDEO_COLOR_PIN_COUNT + 1)
#endif

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

    /** @brief 16 bits color: 5 bits for blue, 5 for green, 1 for alpha, 5 for red */
    typedef uint16_t BGAR5515;

    /** @brief Plane type */
    typedef enum _pico_vgaboard_plane_type
    {
        PICO_VGABOARD_PLANE_NONE,
        PICO_VGABOARD_PLANE_FRAMEBUFFER,
        PICO_VGABOARD_PLANE_CONSOLE,
        PICO_VGABOARD_PLANE_TILES,
        PICO_VGABOARD_PLANE_SPRITES,
        PICO_VGABOARD_PLANE_CUSTOM,
    } pico_vgaboard_plane_type_t;

    /** @brief pointer to plane initialization function */
    typedef void (*t_plane_init_func)(void *plane_state);

    /** @brief pointer to plane scanline render function */
    typedef uint16_t (*t_plane_render_scanline_func)(void *plane_state, uint16_t scanline_number, uint32_t *data, uint16_t data_max);

    /** @brief Type, flags, state, initialization & render functions for one plane */
    typedef struct _pico_vgaboard_plane
    {
        uint8_t type;
        uint8_t flags;
        void *state;
        t_plane_init_func initialize;
        t_plane_render_scanline_func render_scanline;
    } pico_vgaboard_plane_t;

    /** @brief VGA board internals definition */
    typedef struct _pico_vgaboard
    {
        const scanvideo_mode_t *scanvideo_mode; /** @brief VGA timings and scale                            */
        uint16_t width;                         /** @brief Screen width in pixels                           */
        uint16_t height;                        /** @brief Screen height in pixels                          */
        uint8_t freq_hz;                        /** @brief Info: refresh rate in Hz                         */
        uint32_t sys_clock_khz;                 /** @brief 0 = do not change system clock at startup        */
        uint8_t vreg_voltage;                   /** @brief 0 = do not change VREG voltage at startup        */
        bool scanvideo_active;                  /** @brief true if scanvideo has been enabled               */
        volatile bool in_vsync;                 /** @brief > 0 if in vertical sync period (via IRQ handler) */
        uint64_t frame_counter;                 /** @brief Frame counter */
        pico_vgaboard_plane_t planes[3];        /** @brief plane definitions                                */
    } pico_vgaboard_t;

    // /** @brief VGA board mutex */
    // extern static mutex_t RAM pico_vgaboard_mutex;

    /** @brief VGA board internals */
    extern pico_vgaboard_t *pico_vgaboard;

    /** @brief Dump scanvideo mode to console */
    void scanvideo_mode_dump(const scanvideo_mode_t *scanvideo_mode);

    /** @brief Dump VGA board state to console */
    void pico_vgaboard_dump(const pico_vgaboard_t *pico_vgaboard);

    /** @brief Init onboard LED if USE_ONBOARD_LED is 1 */
    void pico_vgaboard_init_led();

    /** @brief Flash onboard LED if USE_ONBOARD_LED is 1 for 500ms in total */
    void pico_vgaboard_flash_led_and_wait();

    /** @brief Toggle onboard LED if USE_ONBOARD_LED is 1 */
    void pico_vgaboard_toggle_led();

    /** @brief Set VGA board palette */
    void pico_vgaboard_set_palette(const BGAR5515 *palette);

    /** @brief VGA board initialization of LED and possibly other stuff, to be called once at startup */
    void pico_vgaboard_init();

    /** @brief Wait for vertical sync (in_vsync set by IRQ handler on core1) */
    void pico_vgaboard_wait_for_vsync();

    /** @brief Set system clock if needed (sys_clock_khz > 0) */
    bool pico_vgaboard_set_system_clock(uint32_t sys_clock_khz);

    /** @brief initialize plane state */
    void pico_vgaboard_init_plane(uint8_t plane, uint8_t type, uint8_t flags, void *state, t_plane_init_func initialize, t_plane_render_scanline_func render_scanline);

    /** @brief VGA board initialization, should not be called several times for now */
    void pico_vgaboard_start(const pico_vgaboard_t *model);

    // /** @brief TODO VGA board change mode, with hopefully a compatible one */
    // void pico_vgaboard_change(const pico_vgaboard_t *model);

    // /** @brief TODO Enable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void pico_vgaboard_enable();

    // /** @brief TODO Disable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void pico_vgaboard_disable();

    /** @brief VGA render loop using scanvideo (on core1) */
    void pico_vgaboard_render_loop(void);

    /** @brief Get luminance of RGB color (between 0 and 310,000) */
    int pico_vgaboard_get_luminance(BGAR5515 rgb);

#ifdef __cplusplus
}
#endif

#endif /* _PICO_VGABOARD_H */
