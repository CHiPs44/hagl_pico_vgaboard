#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"
#include "pico-vgaboard-framebuffer.h"
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

uint16_t vgaboard_default_palette_1bpp[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_WHITE,
};

uint16_t vgaboard_default_palette_2bpp[] = {
    /* 00 */ IRGB_BLACK,
    /* 01 */ IRGB_DARK_GREY,
    /* 02 */ IRGB_LIGHT_GREY,
    /* 03 */ IRGB_WHITE,
};

uint16_t vgaboard_default_palette_4bpp[] = {
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
uint16_t vgaboard_default_palette_8bpp[256];

/* default values 640x240 4bpp / 16 colors*/
uint16_t vgaboard_width = 640;
uint16_t vgaboard_height = 240;
uint8_t vgaboard_depth = 4;
uint16_t *vgaboard_palette = vgaboard_default_palette_4bpp;
static uint8_t __not_in_flash("pico_vgaboard_framebuffer") vgaboard_framebuffer_64k[65536];
uint8_t *vgaboard_framebuffer = (u_int8_t *)(&vgaboard_framebuffer_64k);

/* Specific to 4 bits depth / 16 colors mode */
uint32_t vgaboard_double_palette_4[16 * 16];

void setup_default_palette_8bpp()
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
        vgaboard_default_palette_8bpp[c] = rgb;
    }
}

void setup_double_palette()
{
    // if (vgaboard_depth == 4)
    // {
    /* vgaboard_double_palette_4 is 256 entries of 32bits; */
    /* i.e. all 2 pixel combinations */
    uint32_t *double_palette = vgaboard_double_palette_4;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            *double_palette = (vgaboard_default_palette_4bpp[i] << 16) | vgaboard_default_palette_4bpp[j];
            ++double_palette;
        }
    }
    // }
}

void vgaboard_setup(const scanvideo_mode_t *vga_mode, uint8_t depth, uint16_t *palette)
{
    printf("vgaboard_SETUP 1\n");
    vgaboard_width = vga_mode->width / vga_mode->xscale;
    vgaboard_height = vga_mode->height / vga_mode->yscale;
    vgaboard_depth = depth;
    vgaboard_palette = palette;
    // vgaboard_framebuffer = (uint8_t *)malloc(vgaboard_width * vgaboard_height * vgaboard_depth / 8);
    // Fill screen with color #0
    // memset(vgaboard_framebuffer, 0, sizeof(vgaboard_framebuffer));
    // Initialize colors & palette
    printf("vgaboard_SETUP 2\n");
    setup_default_palette_8bpp();
    printf("vgaboard_SETUP 3\n");
    setup_double_palette();
#ifdef DEBUG
    printf("Starting video\n");
#endif
#if USE_INTERP == 1
    printf("vgaboard_SETUP 4 INTERP\n");
    // Configure interpolater lanes
    interp_config c = interp_default_config();
    interp_config_set_shift(&c, 22);
    interp_config_set_mask(&c, 2, 9);
    interp_set_config(interp0, 0, &c);
    interp_config_set_shift(&c, 14);
    interp_config_set_cross_input(&c, true);
    interp_set_config(interp0, 1, &c);
    interp_set_base(interp0, 0, (uintptr_t)vgaboard_double_palette_4);
    interp_set_base(interp0, 1, (uintptr_t)vgaboard_double_palette_4);
#else
    printf("vgaboard_SETUP 4 NO INTERP\n");
#endif
    printf("vgaboard_SETUP 5\n");
    scanvideo_setup(vga_mode);
    scanvideo_timing_enable(true);
    printf("vgaboard_SETUP 6\n");
}

