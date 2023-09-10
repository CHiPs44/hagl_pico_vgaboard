/*

MIT No Attribution

Copyright (c) 2021-2023 Christophe "CHiPs44" Petit

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#if !PICO_NO_HARDWARE
#include "hardware/clocks.h"
#endif

uint32_t frame_counter = 0;
#ifdef USE_ONBOARD_LED
int led;
#endif
clock_t frameStart, frameEnd, frameElapsed;
clock_t vblankStart, vblankEnd, vblankElapsed;
clock_t renderStart, renderEnd, renderElapsed;
int hours, minutes, seconds, milliseconds, fps;
wchar_t status_text[40];

/**
 * @brief Get the time
 *        cf. https://lindevs.com/measure-execution-time-of-code-using-raspberry-pi-pico
 * @return clock_t 
 */
clock_t get_time()
{
    /*  */
    return (clock_t) time_us_64() / 10000;
}

clock_t get_time_ms()
{
    return (clock_t) time_us_64() / 10000 * 1000 / CLOCKS_PER_SEC;
}

void start_time()
{
    frame_counter = 0;
#ifdef USE_ONBOARD_LED
    led = 0;
#endif
    frameStart = get_time();
}

void wait_for_vblank()
{
    vblankStart = get_time();
    scanvideo_wait_for_vblank();
    vblankEnd = get_time();
    vblankElapsed = (vblankEnd - vblankStart) * 1000 / CLOCKS_PER_SEC;
    renderStart = vblankEnd;
}

void show_status()
{
    if (STATUS.h > 0) {
        // Draw elapsed time HH:MM:SS.mmm & counter
        renderEnd = get_time();
        renderElapsed = (renderEnd - renderStart) * 1000 / CLOCKS_PER_SEC;
        frameEnd = renderEnd;
        frameElapsed = (frameEnd - frameStart) * 1000 / CLOCKS_PER_SEC;
        fps = 1000 * frame_counter / frameElapsed;
        hours = frameElapsed / 1000 / 60 / 60;
        minutes = (frameElapsed / 1000 / 60) % 60;
        seconds = (frameElapsed / 1000) % 60;
        milliseconds = frameElapsed % 1000;
        swprintf(
            status_text, sizeof(status_text), 
            // 0        1         2         3         4
            // 1234567890123456789012345678901234567890
            // 00:00:00.000 000 000000 000 000
            // HH:MM:SS.mmm FPS FRAMES RDR VBL
            L"%02d:%02d:%02d.%03d %03d %06d %03d %03d", 
            hours, minutes, seconds, milliseconds, 
            fps % 1000, 
            frame_counter % 1000000, 
            renderElapsed % 1000,
            vblankElapsed % 1000
        );
        hagl_put_text(
            hagl_backend, 
            status_text, 
            STATUS.x, 
            STATUS.y, 
            COLORS - 1, 
            (const char *)(&FONT8X8)
        );
    }
    // Next cycle
    frame_counter += 1;
#ifdef USE_ONBOARD_LED
    if (frame_counter % 10 == 0) {
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        led = 1 - led;
    }
#endif
}
