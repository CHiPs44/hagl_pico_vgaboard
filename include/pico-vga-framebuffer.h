#ifndef _HAGL_PICO_VGA_FRAMEBUFFER_H
#define _HAGL_PICO_VGA_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEBUG
#define DEBUG 0
#endif
#ifndef USE_INTERP
#define USE_INTERP 1
#endif

/**
 * @brief VGA default palette 1 bpp / 2 colors
 */
extern uint16_t vgafb_default_palette_1[  2];

/**
 * @brief VGA default palette 2 bpp / 4 colors
 */
extern uint16_t vgafb_default_palette_2[  4];

/**
 * @brief VGA default palette 4 bpp / 16 colors
 */
extern uint16_t vgafb_default_palette_4[ 16];

/**
 * @brief VGA default palette 8 bpp / 256 colors
 */
extern uint16_t vgafb_default_palette_8[256];

/**
 * @brief VGA initialization
 */
void vgafb_setup(const scanvideo_mode_t *vga_mode, uint8_t depth, uint16_t *palette);

/**
 * @brief VGA render loop using scanvideo's functions
 */
void vgafb_render_loop(void);

/**
 * @brief VGA default palette 8 bpp / 256 colors
 */
void vgafb_put_pixel(uint16_t x, uint16_t y, uint8_t index);

uint16_t vgafb_get_color(uint8_t index);

uint8_t vgafb_get_pixel_index(uint16_t x, uint16_t y);

uint16_t vgafb_get_pixel_color(uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* _HAGL_PICO_VGA_FRAMEBUFFER_H */
