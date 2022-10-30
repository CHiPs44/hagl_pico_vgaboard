#ifndef _HAGL_PICO_VGABOARD_FRAMEBUFFER_H
#define _HAGL_PICO_VGABOARD_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pico/scanvideo.h"

#ifndef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
#define HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG 0
#endif

#ifndef USE_INTERP
#define USE_INTERP 1
#endif

// Framebuffer size defaults to 64K
#ifndef VGABOARD_FRAMEBUFFER_SIZE
#define VGABOARD_FRAMEBUFFER_SIZE (64 * 1024)
#endif

    typedef struct _vgaboard
    {
        const scanvideo_mode_t *scanvideo_mode;
        uint8_t freq_hz;
        uint16_t width;
        uint16_t height;
        uint8_t depth;
        uint32_t colors;           /* 65536 does not fit in an uint16_t */
        uint16_t *palette;         /* NULL for 16 bits depth / 65536 colors */
        uint32_t framebuffer_size; /* bytes */
        uint8_t *framebuffer;
        uint32_t sys_clock_khz;
    } vgaboard_t;

    /**
     * @brief VGA default palette 1 bpp / 2 colors (black & white)
     */
    extern uint16_t vgaboard_default_palette_1bpp[2];

    /**
     * @brief VGA Amstrad CPC like palette 1 bpp / 2 colors (dark blue & yellow)
     */
    extern uint16_t vgaboard_amstrad_cpc_palette_1bpp[2];

    /**
     * @brief VGA default palette 2 bpp / 4 colors
     */
    extern uint16_t vgaboard_default_palette_2bpp[4];

    /**
     * @brief VGA default palette 4 bpp / 16 colors
     */
    extern uint16_t vgaboard_default_palette_4bpp[16];

    /**
     * @brief VGA default palette 8 bpp / 256 colors
     */
    extern uint16_t vgaboard_default_palette_8bpp[256];

    /**
     * @brief VGA board internals
     */
    extern vgaboard_t *vgaboard;

    /**
     * @brief Dump VGA board state
     */
    void vgaboard_dump(vgaboard_t *vgaboard);

    /**
     * @brief VGA board initialization of 256 colors palette
     *        and optimized ones for 2 and 16 colors,
     *        to be called once at startup
     */
    void vgaboard_init();

    /**
     * @brief VGA board initialization, to be called several times
     */
    void vgaboard_setup(const vgaboard_t *model);
    // const scanvideo_mode_t *scanvideo_mode, uint8_t depth, uint16_t *palette);

    // /** @brief Enable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void vgaboard_enable();

    // /** @brief Disable VGA board (timers, PIO, DMA, interrupts, ...) */
    // void vgaboard_disable();

    /**
     * @brief VGA render loop using scanvideo's functions
     */
    void vgaboard_render_loop(void);

    /**
     * @brief Put pixel at (x, y) with color index in current palette or true color
     */
    void vgaboard_put_pixel(uint16_t x, uint16_t y, uint16_t index_or_color);

    /**
     * @brief Get RGB555 color from index in current palette
     */
    uint16_t vgaboard_get_color(uint8_t index);

    /**
     * @brief Get color index for given pixel
     */
    uint8_t vgaboard_get_pixel_index(uint16_t x, uint16_t y);

    /**
     * @brief Get RGB555 color for given pixel
     */
    uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_FRAMEBUFFER_H */
