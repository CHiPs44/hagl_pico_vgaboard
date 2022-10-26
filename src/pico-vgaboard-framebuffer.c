#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "pico.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/scanvideo_base.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/stdlib.h"

#include "pico-vgaboard-framebuffer.h"
#if USE_INTERP == 1
#include "hardware/interp.h"
extern void convert_from_pal16(uint32_t *dest, uint8_t *src, uint count);
#endif

static uint8_t __not_in_flash("pico_vgaboard_framebuffer") _vgaboard_framebuffer[VGABOARD_FRAMEBUFFER_SIZE];
uint8_t *vgaboard_framebuffer = (u_int8_t *)(&_vgaboard_framebuffer);
vgaboard_t __not_in_flash("pico_vgaboard_framebuffer") _vgaboard;
vgaboard_t *vgaboard = &_vgaboard;

/* Specific to 4 bits depth / 16 colors mode */
uint32_t vgaboard_double_palette_4[16 * 16];

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

void setup_default_palette_8bpp()
{
    //                     0           3           5           7
    uint16_t msb[4] = {0b00000000, 0b01100000, 0b10100000, 0b11100000};
    //                          0          11          21          31
    uint16_t lsb[4] = {0b00000000, 0b00001011, 0b00010101, 0b00011111};
    //                     0           107         181         255
    uint8_t _i, _r, _g, _b;
    // uint8_t i;
    uint8_t r, g, b;
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
        rgb = PICO_SCANVIDEO_PIXEL_FROM_RGB8(r, g, b);
        // i = lsb[i];
        // printf(
        //     "%03d: c=%08b r=%02b-%02x g=%02b-%02x b=%02b-%02x i=%02b-%02x rgb=%016b %04x\n",
        //      c,      c,     _r,  r,     _g,  g,     _b,  b,     _i,  i,       rgb,  rgb);
        vgaboard_default_palette_8bpp[c] = rgb;
    }
}

void setup_double_palette()
{
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
}

void vgaboard_init()
{
#ifdef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
    printf("vgaboard_INIT INIT\n");
#endif
    // Initialize palettes
    setup_default_palette_8bpp();
    setup_double_palette();
#if USE_INTERP == 1
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
#endif
#ifdef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
    printf("vgaboard_INIT DONE\n");
#endif
}

void vgaboard_dump(vgaboard_t *vgaboard)
{
    printf("*** VGABOARD %p ***\n", vgaboard);
    printf("\tWidth: %d\tHeight: %d\n\tDepth: %d\tColors: %d\n\tFramebuffer:%p\tFramebuffer Size: %d\n\tPalette: %p\n",
           vgaboard->width, vgaboard->height,
           vgaboard->depth, vgaboard->colors,
           vgaboard->framebuffer,vgaboard->framebuffer_size,
           vgaboard->palette);
    // printf("\t%p\t%p\n",
    //        vgaboard->palette,
    //        &vgaboard_default_palette_4bpp);
}

void vgaboard_setup(const scanvideo_mode_t *scanvideo_mode, uint8_t depth, uint16_t *palette)
{
#ifdef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
    printf("vgaboard_SETUP INIT\n");
#endif
    vgaboard->scanvideo_mode = scanvideo_mode;
    vgaboard->width = scanvideo_mode->width / scanvideo_mode->xscale;
    vgaboard->height = scanvideo_mode->height / scanvideo_mode->yscale; // scanvideo_mode->yscale_denominator;
    vgaboard->depth = depth;
    vgaboard->colors = 1 << depth;
    vgaboard->palette = palette;
    vgaboard->framebuffer = vgaboard_framebuffer;
    vgaboard->framebuffer_size = VGABOARD_FRAMEBUFFER_SIZE;
    scanvideo_setup(scanvideo_mode);
    scanvideo_timing_enable(true);
#ifdef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
    printf("-------------------\n");
#endif
}