void __not_in_flash_func(vgaboard_render_loop)(void)
{
#ifdef DEBUG
    printf("Starting render %dx%dx%d@%d\n", vgaboard_width, vgaboard_height, vgaboard_depth, clock_get_hz(clk_sys));
    bool blink = true;
    int counter = 0;
#endif
    while (true)
    {
#ifdef DEBUG
        counter++;
        if (counter > 10000)
        {
            counter = 0;
            blink = !blink;
            // printf(".");
            gpio_put(PICO_DEFAULT_LED_PIN, blink ? 1 : 0);
        }
#endif
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        int iScan = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *scanline_colors = buffer->data;
        uint8_t *vgaboard_framebuffer_pixels;
        switch (vgaboard_depth)
        {
        case 4:
            vgaboard_framebuffer_pixels = &vgaboard_framebuffer[(vgaboard_width / 2) * iScan];
#if USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, vgaboard_framebuffer_pixels, vgaboard_width / 2);
            scanline_colors += vgaboard_width / 2;
#else
            for (uint16_t iCol = 0; iCol < vgaboard_width / 2; ++iCol)
            {
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_4[*vgaboard_framebuffer_pixels];
                ++vgaboard_framebuffer_pixels;
            }
            ++scanline_colors;
#endif
            break;
        case 8:
            for (uint16_t iCol = 0; iCol < vgaboard_width; ++iCol)
            {
                ++scanline_colors; // 32 bits
                *scanline_colors = vgaboard_palette[*vgaboard_framebuffer_pixels] << 16;
                ++vgaboard_framebuffer_pixels; // 16 bits
                *scanline_colors |= vgaboard_palette[*vgaboard_framebuffer_pixels];
                ++vgaboard_framebuffer_pixels;
            }
            ++scanline_colors;
            break;
        default:
            break;
        }
        *scanline_colors = COMPOSABLE_EOL_ALIGN << 16;
        scanline_colors = buffer->data;
        scanline_colors[0] = (scanline_colors[1] << 16) | COMPOSABLE_RAW_RUN;
        scanline_colors[1] = (scanline_colors[1] & 0xFFFF0000) | (vgaboard_width - 2);
        buffer->data_used = (vgaboard_width + 4) / 2;
        scanvideo_end_scanline_generation(buffer);
    }
}

void vgaboard_put_pixel(uint16_t x, uint16_t y, uint8_t index)
{
    uint8_t *pixel;
    uint32_t offset;
    switch (vgaboard_depth)
    {
    case 4:
        offset = (vgaboard_width / 2) * y + x / 2;
        if ((offset >= 0) && (offset < vgaboard_width * vgaboard_height / 2))
        {
            // uint8_t *p = &vgaboard_framebuffer[offset];
            pixel = &vgaboard_framebuffer[offset];
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
        offset = vgaboard_width * y + x;
        if ((offset >= 0) && (offset < vgaboard_width * vgaboard_height))
        {
            vgaboard_framebuffer[offset] = index;
        }
        break;
    default:
        break;
    }
}

uint8_t vgaboard_get_pixel_index(uint16_t x, uint16_t y)
{
    uint16_t color = 0;
    uint32_t offset;
    switch (vgaboard_depth)
    {
    case 4:
        offset = (vgaboard_width / 2) * y + x / 2;
        if ((offset >= 0) && (offset < vgaboard_width * vgaboard_height / 2))
        {
            if (x & 1)
            {
                color = vgaboard_framebuffer[offset] >> 4;
            }
            else
            {
                color = vgaboard_framebuffer[offset] & 0x0f;
            }
        }
        break;
    case 8:
        offset = vgaboard_width * y + x;
        if ((offset >= 0) && (offset < vgaboard_width * vgaboard_height))
        {
            color = vgaboard_framebuffer[offset];
        }
        break;
    default:
        break;
    }
    return color;
}

uint16_t vgaboard_get_color(uint8_t index)
{
    uint16_t color = vgaboard_palette[index];
    return color;
}

uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y)
{
    uint8_t index = vgaboard_get_pixel_index(x, y);
    return vgaboard_get_color(index);
}

// EOF
