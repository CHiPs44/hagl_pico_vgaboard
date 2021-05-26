#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"
// #include "pico/malloc.h"

// #include "hagl_hal.h"
#include "pico-vga-framebuffer.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
#endif

// Screen Width
uint16_t screen_width;
// Screen Height
uint16_t screen_height;
// Bits per pixel
uint8_t display_bpp;
// Specific to 4 bpp / 16 colors mode
uint32_t dblpal[16 * 16];
// Framebuffer
uint8_t *framebuffer;

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
        uint8_t *twopix;
        switch (display_bpp)
        {
            case 4:
                twopix = &framebuffer[(screen_width / 2) * iScan];
#if USE_INTERP == 1
                ++twoclr;
                convert_from_pal16(twoclr, twopix, screen_width / 2);
                twoclr += screen_width / 2;
#else
                for (int iCol = 0; iCol < screen_width / 2; ++iCol)
                {
                    ++twoclr;
                    *twoclr = dblpal[*twopix];
                    ++twopix;
                }
                ++twoclr;
#endif
                break;
            default:
                break;
        }
        *twoclr = COMPOSABLE_EOL_ALIGN << 16;
        twoclr = buffer->data;
        twoclr[0] = (twoclr[1] << 16) | COMPOSABLE_RAW_RUN;
        twoclr[1] = (twoclr[1] & 0xFFFF0000) | (screen_width - 2);
        buffer->data_used = (screen_width + 4) / 2;
        scanvideo_end_scanline_generation(buffer);
    }
}

void setup_video(const scanvideo_mode_t *vga_mode, uint8_t bpp, uint16_t *palette)
{
    screen_width = vga_mode->width;
    screen_height = vga_mode->height;
    display_bpp = bpp;
    framebuffer = (uint8_t *)malloc(screen_width * screen_height * display_bpp / 8);
    // Fill screen with black
    memset(framebuffer, 0, sizeof(framebuffer));
    // Initialize palette
    setup_palette(palette);
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
    scanvideo_setup(vga_mode);
    scanvideo_timing_enable(true);
#ifdef DEBUG
    printf("System clock speed %d kHz\n", clock_get_hz(clk_sys) / 1000);
#endif
}

void setup_palette(uint16_t *pclr)
{
    // palette is 256 entries of 32bits; i.e. all 2 pixel combinations
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

void plot_point(uint16_t x, uint16_t y, uint16_t clr)
{
    bool odd = x & 1;
    int n = (screen_width / 2) * y + x / 2;
    clr &= 0x0F;
    if ((n >= 0) && (n < screen_width * screen_height / 2))
    {
        uint8_t *p = &framebuffer[n];
        if (odd)
            // left pixel of the byte
            *p = (clr << 4) | (*p & 0x0F);
        else
            // right pixel of the byte
            *p = (*p & 0xF0) | clr;
    }
}

uint16_t get_point(uint16_t x, uint16_t y)
{
    uint16_t clr = 0;
    bool odd = x & 1;
    int n = (screen_width / 2) * y + x / 2;
    if ((n >= 0) && (n < screen_width * screen_height / 2))
    {
        uint8_t *p = &framebuffer[n];
        if (odd) {
            // left pixel of the byte
            clr = (*p) >> 4;
        } else {
            // right pixel of the byte
            clr = (*p) & 0x0F;
        }
    }
    return clr;
}

// EOF
