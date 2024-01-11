/*

MIT No Attribution

Copyright (c) 2021-2024 Christophe "CHiPs44" Petit

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
#else
#include <time.h>
#endif

uint32_t frame_counter;
uint64_t frame_start, frame_end, frame_elapsed;
uint64_t vblank_start, vblank_end, vblank_elapsed;
uint64_t render_start, render_end, render_elapsed;
int hours, minutes, seconds, milliseconds, fps;
wchar_t status_text[80];

#if !PICO_NO_HARDWARE
/**
 * @brief Get the time - Pico SDK / newlib version
 *        cf. https://lindevs.com/measure-execution-time-of-code-using-raspberry-pi-pico
 * @return uint64_t
 */
uint64_t get_time_ms()
{
    return (uint64_t)time_us_64() / 10000 * 1000 / CLOCKS_PER_SEC;
}
#else
/**
 * @brief Get the time - "Linux" version
 *        cf. https://stackoverflow.com/questions/3756323/how-to-get-the-current-time-in-milliseconds-from-c-in-linux
 *            (Kennett Howlett answer, last)
 * @return uint64_t
 */
uint64_t get_time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000L + (ts.tv_nsec + 500000L) / 1000000L;
}
#endif

void start_time()
{
    frame_counter = 0;
    frame_start = get_time_ms();
}

void wait_for_vblank()
{
    if (!pico_vgaboard->scanvideo_active)
    {
        return;
    }
    vblank_start = get_time_ms();
    scanvideo_wait_for_vblank();
    frame_counter += 1;
    vblank_end = get_time_ms();
#if !PICO_NO_HARDWARE
    vblank_elapsed = (vblank_end - vblank_start) * 1000 / CLOCKS_PER_SEC;
#else
    vblank_elapsed = (vblank_end - vblank_start);
#endif
    render_start = vblank_end;
}

void show_status()
{
    if (STATUS.h > 0)
    {
        clip(&STATUS);
        // Draw elapsed time HH:MM:SS.mmm & counter
        render_end = get_time_ms();
        frame_end = render_end;
        render_elapsed = render_end - render_start;
        frame_elapsed = frame_end - frame_start;
        fps = frame_elapsed == 0 ? 0 : 1000 * frame_counter / frame_elapsed;
        hours = frame_elapsed / 1000 / 60 / 60;
        minutes = (frame_elapsed / 1000 / 60) % 60;
        seconds = (frame_elapsed / 1000) % 60;
        milliseconds = frame_elapsed % 1000;
        swprintf(
            status_text, sizeof(status_text) / sizeof(wchar_t),
            // 0        1         2         3         4
            // 1234567890123456789012345678901234567890
            // 00:00:00.000 000 000000 000 000
            // HH:MM:SS.mmm FPS FRAMES RDR VBL
            L"%02d:%02d:%02d.%03d %03d %06d %03d %03d",
            hours, minutes, seconds, milliseconds,
            fps % 1000,
            frame_counter % 1000000,
            render_elapsed % 1000,
            vblank_elapsed % 1000);
        hagl_put_text(
            hagl_backend,
            status_text,
            STATUS.x,
            STATUS.y,
            COLORS - 1,
            FONT8X8.fontx);
    }
}

/* EOF */
