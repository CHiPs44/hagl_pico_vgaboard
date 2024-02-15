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

#ifndef _HAGL_PICO_VGABOARD_BUTTONS_H
#define _HAGL_PICO_VGABOARD_BUTTONS_H

#include "pico/scanvideo.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef PICO_VGABOARD_BUTTONS_A_PIN
#define PICO_VGABOARD_BUTTONS_A_PIN (PICO_SCANVIDEO_COLOR_PIN_BASE + 0)
#endif

#ifndef PICO_VGABOARD_BUTTONS_B_PIN
#define PICO_VGABOARD_BUTTONS_B_PIN (PICO_SCANVIDEO_COLOR_PIN_BASE + 6)
#endif

#ifndef PICO_VGABOARD_BUTTONS_C_PIN
#define PICO_VGABOARD_BUTTONS_C_PIN (PICO_SCANVIDEO_COLOR_PIN_BASE + 11)
#endif

#ifndef PICO_VGABOARD_BUTTONS_VSYNC_PIN
#define PICO_VGABOARD_BUTTONS_VSYNC_PIN (PICO_SCANVIDEO_COLOR_PIN_BASE + PICO_SCANVIDEO_COLOR_PIN_COUNT + 1)
#endif

/* Delays in µs */
#define PICO_VGABOARD_BUTTONS_DELAY_SHORT 300000
#define PICO_VGABOARD_BUTTONS_DELAY_MEDIUM 850000
#define PICO_VGABOARD_BUTTONS_DELAY_REPEAT 125000
// 3 buttons: A (left), B (center) & C (right)
#define PICO_VGABOARD_BUTTONS_COUNT 3
#define PICO_VGABOARD_BUTTONS_A 0
#define PICO_VGABOARD_BUTTONS_B 1
#define PICO_VGABOARD_BUTTONS_C 2

    typedef enum
    {
        PICO_VGABOARD_BUTTONS_EVENT_NONE = 0,
        // Release after PICO_VGABOARD_BUTTONS_DELAY_SHORT µs
        PICO_VGABOARD_BUTTONS_EVENT_SHORT,
        // Release after PICO_VGABOARD_BUTTONS_DELAY_MEDIUM µs
        PICO_VGABOARD_BUTTONS_EVENT_MEDIUM,
        // Button still pressed: repeat every PICO_VGABOARD_BUTTONS_DELAY_REPEAT µs
        PICO_VGABOARD_BUTTONS_EVENT_REPEAT
    } pico_vgaboard_buttons_event;

    /** @brief State of one button */
    typedef struct _pico_vgaboard_buttons_state
    {
        uint8_t pin;
        volatile uint8_t state;
        uint8_t last_state;
        uint32_t last_time;
        volatile pico_vgaboard_buttons_event event;
    } pico_vgaboard_buttons_state;

    /** @brief State of all buttons */
    extern pico_vgaboard_buttons_state pico_vgaboard_buttons_states[PICO_VGABOARD_BUTTONS_COUNT];

    extern void pico_vgaboard_buttons_init();

    extern void pico_vgaboard_buttons_handle_input();

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_BUTTONS_H */
