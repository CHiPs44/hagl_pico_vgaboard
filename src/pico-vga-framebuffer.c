#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"
#include "pico-vga-framebuffer.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

/* Let's go for the dark colors */
#define IRGB_BLACK PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 0u, 0u)
#define IRGB_DARK_RED PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 0u, 0u)
#define IRGB_DARK_GREEN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 128u, 0u)
#define IRGB_DARK_YELLOW PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 128u, 0u)
#define IRGB_DARK_BLUE PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 0u, 128u)
#define IRGB_DARK_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(128u, 0u, 128u)
#define IRGB_DARK_CYAN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 128u, 128u)
/* NB: light and dark grey are evenly distributed to make a 4 level grayscale with black and white */
#define IRGB_DARK_GREY PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x55, 0x55, 0x55)
#define IRGB_LIGHT_GREY PICO_SCANVIDEO_PIXEL_FROM_RGB8(0xaa, 0xaa, 0xaa)
/* And then the brighter ones */
#define IRGB_RED PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 0u, 0u)
#define IRGB_GREEN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 255u, 0u)
#define IRGB_YELLOW PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 0u)
#define IRGB_BLUE PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 0u, 255u)
#define IRGB_MAGENTA PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 0u, 255u)
#define IRGB_CYAN PICO_SCANVIDEO_PIXEL_FROM_RGB8(0u, 255u, 255u)
#define IRGB_WHITE PICO_SCANVIDEO_PIXEL_FROM_RGB8(255u, 255u, 255u)

uint16_t vgafb_default_palette_1[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_WHITE,
};

uint16_t vgafb_default_palette_2[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_GREY,
    /* 02 */ IRGB_LIGHT_GREY,
    /* 03 */ IRGB_WHITE,
};

uint16_t vgafb_default_palette_4[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_RED,
    /* 02 */ IRGB_DARK_GREEN,
    /* 03 */ IRGB_DARK_YELLOW,
    /* 04 */ IRGB_DARK_BLUE,
    /* 05 */ IRGB_DARK_MAGENTA,
    /* 06 */ IRGB_DARK_CYAN,
    /* 07 */ IRGB_DARK_GREY,
    /* 08 */ IRGB_LIGHT_GREY,
    /* 09 */ IRGB_RED,
    /* 10 */ IRGB_GREEN,
    /* 11 */ IRGB_YELLOW,
    /* 12 */ IRGB_BLUE,
    /* 13 */ IRGB_MAGENTA,
    /* 14 */ IRGB_CYAN,
    /* 15 */ IRGB_WHITE};

/* Specific to 8 bits depth / 256 colors mode */
uint16_t vgafb_default_palette_8[256];

uint16_t vgafb_width = 640;
uint16_t vgafb_height = 240;
uint8_t vgafb_depth = 4;
uint16_t *vgafb_palette = vgafb_default_palette_4;
uint8_t vgafb_framebuffer_64k[65536];
uint8_t *vgafb_framebuffer = (u_int8_t *)(&vgafb_framebuffer_64k);

/* Specific to 4 bits depth / 16 colors mode */
uint32_t vgafb_double_palette_4[16 * 16];

void setup_default_palette_8()
{
    uint16_t msb[4] = {0b00000000, 0b01000000, 0b10100000, 0b11100000};
    uint16_t lsb[4] = {0b00000000, 0b00001000, 0b00010000, 0b00011000};
    uint8_t _i, _r, _g, _b;
    uint8_t i, r, g, b;
    uint16_t rgb;
    for (uint16_t c = 0; c <= 255; c++)
    {
        // 76543210 => rrggbbii
        _r = (c >> 6) & 0x03;
        _g = (c >> 4) & 0x03;
        _b = (c >> 2) & 0x03;
        _i = c & 0x03;
        r = msb[_r] | lsb[_i];
        g = msb[_g] | lsb[_i];
        b = msb[_b] | lsb[_i];
        i = lsb[i];
        rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
        // printf(
        //     "%03d: c=%08b r=%02b-%02x g=%02b-%02x b=%02b-%02x i=%02b-%02x rgb=%016b %04x\n",
        //      c,      c,     _r,  r,     _g,  g,     _b,  b,     _i,  i,       rgb,  rgb);
        vgafb_default_palette_8[c] = rgb;
    }
}

void setup_double_palette()
{
    // if (vgafb_depth == 4)
    // {
    /* vgafb_double_palette_4 is 256 entries of 32bits; */
    /* i.e. all 2 pixel combinations */
    uint32_t *double_palette = vgafb_double_palette_4;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            *double_palette = (vgafb_palette[i] << 16) | vgafb_palette[j];
            ++double_palette;
        }
    }
    // }
}

