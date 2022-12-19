#include "hardware/clocks.h"

uint32_t frame_counter = 0;
int led = 0;
clock_t frameStart, frameEnd, frameElapsed;
clock_t vblankStart, vblankEnd, vblankElapsed;
clock_t renderStart, renderEnd, renderElapsed;
int hours, minutes, seconds, milliseconds, fps;

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
    led = 0;
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

void cycle_time(int16_t x, int16_t y, color_t color)
{
    font_t *font = HEIGHT > 200 ? &FONT8X8 : &FONT5X7;
    wchar_t text[40];

    // Draw counter & elapsed time HH:MM:SS.mmm
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
        text, sizeof(text), 
        // 1234567890123456789012345678901234567890
        // 00:00:00.000 000 0000000 0000 0000
        // HH:MM:SS.mmm FPS FRAMES  RNDR VBLK
        L"%02d:%02d:%02d.%03d %03d %07d %04d %04d", 
        hours, minutes, seconds, milliseconds, 
        fps % 1000, 
        frame_counter % 1000000, 
        renderElapsed % 10000,
        vblankElapsed % 10000
    );
    hagl_put_text(hagl_backend, text, x, y == HEIGHT - 1 ? HEIGHT - font->h : y, color, font->fontx);
    // Next cycle
    frame_counter += 1;
    if (frame_counter % 10 == 0) {
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        led = 1 - led;
    }
}
