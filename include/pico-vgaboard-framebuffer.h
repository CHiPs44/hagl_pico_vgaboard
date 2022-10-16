#ifndef _HAGL_PICO_VGABOARD_FRAMEBUFFER_H
#define _HAGL_PICO_VGABOARD_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG
#define HAGL_PICO_VGABOARD_FRAMEBUFFER_DEBUG 0
#endif

#ifndef USE_INTERP
#define USE_INTERP 1
#endif

/**
 * @brief VGA default palette 1 bpp / 2 colors
 */
extern uint16_t vgaboard_default_palette_1bpp[  2];

/**
 * @brief VGA default palette 2 bpp / 4 colors
 */
extern uint16_t vgaboard_default_palette_2bpp[  4];

/**
 * @brief VGA default palette 4 bpp / 16 colors
 */
extern uint16_t vgaboard_default_palette_4bpp[ 16];

/**
 * @brief VGA default palette 8 bpp / 256 colors
 */
extern uint16_t vgaboard_default_palette_8bpp[256];

extern uint8_t *vgaboard_framebuffer;

/**
 * @brief VGA initialization
 */
void vgaboard_setup(const scanvideo_mode_t *vga_mode, uint8_t depth, uint16_t *palette);

/**
 * @brief VGA render loop using scanvideo's functions
 */
void vgaboard_render_loop(void);

/**
 * @brief VGA default palette 8 bpp / 256 colors
 */
void vgaboard_put_pixel(uint16_t x, uint16_t y, uint8_t index);

uint16_t vgaboard_get_color(uint8_t index);

uint8_t vgaboard_get_pixel_index(uint16_t x, uint16_t y);

uint16_t vgaboard_get_pixel_color(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGABOARD_FRAMEBUFFER_H */