void vgafb_setup(const scanvideo_mode_t *vga_mode, uint8_t depth, uint16_t *palette)
{
    printf("VGAFB_SETUP 1\n");
    vgafb_width = vga_mode->width / vga_mode->xscale;
    vgafb_height = vga_mode->height / vga_mode->yscale;
    vgafb_depth = depth;
    vgafb_palette = palette;
    // vgafb_framebuffer = (uint8_t *)malloc(vgafb_width * vgafb_height * vgafb_depth / 8);
    // Fill screen with color #0
    // memset(vgafb_framebuffer, 0, sizeof(vgafb_framebuffer));
    // Initialize colors & palette
    printf("VGAFB_SETUP 2\n");
    setup_default_palette_8();
    printf("VGAFB_SETUP 3\n");
    setup_double_palette();
#ifdef DEBUG
    printf("Starting video\n");
#endif
#if USE_INTERP == 1
    printf("VGAFB_SETUP 3 BIS\n");
    // Configure interpolater lanes
    interp_config c = interp_default_config();
    interp_config_set_shift(&c, 22);
    interp_config_set_mask(&c, 2, 9);
    interp_set_config(interp0, 0, &c);
    interp_config_set_shift(&c, 14);
    interp_config_set_cross_input(&c, true);
    interp_set_config(interp0, 1, &c);
    interp_set_base(interp0, 0, (uintptr_t)vgafb_double_palette_4);
    interp_set_base(interp0, 1, (uintptr_t)vgafb_double_palette_4);
#endif
    printf("VGAFB_SETUP 4\n");
    scanvideo_setup(vga_mode);
    scanvideo_timing_enable(true);
#ifdef DEBUG
    printf("System clock speed %d kHz\n", clock_get_hz(clk_sys) / 1000);
#endif
    printf("VGAFB_SETUP 5\n");
}

void __time_critical_func(vgafb_render_loop)(void)
{
#ifdef DEBUG
    printf("Starting render %dx%dx%d\n", vgafb_width, vgafb_height, vgafb_depth);
#endif
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        int iScan = scanvideo_scanline_number(buffer->scanline_id);
        if (iScan % 64 == 0)
        {
            printf("%d ", iScan);
        }
        uint32_t *scanline_colors = buffer->data;
        uint8_t *vgafb_framebuffer_pixels;
        switch (vgafb_depth)
        {
        case 4:
            vgafb_framebuffer_pixels = &vgafb_framebuffer[(vgafb_width / 2) * iScan];
#if USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, vgafb_framebuffer_pixels, vgafb_width / 2);
            scanline_colors += vgafb_width / 2;
#else
            for (uint16_t iCol = 0; iCol < vgafb_width / 2; ++iCol)
            {
                ++scanline_colors;
                *scanline_colors = vgafb_double_palette_4[*vgafb_framebuffer_pixels];
                ++vgafb_framebuffer_pixels;
            }
            ++scanline_colors;
#endif
            break;
        case 8:
            for (uint16_t iCol = 0; iCol < vgafb_width; ++iCol)
            {
                ++scanline_colors; // 32 bits
                *scanline_colors = vgafb_palette[*vgafb_framebuffer_pixels] << 16;
                ++vgafb_framebuffer_pixels; // 16 bits
                *scanline_colors |= vgafb_palette[*vgafb_framebuffer_pixels];
                ++vgafb_framebuffer_pixels;
            }
            ++scanline_colors;
            break;
        default:
            break;
        }
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xFFFF0000) | (vgafb_width - 2);
        buffer->data_used = (vgafb_width + 4) / 2;
        scanvideo_end_scanline_generation(buffer);
    }
}

void vgafb_put_pixel(uint16_t x, uint16_t y, uint8_t index)
{
    uint8_t *pixel;
    uint32_t offset;
    switch (vgafb_depth)
    {
    case 4:
        offset = (vgafb_width / 2) * y + x / 2;
        if ((offset >= 0) && (offset < vgafb_width * vgafb_height / 2))
        {
            uint8_t *p = &vgafb_framebuffer[offset];
            if (x & 1)
            {
                *pixel = ((index & 0x0f) << 4) | (*pixel & 0x0f);
            }
            else
            {
                *pixel = (*pixel & 0xf0) | (index & 0x0f);
            }
        }
        break;
    case 8:
        offset = vgafb_width * y + x;
        if ((offset >= 0) && (offset < vgafb_width * vgafb_height))
        {
            vgafb_framebuffer[offset] = index;
        }
        break;
    default:
        break;
    }
}

uint8_t vgafb_get_pixel_index(uint16_t x, uint16_t y)
{
    uint16_t color = 0;
    uint32_t offset;
    switch (vgafb_depth)
    {
    case 4:
        offset = (vgafb_width / 2) * y + x / 2;
        if ((offset >= 0) && (offset < vgafb_width * vgafb_height / 2))
        {
            if (x & 1)
            {
                color = vgafb_framebuffer[offset] >> 4;
            }
            else
            {
                color = vgafb_framebuffer[offset] & 0x0f;
            }
        }
        break;
    case 8:
        offset = vgafb_width * y + x;
        if ((offset >= 0) && (offset < vgafb_width * vgafb_height))
        {
            color = vgafb_framebuffer[offset];
        }
        break;
    default:
        break;
    }
    return color;
}

uint16_t vgafb_get_color(uint8_t index)
{
    uint16_t color = vgafb_palette[index];
    return color;
}

uint16_t vgafb_get_pixel_color(uint16_t x, uint16_t y)
{
    uint8_t index = vgafb_get_pixel_index(x, y);
    return vgafb_get_color(index);
}

// EOF
