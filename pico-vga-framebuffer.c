#include <stdio.h>
#include <string.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "hagl_hal.h"
#include "pico-vga-framebuffer.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
#endif

uint16_t palette[NCLR] = {
    PAL16_BLACK,          PAL16_DARK_RED,       PAL16_DARK_GREEN,     PAL16_DARK_YELLOW, 
    PAL16_DARK_BLUE,      PAL16_DARK_MAGENTA,   PAL16_DARK_CYAN,      PAL16_LIGHT_GREY, 
    PAL16_DARK_GREY,      PAL16_RED,            PAL16_GREEN,          PAL16_YELLOW,
    PAL16_BLUE,           PAL16_MAGENTA,        PAL16_CYAN,           PAL16_WHITE
};

uint32_t dblpal[NCLR * NCLR];

uint8_t fbuf[WIDTH * HEIGHT / 2];

#if USE_INTERP == 1
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

void __time_critical_func(render_loop)(void)
{
#ifdef DEBUG
    printf("Starting render\n");
#endif
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        int iScan = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *twoclr = buffer->data;
        uint8_t *twopix = &fbuf[(WIDTH / 2) * iScan];
#if USE_INTERP == 1
        ++twoclr;
        convert_from_pal16(twoclr, twopix, WIDTH / 2);
        twoclr += WIDTH / 2;
#else
        for (int iCol = 0; iCol < WIDTH / 2; ++iCol)
        {
            ++twoclr;
            *twoclr = dblpal[*twopix];
            ++twopix;
        }
        ++twoclr;
#endif
        *twoclr = COMPOSABLE_EOL_ALIGN << 16;
        twoclr = buffer->data;
        twoclr[0] = (twoclr[1] << 16) | COMPOSABLE_RAW_RUN;
        twoclr[1] = (twoclr[1] & 0xFFFF0000) | (WIDTH - 2);
        buffer->data_used = (WIDTH + 4) / 2;
        scanvideo_end_scanline_generation(buffer);
    }
}

void setup_video(void)
{
    // Fill screen with black
    memset(fbuf, 0x00, sizeof(fbuf));
    // memset(fbuf + 0xff, 0x18, 0x1ff);
    // memset(fbuf + 0x1ff, 0x29, 0x1ff);
    // for ( int x = 0; x < WIDTH; ++x ) {
    //     plot_point(x,          0 + 20, x % NCLR);
    //     plot_point(x, HEIGHT - 1 - 20, x % NCLR);
    // }
    // for ( int y = 0; y < HEIGHT; ++y) {
    //     plot_point(        0 + 20, y, y % NCLR);
    //     plot_point(WIDTH - 1 - 20, y, y % NCLR);
    // }
    // Initialize palette
    set_palette(palette);
#ifdef DEBUG
    printf("System clock speed %d kHz\n", clock_get_hz(clk_sys) / 1000);
    printf("Starting video\n");
#endif
#if USE_INTERP == 1
    // Configure interpolater lanes
    interp_config c = interp_default_config();
    interp_config_set_shift(&c, 22);
    interp_config_set_mask(&c, 2, 9);
    interp_set_config(interp0, 0, &c);
    interp_config_set_shift(&c, 14);
    interp_config_set_cross_input(&c, true);
    interp_set_config(interp0, 1, &c);
    interp_set_base(interp0, 0, (uintptr_t)dblpal);
    interp_set_base(interp0, 1, (uintptr_t)dblpal);
#endif
    scanvideo_setup(&vga_mode_640x480_60);
    scanvideo_timing_enable(true);
#ifdef DEBUG
    printf("System clock speed %d kHz\n", clock_get_hz(clk_sys) / 1000);
#endif
}

void set_palette(uint16_t *pclr)
{
    uint32_t *dpal = dblpal;
    for (int i = 0; i < NCLR; ++i)
    {
        for (int j = 0; j < NCLR; ++j)
        {
            *dpal = (pclr[i] << 16) | pclr[j];
            ++dpal;
        }
    }
}

void plot_point(int x, int y, int clr)
{
    bool odd = x & 1;
    int n = (WIDTH / 2) * y + x / 2;
    clr &= 0x0F;
    if ((n >= 0) && (n < WIDTH * HEIGHT / 2))
    {
        uint8_t *p = &fbuf[n];
        if (odd)
            // left pixel of the byte
            *p = (clr << 4) | (*p & 0x0F);
        else
            // right pixel of the byte
            *p = (*p & 0xF0) | clr;
    }
}

// EOF