void __not_in_flash_func(vgaboard_render_loop)(void)
{
#ifdef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
    printf("Starting render %dx%dx%d/%d@%dMHz\n", 
        vgaboard->width, vgaboard->height, 
        vgaboard->depth, vgaboard->colors, 
        clock_get_hz(clk_sys)/1000000);
#endif
    while (true)
    {
        struct scanvideo_scanline_buffer *buffer = scanvideo_begin_scanline_generation(true);
        int scanline_number = scanvideo_scanline_number(buffer->scanline_id);
        uint32_t *scanline_colors = buffer->data;
        uint8_t *vgaboard_framebuffer_pixels;
        switch (vgaboard->depth)
        {
        case 1:
            /* TODO! */
            break;
        case 2:
            /* TODO! */
            break;
        case 4:
            vgaboard_framebuffer_pixels = &(vgaboard->framebuffer[(vgaboard->width / 2) * scanline_number]);
#if USE_INTERP == 1
            ++scanline_colors;
            convert_from_pal16(scanline_colors, vgaboard_framebuffer_pixels, vgaboard->width / 2);
            scanline_colors += vgaboard->width / 2;
#else
            for (uint16_t iCol = 0; iCol < vgaboard->width / 2; ++iCol)
            {
                ++scanline_colors;
                *scanline_colors = vgaboard_double_palette_4[*vgaboard_framebuffer_pixels];
                ++vgaboard_framebuffer_pixels;
            }
            ++scanline_colors;
#endif
            break;
        case 8:
            for (uint16_t iCol = 0; iCol < vgaboard->width; ++iCol)
            {
                ++scanline_colors; // 32 bits
                *scanline_colors = vgaboard->palette[*vgaboard_framebuffer_pixels] << 16;
                ++vgaboard_framebuffer_pixels; // 16 bits
                *scanline_colors |= vgaboard->palette[*vgaboard_framebuffer_pixels];
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
        scanline_colors[1] = (scanline_colors[1] & 0xFFFF0000) | (vgaboard->width - 2);
        buffer->data_used = (vgaboard->width + 4) / 2;
        scanvideo_end_scanline_generation(buffer);
    }
}

void __not_in_flash_func(vgaboard_put_pixel)(uint16_t x, uint16_t y, uint16_t index_or_color)
{
    uint8_t *byte;
    uint32_t offset;
    uint8_t bit, bits, mask;

    switch (vgaboard->depth)
    {
    case 1:
        /* TEST! */
        offset = (vgaboard->width / 8) * y + x / 8;
        if (offset < vgaboard->width * vgaboard->height / 8)
        {
            byte = &vgaboard_framebuffer[offset];
            bit = 7 - (x % 8);
            mask = 1 << bit;
            if (index_or_color)
            {
                *byte |= mask;
            }
            else
            {
                *byte &= ~mask;
            }
        }
        break;
    case 2:
        /* TEST! */
        offset = (vgaboard->width / 4) * y + x / 4;
        if (offset < vgaboard->width * vgaboard->height / 4)
        {
            byte = &vgaboard_framebuffer[offset];
            switch (x % 4)
            {
            case 0: /* bits 7-6 */
                bits = (index_or_color % 4) << 6;
                mask = 0b00111111;
                break;
            case 1: /* bits 5-4 */
                bits = (index_or_color % 4) << 4;
                mask = 0b11001111;
                break;
            case 2: /* bits 3-2 */
                bits = (index_or_color % 4) << 2;
                mask = 0b11110011;
                break;
            case 3: /* bits 1-0 */
                bits = (index_or_color % 4);
                mask = 0b11111100;
                break;
            }
            *byte &= mask;
            *byte |= bits;
        }
        break;
    case 4:
        offset = (vgaboard->width / 2) * y + x / 2;
        if (offset < vgaboard->width * vgaboard->height / 2)
        {
            byte = &vgaboard_framebuffer[offset];
            if (x & 1)
            {
                *byte = ((index_or_color & 0x0f) << 4) | (*byte & 0x0f);
            }
            else
            {
                *byte = (*byte & 0xf0) | (index_or_color & 0x0f);
            }
        }
        break;
    case 8:
        offset = vgaboard->width * y + x;
        if (offset < vgaboard->width * vgaboard->height)
        {
            vgaboard_framebuffer[offset] = index_or_color;
        }
        break;
    case 16:
        /* TEST! */
        offset = vgaboard->width * y * 2 + x * 2;
        if (offset < vgaboard->width * vgaboard->height)
        {
            vgaboard_framebuffer[offset+0] = index_or_color >> 8;
            vgaboard_framebuffer[offset+1] = index_or_color & 0xff;
        }
        break;
    default:
        break;
    }
}

uint8_t __not_in_flash_func(vgaboard_get_pixel_index)(uint16_t x, uint16_t y)
{
    uint16_t color = 0;
    uint32_t offset;
    switch (vgaboard->depth)
    {
    case 1:
        // 8 pixels per byte
        // TODO!
        break;
    case 2:
        // 4 pixels per byte
        // TODO!
        break;
    case 4:
        // 2 pixels per byte
        offset = (vgaboard->width / 2) * y + x / 2;
        if (offset < vgaboard->width * vgaboard->height / 2)
        {
            if (x & 1)
            {
                // odd pixel => right nibble (LSB)
                color = vgaboard_framebuffer[offset] & 0x0f;
            }
            else
            {
                // even pixel => left nibble (MSB)
                color = vgaboard_framebuffer[offset] >> 4;
            }
        }
        break;
    case 8:
        // 1 pixel per byte
        offset = vgaboard->width * y + x;
        if (offset < vgaboard->width * vgaboard->height)
        {
            color = vgaboard_framebuffer[offset];
        }
        break;
    default:
        break;
    }
    return color;
}

uint16_t __not_in_flash_func(vgaboard_get_color)(uint8_t index)
{
    uint8_t mask;
    switch (vgaboard->depth)
    {
    case 1:
        mask = 0b00000001;
        break;
    case 2:
        mask = 0b00000011;
        break;
    case 4:
        mask = 0b00001111;
        break;
    case 8:
        mask = 0b11111111;
        break;
    default:
        break;
    }
    uint16_t color = vgaboard->palette[index & mask];
    return color;
}

uint16_t __not_in_flash_func(vgaboard_get_pixel_color)(uint16_t x, uint16_t y)
{
    uint8_t index = vgaboard_get_pixel_index(x, y);
    return vgaboard_get_color(index);
}

// EOF
