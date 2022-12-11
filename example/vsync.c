#include "hardware/clocks.h"

uint32_t frame_counter = 0;
int led = 0;
clock_t frameStart, frameEnd, frameElapsed;
clock_t vblankStart, vblankEnd, vblankElapsed;
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
    vblankElapsed = (frameEnd - frameStart) * 1000 / CLOCKS_PER_SEC;
}

void cycle_time(int16_t x, int16_t y, color_t color)
{
    wchar_t text[40];

    // Draw counter & elapsed time HH:MM:SS.mmm
    frameEnd = get_time();
    frameElapsed = (frameEnd - frameStart) * 1000 / CLOCKS_PER_SEC;
    fps = 1000 * frame_counter / frameElapsed;
    hours = frameElapsed / 1000 / 60 / 60;
    minutes = (frameElapsed / 1000 / 60) % 60;
    seconds = (frameElapsed / 1000) % 60;
    milliseconds = frameElapsed % 1000;
    swprintf(
        text, sizeof(text), 
        //123456789012345678901234567890
        //000000 00:00:00.000 000 00000
        //FRAMES HH:MM:SS.mmm FPS VSYNC
        L"%06d %02d:%02d:%02d.%03d %03d %05d", 
        frame_counter % 1000000, 
        hours, minutes, seconds, milliseconds, 
        fps % 1000, 
        vblankElapsed % 100000
    );
    hagl_put_text(hagl_backend, text, x, y, color, FONT5X7.fontx);
    // Next cycle
    frame_counter += 1;
    if (frame_counter % 10 == 0) {
        gpio_put(PICO_DEFAULT_LED_PIN, led);
        led = 1 - led;
    }
}
