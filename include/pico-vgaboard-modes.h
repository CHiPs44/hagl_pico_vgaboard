#ifndef _HAGL_PICO_VGABOARD_MODES_H
#define _HAGL_PICO_VGABOARD_MODES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"
#include "pico-vgaboard-framebuffer.h"

    /** @brief 640x480@60Hz, 1bpp, monochrome */

    const scanvideo_mode_t vga_mode_640x480_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 1,
        .yscale = 1,
    };

    const vgaboard_t vgaboard_640x480x1 = {
        .scanvideo_mode = &vga_mode_640x480_60_chips44,
        .width = 640,
        .height = 480,
        .depth = 1,
        .colors = 2,
        .palette = ((uint16_t *)(&vgaboard_default_palette_1bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 640x240@60Hz, 2bpp, 4 colors */

    const scanvideo_mode_t vga_mode_640x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 1,
        .yscale = 2,
    };

    const vgaboard_t vgaboard_640x240x2 = {
        .scanvideo_mode = &vga_mode_640x480_60_chips44,
        .width = 640,
        .height = 240,
        .depth = 2,
        .colors = 4,
        .palette = ((uint16_t *)(&vgaboard_default_palette_2bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 320x240@60Hz, 4bpp, 16 colors */

    const scanvideo_mode_t vga_mode_320x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 2,
        .yscale = 2,
    };

    const vgaboard_t vgaboard_320x240x4 = {
        .scanvideo_mode = &vga_mode_320x240_60_chips44,
        .width = 320,
        .height = 240,
        .depth = 4,
        .colors = 16,
        .palette = ((uint16_t *)(&vgaboard_default_palette_4bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 320x120@60Hz, 8bpp, 256 colors */

    const scanvideo_mode_t vga_mode_320x120_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 2,
        .yscale = 4,
    };

    const vgaboard_t vgaboard_320x120x8 = {
        .scanvideo_mode = &vga_mode_320x240_60_chips44,
        .width = 320,
        .height = 120,
        .depth = 8,
        .colors = 256,
        .palette = ((uint16_t *)(&vgaboard_default_palette_8bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 160x240@60Hz, 8bpp, 256 colors */

    const scanvideo_mode_t vga_mode_160x240_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 4,
        .yscale = 2,
    };

    const vgaboard_t vgaboard_160x240x8 = {
        .scanvideo_mode = &vga_mode_160x240_60_chips44,
        .width = 160,
        .height = 240,
        .depth = 8,
        .colors = 256,
        .palette = ((uint16_t *)(&vgaboard_default_palette_8bpp)),
        .sys_clock_khz = 250000L,
    };

    /** @brief 160x120@60Hz, 16bpp, 65536 colors */

    const scanvideo_mode_t vga_mode_160x120_60_chips44 = {
        .default_timing = &vga_timing_640x480_60_default,
        .pio_program = &video_24mhz_composable,
        .width = 640,
        .height = 480,
        .xscale = 4,
        .yscale = 4,
    };

    const vgaboard_t vgaboard_160x120x16 = {
        .scanvideo_mode = &vga_mode_320x240_60_chips44,
        .width = 160,
        .height = 120,
        .depth = 16,
        .colors = 65536,
        .palette = NULL,
        .sys_clock_khz = 250000L,
    };



#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_MODES_H */
