/*

MIT License

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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

/*

Mostly borrowed from https://github.com/raspberrypi/pico-playground/blob/master/apps/popcorn/popcorn.c

int main(void) {
#if USE_VGABOARD_BUTTONS
    pico_vgaboard_init_buttons();
#endif
}
*/

#include "pico-vgaboard-buttons.h"

#include "hardware/irq.h"
#include "hardware/structs/timer.h"
#include "pico/stdlib.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "hardware/divider.h"

pico_vgaboard_buttons_state pico_vgaboard_buttons_states[PICO_VGABOARD_BUTTONS_COUNT] = {
    {.pin = PICO_VGABOARD_BUTTONS_A_PIN},
    {.pin = PICO_VGABOARD_BUTTONS_B_PIN},
    {.pin = PICO_VGABOARD_BUTTONS_C_PIN},
};

// Registered as GPIO interrupt on both edges of vsync. On vsync assertion,
// set pins to input. On deassertion, sample and set back to output.
void pico_vgaboard_buttons_irq_handler()
{
    int vsync_current_level = gpio_get(PICO_VGABOARD_BUTTONS_VSYNC_PIN);
    gpio_acknowledge_irq(PICO_VGABOARD_BUTTONS_VSYNC_PIN, vsync_current_level ? GPIO_IRQ_EDGE_RISE : GPIO_IRQ_EDGE_FALL);

    // Note v_sync_polarity == 1 means active-low because anything else would be confusing
    if (vsync_current_level != scanvideo_get_mode().default_timing->v_sync_polarity)
    {
        for (int i = 0; i < PICO_VGABOARD_BUTTONS_COUNT; ++i)
        {
            gpio_pull_down(pico_vgaboard_buttons_states[i].pin);
            gpio_set_oeover(pico_vgaboard_buttons_states[i].pin, GPIO_OVERRIDE_LOW);
        }
    }
    else
    {
        for (int i = 0; i < PICO_VGABOARD_BUTTONS_COUNT; ++i)
        {
            pico_vgaboard_buttons_states[i].state = gpio_get(pico_vgaboard_buttons_states[i].pin);
            gpio_set_oeover(pico_vgaboard_buttons_states[i].pin, GPIO_OVERRIDE_NORMAL);
        }
    }
}

void pico_vgaboard_buttons_init()
{
    gpio_set_irq_enabled(PICO_VGABOARD_BUTTONS_VSYNC_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    irq_set_exclusive_handler(IO_IRQ_BANK0, pico_vgaboard_buttons_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void pico_vgaboard_buttons_handle_input()
{
    for (uint b = 0; b < 3; b++)
    {
        if (!(pico_vgaboard_buttons_states[b].state) && (pico_vgaboard_buttons_states[b].last_state))
        {
            // button released
            int32_t length = time_us_32() - pico_vgaboard_buttons_states[b].last_time;
            if (length < PICO_VGABOARD_BUTTONS_DELAY_SHORT)
            {
                pico_vgaboard_buttons_states[b].event = PICO_VGABOARD_BUTTONS_EVENT_SHORT;
            }
            else if (length < PICO_VGABOARD_BUTTONS_DELAY_MEDIUM)
            {
                pico_vgaboard_buttons_states[b].event = PICO_VGABOARD_BUTTONS_EVENT_MEDIUM;
            }
            else
            {
                pico_vgaboard_buttons_states[b].event = PICO_VGABOARD_BUTTONS_EVENT_NONE;
            }
        }
        else if ((pico_vgaboard_buttons_states[b].state) && !(pico_vgaboard_buttons_states[b].last_state))
        {
            // button pressed, wait for short release, medium release or repeat
            pico_vgaboard_buttons_states[b].last_time = time_us_32();
        }
        else if ((pico_vgaboard_buttons_states[b].state) && (pico_vgaboard_buttons_states[b].last_state))
        {
            // button held
            int32_t length = time_us_32() - pico_vgaboard_buttons_states[b].last_time;
            if (length >= PICO_VGABOARD_BUTTONS_DELAY_MEDIUM + PICO_VGABOARD_BUTTONS_DELAY_REPEAT)
            {
                pico_vgaboard_buttons_states[b].event = PICO_VGABOARD_BUTTONS_EVENT_REPEAT;
                pico_vgaboard_buttons_states[b].last_time += PICO_VGABOARD_BUTTONS_DELAY_REPEAT;
            }
            pico_vgaboard_buttons_states[b].last_state = pico_vgaboard_buttons_states[b].state;
        }
    }
}
